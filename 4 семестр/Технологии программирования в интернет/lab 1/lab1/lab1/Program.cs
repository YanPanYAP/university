internal class Program
{
    // Точка входа в приложение
    private static void Main(string[] args)
    {
        // Создание билдера приложения, который позволяет настраивать сервисы и конфигурацию
        var builder = WebApplication.CreateBuilder(args);

        // Добавление сервиса HTTP-логирования в контейнер зависимостей
        builder.Services.AddHttpLogging(options =>
        {
            // Настройка полей, которые будут логироваться (в данном случае логируются все доступные поля)
            options.LoggingFields = Microsoft.AspNetCore.HttpLogging.HttpLoggingFields.All;
        });

        // Создание экземпляра веб-приложения на основе настроек, заданных в билдере
        var app = builder.Build();

        // Включение middleware для логирования HTTP-запросов и ответов
        app.UseHttpLogging();

        // Определение маршрута для обработки HTTP GET-запросов по корневому пути ("/")
        // При обращении к корневому пути возвращается строка "Мое первое ASPA"
        app.MapGet("/", () => "Мое первое ASPA");

        // Запуск веб-приложения и начало прослушивания входящих HTTP-запросов
        app.Run();
    }
}