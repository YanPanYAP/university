internal class Program
{
    private static void Main(string[] args)
    {
        var builder = WebApplication.CreateBuilder(args);
        var app = builder.Build();

        app.UseWelcomePage("/aspnetcore");
        app.MapGet("/aspnetcore", () => "Этот обработчик не будет выполнен!");

        var options = new DefaultFilesOptions();
        options.DefaultFileNames.Clear();
        options.DefaultFileNames.Add("neumann.html");

        app.UseDefaultFiles(options);
        app.UseStaticFiles();

        app.UseStaticFiles(new StaticFileOptions
        {
            RequestPath = "/static"
        });


        app.Run();

    }
}