using System;
using System.Collections.Generic;
using System.Data.SQLite;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace photoLoom
{
    /// <summary>
    /// Логика взаимодействия для TimetablePage.xaml
    /// </summary>
    public partial class TimetablePage : Page
    {
        public TimetablePage()
        {
            InitializeComponent();
            LoadSchedule();
        }

        private void LoadSchedule()
        {
            TimetablePanel.Children.Clear();

            string[] days = new string[7];
            days[0] = (string)Application.Current.Resources["DayMonday"];
            days[1] = (string)Application.Current.Resources["DayTuesday"];
            days[2] = (string)Application.Current.Resources["DayWednesday"];
            days[3] = (string)Application.Current.Resources["DayThursday"];
            days[4] = (string)Application.Current.Resources["DayFriday"];
            days[5] = (string)Application.Current.Resources["DaySaturday"];
            days[6] = (string)Application.Current.Resources["DaySunday"];

            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();

                // Сначала понедельник (1), ..., суббота (6), потом воскресенье (0)
                string query = @"
            SELECT day_of_week, time_from, time_to
            FROM StudioSchedule
            ORDER BY 
                CASE 
                    WHEN day_of_week = 0 THEN 7 
                    ELSE day_of_week 
                END
        ";

                using (var command = new SQLiteCommand(query, connection))
                using (var reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        int dbDay = reader.GetInt32(0); // 0=Sunday, 1=Monday...
                        int visualDayIndex = (dbDay + 6) % 7; // Преобразуем 0→6, 1→0, ..., 6→5
                        string timeFrom = reader.GetString(1);
                        string timeTo = reader.GetString(2);

                        var panel = new StackPanel
                        {
                            Orientation = Orientation.Horizontal,
                            Margin = new Thickness(0, 5, 0, 5)
                        };

                        var dayLabel = new TextBlock
                        {
                            Text = days[visualDayIndex],
                            Width = 120,
                            Foreground = Brushes.White,
                            FontWeight = FontWeights.Bold,
                            FontSize = 16,
                            VerticalAlignment = VerticalAlignment.Center
                        };

                        var fromBox = new TextBox
                        {
                            Text = timeFrom,
                            Width = 80,
                            Tag = $"from_{dbDay}",
                            Margin = new Thickness(10, 0, 10, 0),
                            Foreground = Brushes.White,
                            FontWeight = FontWeights.Bold,
                            FontSize = 16,
                            Background = Brushes.Transparent,
                            BorderBrush = Brushes.Gray
                        };
                        fromBox.PreviewTextInput += TimeBox_PreviewTextInput;

                        var toBox = new TextBox
                        {
                            Text = timeTo,
                            Width = 80,
                            Tag = $"to_{dbDay}",
                            Margin = new Thickness(10, 0, 10, 0),
                            Foreground = Brushes.White,
                            FontWeight = FontWeights.Bold,
                            FontSize = 16,
                            Background = Brushes.Transparent,
                            BorderBrush = Brushes.Gray
                        };
                        toBox.PreviewTextInput += TimeBox_PreviewTextInput;

                        panel.Children.Add(dayLabel);
                        panel.Children.Add(fromBox);
                        panel.Children.Add(toBox);

                        TimetablePanel.Children.Add(panel);
                    }
                }
            }

            var saveAllButton = new Button
            {
                Content = (string)Application.Current.Resources["Save"],
                Style = (Style)Application.Current.FindResource("HighlightButtonStyle"),
                Margin = new Thickness(0, 20, 0, 0),
                HorizontalAlignment = HorizontalAlignment.Center
            };
            saveAllButton.Click += SaveAllSchedule_Click;

            TimetablePanel.Children.Add(saveAllButton);
        }




        private void SaveAllSchedule_Click(object sender, RoutedEventArgs e)
        {
            var updates = new List<(int day, string fromTime, string toTime)>();

            foreach (var child in TimetablePanel.Children)
            {
                if (child is StackPanel panel)
                {
                    int? day = null;
                    string from = null;
                    string to = null;

                    foreach (var item in panel.Children)
                    {
                        if (item is TextBox tb && tb.Tag is string tag)
                        {
                            if (tag.StartsWith("from_"))
                            {
                                day = int.Parse(tag.Split('_')[1]);
                                from = tb.Text;
                            }
                            else if (tag.StartsWith("to_"))
                            {
                                to = tb.Text;
                            }
                        }
                    }

                    if (day.HasValue)
                    {
                        if (!IsValidTime(from) || !IsValidTime(to))
                        {
                            MessageBox.Show($"Неверный формат времени для дня: {day}", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Warning);
                            return;
                        }

                        updates.Add((day.Value, from, to));
                    }
                }
            }
            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();

                foreach (var (day, from, to) in updates)
                {
                    string query = "UPDATE StudioSchedule SET time_from = @From, time_to = @To WHERE day_of_week = @Day";

                    using (var command = new SQLiteCommand(query, connection))
                    {
                        command.Parameters.AddWithValue("@From", from);
                        command.Parameters.AddWithValue("@To", to);
                        command.Parameters.AddWithValue("@Day", day);
                        command.ExecuteNonQuery();
                    }
                }
            }

            MessageBox.Show("Расписание успешно обновлено.", "Успех", MessageBoxButton.OK, MessageBoxImage.Information);
            LoadSchedule();
        }


        private void TimeBox_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            e.Handled = !System.Text.RegularExpressions.Regex.IsMatch(e.Text, @"^[0-9:]$");
        }

        private bool IsValidTime(string time)
        {
            return TimeSpan.TryParse(time, out _);
        }


        private void TimetableButton(object sender, RoutedEventArgs e)
        {
            ShowMainContent();
        }

        private void StatusButton(object sender, RoutedEventArgs e)
        {
            NavigateToPage("StatusPage.xaml");
        }

        private void ReviewButton(object sender, RoutedEventArgs e)
        {
            NavigateToPage("ReviewsPage.xaml");
        }

        private void StudiosButton(object sender, RoutedEventArgs e)
        {
            NavigateToPage("AdminPage.xaml");
        }

        private void RentButton(object sender, RoutedEventArgs e)
        {
            NavigateToPage("BookingPage.xaml");
        }

        private void NavigateToPage(string pageUri)
        {
            TimetablePageContent.Visibility = Visibility.Collapsed;
            TimetablePageFrame.Visibility = Visibility.Visible;
            TimetablePageFrame.Navigate(new Uri(pageUri, UriKind.Relative));
        }

        private void ShowMainContent()
        {
            TimetablePageFrame.Visibility = Visibility.Collapsed;
            TimetablePageContent.Visibility = Visibility.Visible;
            TimetablePageFrame.Content = null;
        }

        private void ReturnToMainContent()
        {
            TimetablePageFrame.Visibility = Visibility.Collapsed;
            TimetablePageContent.Visibility = Visibility.Visible;
            TimetablePageFrame.Content = null;
        }
        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            if (Application.Current.MainWindow is Views.MainWindow mainWindow)
            {
                mainWindow.ReturnToMainContent();
                Session.CurrentUser = null;
            }
        }
    }
}
