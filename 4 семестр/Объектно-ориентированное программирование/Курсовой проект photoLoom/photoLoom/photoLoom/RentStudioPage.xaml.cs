using System;
using System.Collections.Generic;
using System.Data.SQLite;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web.SessionState;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
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
    /// Логика взаимодействия для RentStudioPage.xaml
    /// </summary>
    public partial class RentStudioPage : Page
    {
        
        private Calendar calendar;
        private ComboBox timeFromCombo;
        private ComboBox timeToCombo;
        private TextBlock totalText;
        private Studio selectedStudio;
        double totalPrice = 0;
        private TimeSpan? fromTime = null;
        private TimeSpan? toTime = null;


        public RentStudioPage()
        {
            InitializeComponent();
            selectedStudio = Studio.SelectedStudio;
            if (selectedStudio != null)
            {
                DisplaySingleStudio(selectedStudio);
            }
            else
            {
                MessageBox.Show("Студия не выбрана");
            }

            
        }
        private void UpdateTotalPrice()
        {
            if (fromTime.HasValue && toTime.HasValue && toTime > fromTime)
            {
                double hours = (toTime.Value - fromTime.Value).TotalHours;
                totalPrice = hours * selectedStudio.price;
                int roundedPrice = (int)Math.Round(totalPrice);
                totalText.Text = $"{(string)Application.Current.Resources["RentCoast"]} {roundedPrice} Byn";
            }
            else
            {
                totalText.Text = $"{(string)Application.Current.Resources["RentCoast"]} 0 Byn";
            }
        }


        public void UpdateStudio()
        {
            selectedStudio = Studio.SelectedStudio;
            DisplaySingleStudio(selectedStudio);
        }

        private void DisplaySingleStudio(Studio studio)
        {

            MainStudioGrid.Children.Clear();
            List<string> allSlots = new List<string>();


            var mainPanel = new StackPanel
            {
                Orientation = Orientation.Vertical
            };

            mainPanel.Children.Add(new TextBlock
            {
                Text = studio.name,
                FontSize = 30,
                FontWeight = FontWeights.Bold,
                Foreground = Brushes.White
            });

            var topGrid = new Grid { Margin = new Thickness(0, 0, 0, 30) };
            topGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(2, GridUnitType.Star) });
            topGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(3, GridUnitType.Star) });

            var image = new Image
            {
                Stretch = Stretch.Uniform,
                Margin = new Thickness(0, 0, 40, 0),
                Height = 300
            };
            try { image.Source = new BitmapImage(new Uri(studio.photoPath, UriKind.RelativeOrAbsolute)); }
            catch { image.Source = null; }
            Grid.SetColumn(image, 0);
            topGrid.Children.Add(image);

            var descPanel = new StackPanel { VerticalAlignment = VerticalAlignment.Center };
            descPanel.Children.Add(new TextBlock
            {
                Text = studio.description,
                FontSize = 16,
                Foreground = Brushes.White,
                TextWrapping = TextWrapping.Wrap,
                HorizontalAlignment = HorizontalAlignment.Stretch,
            });
            descPanel.Children.Add(new TextBlock
            {
                Text = $"{(string)Application.Current.Resources["SquareLabel"]}: {studio.square} м²",
                FontSize = 14,
                Foreground = Brushes.Gray,
                Margin = new Thickness(0, 10, 0, 0)
            });
            Grid.SetColumn(descPanel, 1);
            topGrid.Children.Add(descPanel);

            mainPanel.Children.Add(topGrid);

            var bottomGrid = new Grid();
            bottomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(2, GridUnitType.Star) });
            bottomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(3, GridUnitType.Star) });

            calendar = new Calendar
            {
                Margin = new Thickness(0, 0, 20, 0),
                Background = Brushes.Transparent,
                Foreground = Brushes.White,
                BorderThickness = new Thickness(0),
                FontSize = 18,
                Padding = new Thickness(10),
                DisplayDateStart = DateTime.Today 
            };
            Grid.SetColumn(calendar, 0);

            
            var dayButtonStyle = new Style(typeof(CalendarDayButton));
            dayButtonStyle.Setters.Add(new Setter(Control.ForegroundProperty, Brushes.White));
            dayButtonStyle.Setters.Add(new Setter(Control.BackgroundProperty, Brushes.Transparent));
            dayButtonStyle.Setters.Add(new Setter(Control.BorderThicknessProperty, new Thickness(0)));
            dayButtonStyle.Setters.Add(new Setter(Control.FontSizeProperty, 18.0));
            dayButtonStyle.Setters.Add(new Setter(Control.PaddingProperty, new Thickness(10)));

            
            var mouseOverTrigger = new Trigger
            {
                Property = UIElement.IsMouseOverProperty,
                Value = true
            };
            mouseOverTrigger.Setters.Add(new Setter(Control.ForegroundProperty, new SolidColorBrush((Color)ColorConverter.ConvertFromString("#AAAAAA"))));
            dayButtonStyle.Triggers.Add(mouseOverTrigger);

            
            var selectedTrigger = new Trigger
            {
                Property = CalendarDayButton.IsSelectedProperty,
                Value = true
            };
            selectedTrigger.Setters.Add(new Setter(Control.ForegroundProperty, new SolidColorBrush((Color)ColorConverter.ConvertFromString("#AAAAAA"))));
            selectedTrigger.Setters.Add(new Setter(Control.BackgroundProperty, Brushes.Transparent));
            selectedTrigger.Setters.Add(new Setter(Control.BorderThicknessProperty, new Thickness(0)));
            dayButtonStyle.Triggers.Add(selectedTrigger);

            calendar.CalendarDayButtonStyle = dayButtonStyle;

            bottomGrid.Children.Add(calendar);

            TimeSpan endTime = new TimeSpan(22, 30, 0);

            
            DateTime now = DateTime.Now;
            if (now.TimeOfDay >= endTime)
            {
                calendar.SelectedDate = DateTime.Today.AddDays(1);
                calendar.DisplayDate = DateTime.Today.AddDays(1);
            }
            else
            {
                calendar.SelectedDate = DateTime.Today;
                calendar.DisplayDate = DateTime.Today;
            }
            
            
            calendar.SelectedDatesChanged += (s, e) =>
            {
                if (calendar.SelectedDate is DateTime selectedDate)
                {
                    DateTime today = DateTime.Today;
                    DateTime nowInner = DateTime.Now;

                    
                    if (selectedDate < today)
                    {
                        
                        calendar.SelectedDate = nowInner.TimeOfDay >= endTime ? today.AddDays(1) : today;
                        calendar.DisplayDate = calendar.SelectedDate.Value;
                    }
                    else if (selectedDate == today && nowInner.TimeOfDay >= endTime)
                    {
                        
                        calendar.SelectedDate = today.AddDays(1);
                        calendar.DisplayDate = calendar.SelectedDate.Value;
                    }

                    var booked = GetBookedSlots(studio.id, selectedDate);

                    
                    var availableSlots = GenerateHalfHourSlots(selectedDate);

                    
                    foreach (var (from, to) in booked)
                    {
                        availableSlots.RemoveAll(t =>
                        {
                            var time = TimeSpan.Parse(t);
                            return time >= from && time < to;
                        });
                    }

                    allSlots = availableSlots;

                    
                    timeFromCombo.ItemsSource = allSlots;
                    timeToCombo.ItemsSource = allSlots;

                    
                    timeFromCombo.IsEnabled = true;
                    timeToCombo.IsEnabled = true;

                }
            };

            var bookingPanel = new StackPanel
            {
                Orientation = Orientation.Vertical,
                VerticalAlignment = VerticalAlignment.Top
            };

            bookingPanel.Children.Add(new TextBlock
            {
                Text = $"{(string)Application.Current.Resources["ChoseTimeLabel"]}",
                FontWeight = FontWeights.Bold,
                FontSize = 16,
                Foreground = Brushes.White,
                Margin = new Thickness(0, 0, 0, 10)
            });

             timeFromCombo = new ComboBox
            {
                Width = 100,
                Height = 30,
                Style = (Style)Application.Current.Resources["DarkComboBoxStyle"],
                 IsEnabled = false
             };

             timeToCombo = new ComboBox
            {
                Width = 100,
                Height = 30,
                Style = (Style)Application.Current.Resources["DarkComboBoxStyle"],
                 IsEnabled = false
             };




            List<string> GenerateHalfHourSlots(DateTime selectedDate)
            {
                var slots = new List<string>();

                var workHours = GetStudioWorkHours(selectedDate.DayOfWeek);
                if (workHours == null)
                    return slots;

                var (start, end) = workHours.Value;

                TimeSpan minTime;

                if (selectedDate.Date == DateTime.Today)
                {
                    TimeSpan ynow = DateTime.Now.TimeOfDay;
                    if (ynow >= end)
                    {
                        
                        return slots;
                    }

                    int minutesToAdd = 30 - (ynow.Minutes % 30);
                    if (minutesToAdd == 30) minutesToAdd = 0;

                    minTime = ynow.Add(TimeSpan.FromMinutes(minutesToAdd));
                    if (minTime < start)
                        minTime = start;
                }
                else
                {
                    minTime = start;
                }

                for (var time = start; time <= end; time = time.Add(TimeSpan.FromMinutes(30)))
                {
                    if (time >= minTime)
                        slots.Add(time.ToString(@"hh\:mm"));
                }

                return slots;
            }

            (TimeSpan timeFrom, TimeSpan timeTo)? GetStudioWorkHours(DayOfWeek dayOfWeek)
            {
                using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                {
                    connection.Open();
                    using (var command = new SQLiteCommand("SELECT time_from, time_to FROM StudioSchedule WHERE day_of_week = @dayOfWeek", connection))
                    {
                        command.Parameters.AddWithValue("@dayOfWeek", (int)dayOfWeek);

                        using (var reader = command.ExecuteReader())
                        {
                            if (reader.Read())
                            {
                                var from = TimeSpan.Parse(reader.GetString(0));
                                var to = TimeSpan.Parse(reader.GetString(1));
                                return (from, to);
                            }
                        }
                    }
                }
                return null;
            }



            timeFromCombo.ItemsSource = allSlots;
            timeToCombo.ItemsSource = allSlots;


            timeFromCombo.SelectionChanged += (s, e) =>
            {
                if (timeFromCombo.SelectedItem is string fromStr &&
                    TimeSpan.TryParse(fromStr, out var fromParsed))
                {
                    var filtered = allSlots
                        .Where(t => TimeSpan.Parse(t) > fromParsed)
                        .ToList();

                    timeToCombo.ItemsSource = filtered;
                    fromTime = fromParsed;

                }
                else
                {
                    timeToCombo.ItemsSource = allSlots;
                }
                UpdateTotalPrice();
            };

            timeToCombo.SelectionChanged += (s, e) =>
            {
                if (timeToCombo.SelectedItem is string toStr &&
                    TimeSpan.TryParse(toStr, out var toParsed))
                {
                    var filtered = allSlots
                        .Where(t => TimeSpan.Parse(t) < toParsed)
                        .ToList();

                    timeFromCombo.ItemsSource = filtered;
                    toTime = toParsed;
                }
                else
                {
                    timeFromCombo.ItemsSource = allSlots;
                }
                
                UpdateTotalPrice();
            };

            bookingPanel.Children.Add(new StackPanel
            {
                Orientation = Orientation.Horizontal,
                Margin = new Thickness(0, 0, 0, 5),
                Children =
    {
        new TextBlock
        {
            Text = $"{(string)Application.Current.Resources["From"]}",
            FontSize = 16,
            Foreground = Brushes.White,
            Margin = new Thickness(0, 0, 5, 0),
            VerticalAlignment = VerticalAlignment.Center
        },
        timeFromCombo
    }
            });

            bookingPanel.Children.Add(new StackPanel
            {
                Orientation = Orientation.Horizontal,
                Margin = new Thickness(0, 10, 0, 10),
                Children =
    {
        new TextBlock
        {
            Text = $"{(string)Application.Current.Resources["To"]}",
            FontSize = 16,
            Foreground = Brushes.White,
            Margin = new Thickness(0, 0, 5, 0),
            VerticalAlignment = VerticalAlignment.Center
        },
        timeToCombo
    }
            });

            
            totalText = new TextBlock
            {
                Text = $"{(string)Application.Current.Resources["RentCoast"]} {totalPrice} Byn",
                FontSize = 20,
                FontWeight = FontWeights.Bold,
                Foreground = Brushes.White,
                Margin = new Thickness(0, 10, 0, 10)
            };
            bookingPanel.Children.Add(totalText);

            var button = new Button
            {
                Content = $"{(string)Application.Current.Resources["RentButton"]}",
                Width = 160,
                Cursor = Cursors.Hand,
                Style = (Style)FindResource("HighlightButtonStyle")
            };
            button.Click += RentButtonClick;
            bookingPanel.Children.Add(button);

            Grid.SetColumn(bookingPanel, 1);
            bottomGrid.Children.Add(bookingPanel);

            mainPanel.Children.Add(bottomGrid);
            MainStudioGrid.Children.Clear();
            MainStudioGrid.Children.Add(mainPanel);
        }

        private List<(TimeSpan from, TimeSpan to)> GetBookedSlots(int studioId, DateTime date)
        {
            var bookedSlots = new List<(TimeSpan from, TimeSpan to)>();

            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();
                using (var command = new SQLiteCommand(@"
            SELECT time_from, time_to
            FROM StudioRentOrders
            WHERE studio_id = @studioId AND rent_date = @date AND status != 'cancelled'", connection))
                {
                    command.Parameters.AddWithValue("@studioId", studioId);
                    command.Parameters.AddWithValue("@date", date.ToString("yyyy-MM-dd"));

                    using (var reader = command.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            if (TimeSpan.TryParse(reader.GetString(0), out var from) &&
                                TimeSpan.TryParse(reader.GetString(1), out var to))
                            {
                                bookedSlots.Add((from, to));
                            }
                        }
                    }
                }
            }

            return bookedSlots;
        }

        private bool IsIntervalAvailable(TimeSpan from, TimeSpan to, List<(TimeSpan from, TimeSpan to)> booked)
        {
            foreach (var slot in booked)
            {
                
                
                if (from < slot.to && to > slot.from)
                {
                    return false; 
                }
            }
            return true; 
        }


        private void RentButtonClick(object sender, RoutedEventArgs e)
        {
            
            if (calendar.SelectedDate == null)
            {
                return;
            }

            DateTime selectedDate = calendar.SelectedDate.Value.Date;
            DateTime today = DateTime.Today;

            
            if (selectedDate < today)
            {
                return;
            }

            
            if (timeFromCombo.SelectedItem == null || timeToCombo.SelectedItem == null)
            {
                return;
            }

            string fromStr = timeFromCombo.SelectedItem.ToString();
            string toStr = timeToCombo.SelectedItem.ToString();

            if (!TimeSpan.TryParse(fromStr, out TimeSpan fromTime) ||
                !TimeSpan.TryParse(toStr, out TimeSpan toTime))
            {
                return;
            }

            var bookedSlots = GetBookedSlots(Studio.SelectedStudio.id, selectedDate);
            if (!IsIntervalAvailable(fromTime, toTime, bookedSlots))
            {
                return;
            }

            
            bool IsValidHalfHourFormat(TimeSpan t) =>
                (t.Minutes == 0 || t.Minutes == 30) && t.Seconds == 0 && t.Milliseconds == 0;

            if (!IsValidHalfHourFormat(fromTime) || !IsValidHalfHourFormat(toTime))
            {
                return;
            }

            
            if (fromTime >= toTime)
            {
                return;
            }

            
            double hours = (toTime - fromTime).TotalHours;
            if (hours < 1)
            {
                return;
            }

            
            if (selectedDate == today)
            {
                TimeSpan nowTime = DateTime.Now.TimeOfDay;
                if (fromTime < nowTime)
                {
                    return;
                }
            }

            if (Studio.SelectedStudio == null)
            {
                return;
            }

            if (Session.CurrentUser == null)
            {
                NavigateToPage("AuthPage.xaml");
                return;
            }

            if (selectedStudio == null)
            {
                return;
            }

            try
            {
                using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                {
                    connection.Open();
                    string updateQuery = "INSERT INTO StudioRentOrders (studio_id, user_id, rent_date, time_from, time_to, total_price) VALUES (@StudioId, @UserId, @Date, @TimeFrom, @TimeTo, @TotalPrice)";
                    using (var command = new SQLiteCommand(updateQuery, connection))
                    {
                        command.Parameters.AddWithValue("@StudioId", Studio.SelectedStudio.id);
                        command.Parameters.AddWithValue("@UserId", Session.CurrentUser.id);
                        command.Parameters.AddWithValue("@Date", selectedDate.ToString("yyyy-MM-dd"));
                        command.Parameters.AddWithValue("@TimeFrom", fromTime.ToString(@"hh\:mm"));
                        command.Parameters.AddWithValue("@TimeTo", toTime.ToString(@"hh\:mm"));
                        command.Parameters.AddWithValue("@TotalPrice", totalPrice);
                        command.ExecuteNonQuery();
                    }


                    SendBookingNotification(
                    connection,
                    Session.CurrentUser.id,
                    Studio.SelectedStudio.name,
                    fromTime.ToString(@"hh\:mm"),
                    toTime.ToString(@"hh\:mm"),
                    selectedDate.ToString("yyyy-MM-dd"),
                    (int)totalPrice
                );

                    NavigateToPage("AccountPage.xaml");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при оформлении аренды: {ex.Message}", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Error);
            }

        }

        private void SendBookingNotification(SQLiteConnection connection, int userId, string studioName, string timeFrom, string timeTo, string rentDate, int totalPrice)
        {
            string messageEn = $"You booked the studio \"{studioName}\" on {rentDate} from {timeFrom} to {timeTo}. Total: {totalPrice} BYN.";
            string messageRu = $"Вы забронировали студию \"{studioName}\" на {rentDate} с {timeFrom} до {timeTo}. Итого: {totalPrice} BYN.";

            string insertNotifQuery = @"
        INSERT INTO Notifications (user_id, message_en, message_ru, send_datetime)
        VALUES (@UserId, @MessageEn, @MessageRu, @SendTime)";

            using (var notifCmd = new SQLiteCommand(insertNotifQuery, connection))
            {
                notifCmd.Parameters.AddWithValue("@UserId", userId);
                notifCmd.Parameters.AddWithValue("@MessageEn", messageEn);
                notifCmd.Parameters.AddWithValue("@MessageRu", messageRu);
                notifCmd.Parameters.AddWithValue("@SendTime", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                notifCmd.ExecuteNonQuery();
            }
        }




        private void LanguageButton_Click(object sender, RoutedEventArgs e)
        {
            var newLang = LocalizationManager.CurrentLanguage == AppLanguage.English
                ? AppLanguage.Russian
                : AppLanguage.English;

            LocalizationManager.ChangeLanguage(newLang);

            Studio.ReloadSelectedStudio();
            UpdateStudio();
        }

        private void ThemeButton_Click(object sender, RoutedEventArgs e)
        {
            var newTheme = ThemeManager.CurrentTheme == AppTheme.Dark
                ? AppTheme.Light
                : AppTheme.Dark;

            ThemeManager.ChangeTheme(newTheme);
        }

        private void HomeButton_Click(object sender, RoutedEventArgs e)
        {
            if (Application.Current.MainWindow is Views.MainWindow mainWindow)
            {
                mainWindow.ReturnToMainContent();
            }
        }

        private void StudiosButton_Click(object sender, RoutedEventArgs e)
        {
            NavigateToPage("StudiosPage.xaml");
        }

        private void PhotographersButton_Click(object sender, RoutedEventArgs e)
        {
            NavigateToPage("PhotographersPage.xaml");
        }

        private void AccountButton_Click(object sender, RoutedEventArgs e)
        {

            if (Session.CurrentUser != null)
            {
                NavigateToPage("AccountPage.xaml");
            }
            else
            {
                NavigateToPage("AuthPage.xaml");
            }
        }

        private void NavigateToPage(string pageUri)
        {
            RentStudioContent.Visibility = Visibility.Collapsed;
            RentStudioFrame.Visibility = Visibility.Visible;
            RentStudioFrame.Navigate(new Uri(pageUri, UriKind.Relative));
        }

        public void ReturnToMainContent()
        {
            RentStudioFrame.Visibility = Visibility.Collapsed;
            RentStudioContent.Visibility = Visibility.Visible;
            RentStudioFrame.Content = null;
        }

        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            NavigateToPage("StudiosPage.xaml");
        }
    }
}
