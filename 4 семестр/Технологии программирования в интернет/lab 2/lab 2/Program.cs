internal class Program
{
    private static void Main(string[] args)
    {
        var builder = WebApplication.CreateBuilder(args);
        var app = builder.Build();

        // Встроенный Middleware для приветственной страницы
        app.UseWelcomePage("/aspnetcore");

        // Этот обработчик НЕ сработает, потому что UseWelcomePage уже обработал этот маршрут
        app.MapGet("/aspnetcore", () => "Этот обработчик не будет выполнен!");

        app.UseDefaultFiles();
        app.UseStaticFiles();


        app.Run();

    }
}