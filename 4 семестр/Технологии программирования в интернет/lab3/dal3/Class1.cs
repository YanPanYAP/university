using System.Text.Json;
using System.IO;
using System.Linq;


namespace dal3
{

    public interface IRepository : IDisposable
    {
        string BasePath { get; }                                // полный директорий для JSON и фотографий
        Celebrity[] getAllCelebrities();                        // получить весь список знаменитостей
        Celebrity? getCelebrityById(int id);                    // получить знаменитость по id
        Celebrity[] getCelebritiesBySurname(string Surname);    // получить знаменитость по фамилии
        string? getPhotoPathById(int id);                       // получить путь для GET-запроса к фотографии
    }

    public record Celebrity(int Id, string Firstname, string Surname, string PhotoPath);

    public class Repository : IRepository
    {
        private bool disposed = false; // Флаг для отслеживания вызова Dispose
        private Celebrity[] celebrities; // Массив объектов Celebrity, считанных из JSON

        // Статическое свойство, содержащее имя JSON-файла
        public static string JSONFileName;

        // Полное имя директории, в которой находятся JSON и фотографии
        public string BasePath { get; }

        // Конструктор принимает относительный путь к папке с JSON-файлом и фото
        public Repository(string relativePath)
        {
            // Получаем полный путь к директории, где хранятся данные
            BasePath = Path.Combine(AppContext.BaseDirectory, relativePath);


            // Формируем полный путь к JSON-файлу
            string jsonPath = Path.Combine(BasePath, JSONFileName);

            // Если JSON-файл не найден — выбрасываем исключение
            if (!File.Exists(jsonPath))
                throw new FileNotFoundException($"Файл {JSONFileName} не найден по пути: {jsonPath}");

            // Чтение содержимого JSON-файла как строки
            string json = File.ReadAllText(jsonPath);

            // Десериализация строки JSON в массив объектов Celebrity
            // Если JSON пуст или некорректен — подстраховка с оператором null-coalescing
            celebrities = JsonSerializer.Deserialize<Celebrity[]>(json) ?? [];
        }

        public static IRepository Create(string relativePath)
        {
            return new Repository(relativePath);
        }


        // Метод возвращает всех знаменитостей
        public Celebrity[] getAllCelebrities()
        {
            return celebrities;
        }

        // Метод возвращает знаменитость по её ID
        public Celebrity? getCelebrityById(int id)
        {
            // Возвращает первого найденного по ID или null, если не найден
            return celebrities.FirstOrDefault(c => c.Id == id);
        }

        // Метод возвращает всех знаменитостей с указанной фамилией
        public Celebrity[] getCelebritiesBySurname(string surname)
        {
            return celebrities
                .Where(c => string.Equals(c.Surname, surname, StringComparison.OrdinalIgnoreCase)) // Без учёта регистра
                .ToArray(); // Преобразуем результат в массив
        }

        // Метод возвращает путь к фото по ID знаменитости
        public string? getPhotoPathById(int id)
        {
            // Получаем объект знаменитости по ID
            var celeb = getCelebrityById(id);

            // Если не найден — возвращаем null
            if (celeb == null || string.IsNullOrWhiteSpace(celeb.PhotoPath))
                return string.Empty;

            return celeb.PhotoPath.Trim();


        }



        // Метод освобождения ресурсов
        public void Dispose()
        {
            if (!disposed)
            {
                // Здесь можно освободить ресурсы, если они будут добавлены
                disposed = true;
            }
        }
    }

}
