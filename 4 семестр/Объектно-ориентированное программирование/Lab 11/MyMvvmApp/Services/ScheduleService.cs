using System;
using System.Collections.ObjectModel;
using System.Data.SQLite;
using System.Windows;
using MyMvvmApp.Models;
using photoLoom.Data;

namespace MyMvvmApp.Services
{
    public static class ScheduleService
    {
        public static ObservableCollection<Schedule> LoadSchedule()
        {
            var schedule = new ObservableCollection<Schedule>();

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

            using (var connection = new SQLiteConnection(photoLoom.Data.AppContext.ConnectionString))
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

                        schedule.Add(new Schedule
                        {
                            DayName = days[shiftedIndex],
                            TimeFromTo = $"{from} — {to}"
                        });
                    }
                }
            }

            return schedule;
        }
    }
}
