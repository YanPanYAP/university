using System;
using System.Collections.Generic;
using System.Data.SQLite;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace photoLoom
{
    public class Studio
    {
        public int id { get; set; }
        public string name { get; set; }
        public string description { get; set; }
        public string name_ru { get; set; }
        public string description_ru { get; set; }
        public double price { get; set; }
        public string photoPath { get; set; }
        public int square { get; set; }
        public bool IsEditing { get; set; } = false;
        public static Studio SelectedStudio { get; set; }

        public List<Studio> LoadStudios()
        {
            List<Studio> studios = new List<Studio>();

            string nameColumn = LocalizationManager.CurrentLanguage == AppLanguage.Russian ? "name_ru" : "name";
            string descriptionColumn = LocalizationManager.CurrentLanguage == AppLanguage.Russian ? "description_ru" : "description";

            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();
                string query = $"SELECT id, {nameColumn}, {descriptionColumn}, price, photoPath, square FROM Studios";

                using (var command = new SQLiteCommand(query, connection))
                using (var reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        studios.Add(new Studio
                        {
                            id = Convert.ToInt32(reader["id"]),
                            name = reader[nameColumn].ToString(),
                            description = reader[descriptionColumn].ToString(),
                            price = Convert.ToDouble(reader["price"]),
                            photoPath = reader["photoPath"].ToString(),
                            square = Convert.ToInt32(reader["square"])
                        });
                    }
                }
            }

            return studios;
        }

        public List<Studio> LoadAllStudios()
        {
            List<Studio> studios = new List<Studio>();

            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();
                string query = @"SELECT id, name, description, name_ru, description_ru, 
                        price, photoPath, square FROM Studios";

                using (var command = new SQLiteCommand(query, connection))
                using (var reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        studios.Add(new Studio
                        {
                            id = Convert.ToInt32(reader["id"]),
                            name = reader["name"].ToString(),
                            description = reader["description"].ToString(),
                            name_ru = reader["name_ru"].ToString(),
                            description_ru = reader["description_ru"].ToString(),
                            price = Convert.ToDouble(reader["price"]),
                            photoPath = reader["photoPath"].ToString(),
                            square = Convert.ToInt32(reader["square"])
                        });
                    }
                }
            }
            return studios;
        }

        public static void ReloadSelectedStudio()
        {
            if (SelectedStudio == null) return;

            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();

                string nameColumn = LocalizationManager.CurrentLanguage == AppLanguage.Russian ? "name_ru" : "name";
                string descriptionColumn = LocalizationManager.CurrentLanguage == AppLanguage.Russian ? "description_ru" : "description";

                string query = $"SELECT {nameColumn}, {descriptionColumn}, price, photoPath, square " +
                              $"FROM Studios WHERE id = {SelectedStudio.id}";

                using (var command = new SQLiteCommand(query, connection))
                using (var reader = command.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        SelectedStudio.name = reader[nameColumn].ToString();
                        SelectedStudio.description = reader[descriptionColumn].ToString();
                        SelectedStudio.price = Convert.ToDouble(reader["price"]);
                        SelectedStudio.photoPath = reader["photoPath"].ToString();
                        SelectedStudio.square = Convert.ToInt32(reader["square"]);
                    }
                }
            }
        }

    }



}
