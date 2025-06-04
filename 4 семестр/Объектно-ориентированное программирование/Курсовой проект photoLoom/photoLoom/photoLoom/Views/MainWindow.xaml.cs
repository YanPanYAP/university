using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
using photoLoom;

namespace photoLoom
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    /// 
    public partial class MainWindow : Window
    {
        public ObservableCollection<ScheduleViewModel> Schedule { get; set; }
        public MainWindow()
        {
            InitializeComponent();
            
            var reviews = LoadReviews();
            ReviewsPanel.ItemsSource = LoadReviews();
            NoReviewsTextBlock.Visibility = reviews.Count == 0 ? Visibility.Visible : Visibility.Collapsed;


            Schedule = LoadSchedule();
            ScheduleList.ItemsSource = Schedule;

            this.Closing += MainWindow_Closing;
        }
        public class ReviewViewModel
        {
            public string UserLogin { get; set; }
            public string UserPhotoPath { get; set; }
            public string ReviewText { get; set; }
            public string DateReviewText { get; set; }
        }

        public ObservableCollection<ReviewViewModel> LoadReviews()
        {
            var reviews = new ObservableCollection<ReviewViewModel>();

            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();

                string query = @"
                    SELECT Users.login, Users.photoPath, Reviews.review_text, Reviews.created_at
                    FROM Reviews
                    JOIN Users ON Users.id = Reviews.user_id
                    ORDER BY datetime(Reviews.created_at) DESC
                    LIMIT 20;
                ";


                using (var command = new SQLiteCommand(query, connection))
                using (var reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        reviews.Add(new ReviewViewModel
                        {
                            UserLogin = reader["login"].ToString(),
                            UserPhotoPath = reader["photoPath"].ToString(),
                            ReviewText = reader["review_text"].ToString(),
                            DateReviewText = DateTime.Parse(reader["created_at"].ToString()).ToString("dd.MM.yyyy")
                        });
                    }
                }
            }

            return reviews;
        }

        public class ScheduleViewModel
        {
            public string DayName { get; set; }
            public string TimeFromTo { get; set; }
        }

        public ObservableCollection<ScheduleViewModel> LoadSchedule()
        {
            var schedule = new ObservableCollection<ScheduleViewModel>();
            string[] days =
            {
                (string)Application.Current.Resources["DayMonday"],
                (string)Application.Current.Resources["DayTuesday"],
                (string)Application.Current.Resources["DayWednesday"],
                (string)Application.Current.Resources["DayThursday"],
                (string)Application.Current.Resources["DayFriday"],
                (string)Application.Current.Resources["DaySaturday"],
                (string)Application.Current.Resources["DaySunday"]
            };


            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();

                string query = @"
            SELECT day_of_week, time_from, time_to
            FROM StudioSchedule
            ORDER BY CASE 
                WHEN day_of_week = 0 THEN 7
                ELSE day_of_week
            END
        ";

                using (var command = new SQLiteCommand(query, connection))
                using (var reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        int dayIndex = Convert.ToInt32(reader["day_of_week"]);
                        string from = reader["time_from"].ToString();
                        string to = reader["time_to"].ToString();

                        
                        int shiftedIndex = (dayIndex + 6) % 7;

                        schedule.Add(new ScheduleViewModel
                        {
                            DayName = days[shiftedIndex],
                            TimeFromTo = $"{from} — {to}"
                        });
                    }
                }
            }

            return schedule;
        }



        private void LanguageButton_Click(object sender, RoutedEventArgs e)
        {
            var newLang = LocalizationManager.CurrentLanguage == AppLanguage.English
                ? AppLanguage.Russian
                : AppLanguage.English;

            LocalizationManager.ChangeLanguage(newLang);
            Schedule = LoadSchedule();
            ScheduleList.ItemsSource = Schedule;
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
            ShowMainContent();
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

        public void NavigateToPage(string pageUri)
        {
            MainContent.Visibility = Visibility.Collapsed;
            MainFrame.Visibility = Visibility.Visible;
            MainFrame.Navigate(new Uri(pageUri, UriKind.Relative));
        }

        private void ShowMainContent()
        {
            MainFrame.Visibility = Visibility.Collapsed;
            MainContent.Visibility = Visibility.Visible;
            MainFrame.Content = null;
        }

        public void ReturnToMainContent()
        {
            MainFrame.Visibility = Visibility.Collapsed;
            MainContent.Visibility = Visibility.Visible;
            MainFrame.Content = null;
        }

        private void MainWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            Session.CurrentUser = null;
        }
    }

}
