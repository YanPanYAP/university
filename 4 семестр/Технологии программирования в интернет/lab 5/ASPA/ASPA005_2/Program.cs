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

            RouteGroupBuilder api = app.MapGroup("/Celebrities");

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

            app.MapPut("/Celebrities/{id:int}", (int id, Celebrity celebrity) =>
            {
                var result = repository.updCelebrityById(id, celebrity);
                if (result == -1)
                {
                    throw new PutCelebrityById($"Celebrity id {id} not found for put.");
                }
                var updatedCelebrity = repository.GetCelebrityById(id);
                return Results.Ok(updatedCelebrity);
            }).AddEndpointFilter<IdFilter>();

            app.MapDelete("/Celebrities/{id:int}", (int id) =>
            {
                bool deleted = repository.delCelebrity(id);
                if (!deleted)
                {
                    throw new DeleteCelebrityById($"Celebrity id {id} not found for deletion.");
                }
                return Results.Ok($"Celebrity id {id} deleted.");
            }).AddEndpointFilter<IdFilter>();


            SurnameFilter.repository = PhotoExistFilter.repository = /*FirstnameFilter.repository =*/ IdFilter.repository = repository;

            app.MapPost("/Celebrities", (Celebrity celebrity) =>
            {
                int? id = repository.addCelebrity(celebrity);
                if (id == null)
                {
                    throw new AddCelebrityException("/Celebrities error, id was null");
                }
                if (celebrity.PhotoPath == null)
                {
                    throw new PhotoPathException("/PhotoPath error, path was null");
                }
                if (repository.saveChanges() <= 0)
                {
                    throw new SaveException("/Celebrities error, SaveChanges() < 0");
                }
                return new Celebrity((int)id, celebrity.Firstname, celebrity.Surname, celebrity.PhotoPath);
            }).AddEndpointFilter<SurnameFilter>() 
            .AddEndpointFilter<PhotoExistFilter>(); 


            app.MapFallback((HttpContext ctx) =>
                Results.NotFound(new { error = $"Path {ctx.Request.Path} not supported" }));

            app.Map("/Celebrities/Error", (HttpContext ctx) =>
            {
                Exception? ex = ctx.Features.Get<IExceptionHandlerFeature>()?.Error;
                IResult rc = Results.Problem(detail: "Panic", instance: app.Environment.EnvironmentName, title: "ASPA004", statusCode: 500);
                if (ex != null)
                {
                    rc = ex switch
                    {
                        FoundByIdException => Results.NotFound(ex.Message),
                        BadHttpRequestException => Results.BadRequest(ex.Message),
                        CelebryIsNull => Results.Problem(title: "ASP004/CelebrityIsNull", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500),
                        SaveException => Results.Problem(title: "ASP004/SaveChanges", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500),
                        AddCelebrityException => Results.Problem(title: "ASP004/AddCelebrity", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500),
                        DeleteCelebrityById => Results.Problem(title: "ASP004/delCelebrity", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500),
                        PutCelebrityById => Results.Problem(title: "ASP004/putCelebrity", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500),
                        PhotoPathException => Results.Problem(title: "ASP004/PhotoPath", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500),
                        SurnameEquals => Results.Problem(title: "ASP004/SurnameEquals", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500),
                        SurnameIsNull => Results.Problem(title: "ASP004/SurnameIsNull", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500),
                        _ => rc
                    };
                }
                return rc;
            });
        }

        app.Run();
    }
}

