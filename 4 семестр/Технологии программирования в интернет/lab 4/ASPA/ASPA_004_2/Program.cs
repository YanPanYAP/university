
using CustomExceptions;
using DAL004;
using Microsoft.AspNetCore.Diagnostics;

internal class Program
{
    private static void Main(string[] args)
    {
        var builder = WebApplication.CreateBuilder(args);
        var app = builder.Build();

        app.MapGet("/", () => "Hello World!");


        Repository.JSONFileName = "Celebrities.json";
        using (IRepository repository = Repository.Create("Celebrities"))
        {
            app.UseExceptionHandler("/Celebrities/Error");

            app.MapGet("/Celebrities", () => repository.GetAllCelebrities());

            app.MapGet("/Celebrities/{id:int}", (int id) =>
            {
                Celebrity? celebrity = repository.GetCelebrityById(id);
                if (celebrity == null)
                {
                    throw new FoundByIdException($"Celebrity id was {id}");
                }
                return celebrity;
            });

            app.MapPost("/Celebrities", (Celebrity celebrity) =>
            {
                int? id = repository.addCelebrity(celebrity);
                if (id == null)
                {
                    throw new AddCelebrityException("/Celebrities error, id was null");
                }

                if (!celebrity.PhotoPath.ToLower().Contains(celebrity.Surname.ToLower()))
                {
                    throw new Exception($"Could not find file: {repository.BasePath}");
                }

                if (repository.saveChanges() <= 0)
                {
                    throw new SaveException("/Celebrities error, SaveChanges() < 0");
                }
                return new Celebrity((int)id, celebrity.Firstname, celebrity.Surname, celebrity.PhotoPath);
            });

            app.MapDelete("/Celebrities/{id:int}", (int id) =>
            {
                bool deleted = repository.delCelebrity(id);
                if (!deleted)
                {
                    throw new DeletionException($"Could not delete celebrity with id {id}");
                }

                return $"Celebrity with id {id} was deleted\n";

            });

            app.MapFallback((HttpContext ctx) => Results.NotFound(new { error = $"Path {ctx.Request.Path} not supported" }));

            app.Map("/Celebrities/Error", (HttpContext ctx) =>
            {
                Exception? ex = ctx.Features.Get<IExceptionHandlerFeature>()?.Error;
                IResult rc = Results.Problem(detail: ex.Message, instance: app.Environment.EnvironmentName, title: "ASPA004", statusCode: 500);
                if (ex != null)
                {
                    if(ex is DeletionException)
                    {
                        rc = Results.NotFound(ex.Message);
                    }
                    if (ex is FoundByIdException)
                    {
                        rc = Results.NotFound(ex.Message);
                    }
                    if (ex is BadHttpRequestException)
                    {
                        rc = Results.BadRequest(ex.Message);
                    }
                    if (ex is SaveException)
                    {
                        rc = Results.Problem(title: "ASP004/SaveChanges", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500);
                    }
                    if (ex is AddCelebrityException)
                    {
                        rc = Results.Problem(title: "ASP004/AddCelebrity", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500);
                    }
                }
                return rc;
            });
        }

        app.Run();
    }
}