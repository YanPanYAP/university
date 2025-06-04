using System;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Text.Json;
using System.IO;
using System.Linq;
using dal3;

namespace dal3
{
    class Program
    {
        private static void Main()
        {
            Repository.JSONFileName = "Celebrities.json"; // Имя файла
            using IRepository repository = Repository.Create("Celebrities"); // Директория с данными

            // Вывод всех знаменитостей
            PrintCelebrities(repository.getAllCelebrities());

            // Проверка поиска по ID
            var idsToCheck = new[] { 1, 3, 7, 222 };
            foreach (var id in idsToCheck)
            {
                PrintCelebrityById(repository, id);
            }

            // Поиск по фамилии
            var surnamesToCheck = new[] { "Chomsky", "Knuth", "XXXX", "Codd" };
            foreach (var surname in surnamesToCheck)
            {
                PrintCelebrities(repository.getCelebritiesBySurname(surname));
            }

            // Получение пути к фото
            var photoIds = new[] { 4, 6, 222 };
            foreach (var id in photoIds)
            {
                Console.WriteLine($"PhotoPathById {id} = {repository.getPhotoPathById(id)}");
            }
        }

        //Вывод списка знаменитостей
        private static void PrintCelebrities(IEnumerable<Celebrity> celebrities)
        {
            foreach (var celebrity in celebrities)
            {
                Console.WriteLine($"Id = {celebrity.Id}, Firstname = {celebrity.Firstname}, " +
                                  $"Surname = {celebrity.Surname}, PhotoPath = {celebrity.PhotoPath}");
            }
        }

        //Вывод информации о знаменитости по ID
        private static void PrintCelebrityById(IRepository repository, int id)
        {
            var celebrity = repository.getCelebrityById(id);
            if (celebrity != null)
            {
                Console.WriteLine($"Id = {celebrity.Id}, Firstname = {celebrity.Firstname}, " +
                                  $"Surname = {celebrity.Surname}, PhotoPath = {celebrity.PhotoPath}");
            }
            else
            {
                Console.WriteLine($"Not Found {id}");
            }
        }
    }
}