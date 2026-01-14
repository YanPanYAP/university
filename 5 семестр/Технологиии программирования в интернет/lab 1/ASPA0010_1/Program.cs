using BSTU.Results.Collection.Configuration;
using BSTU.Results.Collection.Services;
using Microsoft.AspNetCore.Authentication.Cookies;
using BSTU.Results.Authenticate.Services;

// dotnet pack BSTU.Results.Collection -c Release
public class Program
{
    private static void Main(string[] args)
    {
        var builder = WebApplication.CreateBuilder(args);

        builder.Services.AddControllers();

        // Настройка Cookie-аутентификации
        builder.Services.AddAuthentication(CookieAuthenticationDefaults.AuthenticationScheme)
            .AddCookie(options =>
            {
                options.LoginPath = "/api/Results/SignIn";

                options.ExpireTimeSpan = TimeSpan.FromHours(1);

                options.SlidingExpiration = true;

                // Настройка событий cookie-аутентификации
                options.Events = new CookieAuthenticationEvents
                {
                    // Если пользователь не авторизован
                    OnRedirectToLogin = context =>
                    {
                        context.Response.StatusCode = 401;
                        context.Response.ContentType = "application/json";
                        var message = System.Text.Json.JsonSerializer.Serialize(new
                        {
                            error = "Unauthorized",
                            message = "Пользователь не авторизован. Выполните вход через /api/Results/SignIn"
                        });
                        return context.Response.WriteAsync(message);
                    },

                    // Если пользователь авторизован, но нет прав доступа
                    OnRedirectToAccessDenied = context =>
                    {
                        context.Response.StatusCode = 403;
                        context.Response.ContentType = "application/json";
                        var message = System.Text.Json.JsonSerializer.Serialize(new
                        {
                            error = "Forbidden",
                            message = "У вас нет прав для выполнения этого действия"
                        });
                        return context.Response.WriteAsync(message);
                    }
                };
            });

        // Transient — создаётся новый экземпляр при каждом запросе
        builder.Services.AddTransient<IResultsService>(provider =>
        {
            var options = new ResultsOptions
            {
                FilePath = Path.Combine(builder.Environment.ContentRootPath, "results.json")
            };
            return new ResultsService(options);
        });

        // Scoped — создаётся один экземпляр на каждый HTTP-запрос
        builder.Services.AddScoped<IAuthenticateService, AuthenticateService>();

        // OpenAPI/Swagger
        builder.Services.AddEndpointsApiExplorer();
        builder.Services.AddSwaggerGen();

        var app = builder.Build();

        if (app.Environment.IsDevelopment())
        {
            app.UseSwagger();
            app.UseSwaggerUI();
        }

        app.UseHttpsRedirection();

        app.UseAuthentication();
        app.UseAuthorization();

        app.MapControllers();

        app.Run();
    }
}
