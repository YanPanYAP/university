using photoLoom.Models;
using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Data.SQLite;
using System.Runtime.CompilerServices;
using System.Windows;
using photoLoom.Commands;
using System.Windows.Input;

namespace photoLoom.ViewModels
{
    public class MainViewModel : INotifyPropertyChanged
    {
        public ObservableCollection<Review> Reviews { get; set; }
        public ObservableCollection<ScheduleItem> Schedule { get; set; }
        public ICommand HomeCommand { get; set; }

        public MainViewModel()
        {
            Reviews = LoadReviews();
            Schedule = LoadSchedule();
            HomeCommand = new RelayCommand(_ => ExecuteHomeCommand());
        }



        private void ExecuteHomeCommand()
        {
            MessageBox.Show("Команда Home выполнена!");
        }
        private ObservableCollection<Review> LoadReviews()
        {
            var reviews = new ObservableCollection<Review>();

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

        private ObservableCollection<ScheduleItem> LoadSchedule()
        {
            var schedule = new ObservableCollection<ScheduleItem>();
            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();
                string query = "SELECT * FROM StudioSchedule ORDER BY id;";
                using (var command = new SQLiteCommand(query, connection))
                using (var reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        schedule.Add(new ScheduleItem
                        {
                            DayName = reader["day_name"].ToString(),
                            TimeFromTo = reader["time_from"].ToString() + " - " + reader["time_to"].ToString()
                        });
                    }
                }
            }

            return schedule;
        }

        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged([CallerMemberName] string name = "") =>
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
    }
}
