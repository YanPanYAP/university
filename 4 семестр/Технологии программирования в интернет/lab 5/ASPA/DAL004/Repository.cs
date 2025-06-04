﻿using System.Reflection.Metadata.Ecma335;
using System.Text.Json;

namespace DAL004
{
    public interface IRepository:IDisposable
    {
        string BasePath { get; }
        Celebrity[] GetAllCelebrities();
        Celebrity? GetCelebrityById(int id);
        Celebrity[] GetCelebritiesBySurename(string surename);
        string? GetPhotoPathById(int id);
        int? addCelebrity(Celebrity celeb);
        bool delCelebrity(int id);
        int? updCelebrityById(int id , Celebrity celeb);
        int saveChanges();
    }

    public record Celebrity(int Id, string Firstname, string Surname, string PhotoPath);

    public class Repository:IRepository
    {
        private int _lastId;
        public static string JSONFileName = "Celebrities.json";
        public string BasePath { get; }
        public string filePath { get; }
        public List<Celebrity> _celebrities;

        public Repository(string dirPath)
        {
            this.BasePath = Path.Combine(Directory.GetCurrentDirectory(), dirPath);
            this.filePath = Path.Combine(BasePath, JSONFileName);
           
            if (!Directory.Exists(this.BasePath))
            {
                Directory.CreateDirectory(this.BasePath);
            }
            if (!File.Exists(this.filePath))
            {
                File.WriteAllText(this.filePath, "[]");
            }
            LoadData();
        }

        private void LoadData()
        {
            var json = File.ReadAllText(filePath);
            _celebrities = JsonSerializer.Deserialize<List<Celebrity>>(json) ?? new List<Celebrity>();
            var tempcelebrities = _celebrities.ToArray();
            _lastId = tempcelebrities[tempcelebrities.Length - 1].Id;
        }

        public Celebrity[] GetAllCelebrities()
        {
            return _celebrities.ToArray();
        }

        public Celebrity? GetCelebrityById(int id)
        {
            return _celebrities.FirstOrDefault(c => c.Id == id);
        }

        public Celebrity[] GetCelebritiesBySurename(string surename)
        {
            return _celebrities.Where(c => c.Surname.Equals(surename, StringComparison.OrdinalIgnoreCase)).ToArray();
        }

        public string? GetPhotoPathById(int id)
        {
            return GetCelebrityById(id)?.PhotoPath;
        }

        public static Repository Create(string dir) 
        {
            return new Repository(dir);
        }
        public int? addCelebrity(Celebrity celeb)
        {
            if (this._celebrities.Find(c => c.Id == celeb.Id) != null||celeb.Id==0)
            {
                celeb = new Celebrity(++_lastId,celeb.Firstname,celeb.Surname,celeb.PhotoPath);
                this._celebrities.Add(celeb);
                return celeb.Id;
            }
            else
            {
                this._celebrities.Add(celeb);
                return celeb.Id;
            }
            
        }

        public bool delCelebrity(int id)
        {
            if(this._celebrities.Find(c=>c.Id == id) != null)
            {
                this._celebrities.RemoveAt(this._celebrities.FindIndex(c => c.Id == id));
                return true;
            }
            else
            {
                return false;
            }
        }

        public int? updCelebrityById(int id, Celebrity celeb)
        {
            var existingCelebrity = _celebrities.FirstOrDefault(c => c.Id == id);
            if (existingCelebrity != null)
            {
                // Создаем новый объект Celebrity с обновленными значениями
                var updatedCelebrity = existingCelebrity with
                {
                    Firstname = celeb.Firstname,
                    Surname = celeb.Surname,
                    PhotoPath = celeb.PhotoPath
                };

                // Заменяем старую запись на новую
                _celebrities[_celebrities.FindIndex(c => c.Id == id)] = updatedCelebrity;
                return updatedCelebrity.Id;
            }
            return -1; // Знаменитость не найдена
        }

        public int saveChanges()
        {
            int beforeUpdLength = File.ReadAllText(this.filePath).Length;
            var updatedJsonString = JsonSerializer.Serialize(this._celebrities);
            File.WriteAllText(this.filePath, updatedJsonString);
            int afterUpdLength = File.ReadAllText(this.filePath).Length;

            return afterUpdLength - beforeUpdLength;
        }
       

        public void Dispose()
        {

        }
    }
}
