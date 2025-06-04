using MyMvvmApp.Services;
using photoLoom.Models;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Data.SQLite;
using photoLoom.Data;
using System.Windows;
using MyMvvmApp.Models;

namespace MyMvvmApp.ViewModels
{
    public class MainViewModel : INotifyPropertyChanged
    {
        private string _greeting;
        public string Greeting
        {
            get => _greeting;
            set { _greeting = value; OnPropertyChanged(); }
        }

        public ICommand ChangeLanguageCommand { get; }
        public ICommand ChangeThemeCommand { get; }
        public ObservableCollection<Review> Reviews { get; set; }
        public ObservableCollection<Schedule> Schedule { get; set; }
        public MainViewModel()
        {
            try
            {
                Reviews = LoadReviews();
                Schedule = ScheduleService.LoadSchedule();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при загрузке отзывов: {ex.Message}", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Error);
            }

            ChangeLanguageCommand = new RelayCommand(ChangeLanguage);
            ChangeThemeCommand = new RelayCommand(ChangeTheme);
        }

        private void RefreshSchedule()
        {
            Schedule = ScheduleService.LoadSchedule();
            OnPropertyChanged(nameof(Schedule));
        }


        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged([CallerMemberName] string name = null) =>
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));

        private ObservableCollection<Review> LoadReviews()
        {
            var reviews = new ObservableCollection<Review>();

            using (var connection = new SQLiteConnection(photoLoom.Data.AppContext.ConnectionString))
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
                        reviews.Add(new Review
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

        private void ChangeLanguage()
        {
            var newLang = LocalizationManager.CurrentLanguage == AppLanguage.English
                ? AppLanguage.Russian
                : AppLanguage.English;

            LocalizationManager.ChangeLanguage(newLang);
            RefreshSchedule();
        }

        private void ChangeTheme()
        {
            var newTheme = ThemeManager.CurrentTheme == AppTheme.Dark
                ? AppTheme.Light
                : AppTheme.Dark;

            ThemeManager.ChangeTheme(newTheme);
        }

    }

}
