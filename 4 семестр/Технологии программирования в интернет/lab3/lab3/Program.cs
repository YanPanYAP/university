using dal3;
using Microsoft.AspNetCore.Builder;
using Microsoft.Extensions.FileProviders;
using Microsoft.AspNetCore.StaticFiles;

var builder = WebApplication.CreateBuilder(args);
var app = builder.Build();

app.UseStaticFiles();

Repository.JSONFileName = "Celebrities.json";

var celebritiesPath = Path.Combine(AppContext.BaseDirectory, "Celebrities");

app.UseStaticFiles(new StaticFileOptions
{
    FileProvider = new PhysicalFileProvider(celebritiesPath),
    RequestPath = "/Celebrities/download",
    OnPrepareResponse = ctx =>
    {
        var fileName = Path.GetFileName(ctx.File.PhysicalPath);
        ctx.Context.Response.Headers.Append("Content-Disposition", $"attachment; filename=\"{fileName}\"");
    }
});

app.UseDirectoryBrowser(new DirectoryBrowserOptions
{
    FileProvider = new PhysicalFileProvider(celebritiesPath),
    RequestPath = "/Celebrities/download"
});

using (IRepository repository = Repository.Create("Celebrities"))
{
    app.MapGet("/Celebrities", () => repository.getAllCelebrities());
    app.MapGet("/Celebrities/{id:int}", (int id) => repository.getCelebrityById(id));
    app.MapGet("/Celebrities/BySurname/{surname}", (string surname) => repository.getCelebritiesBySurname(surname));
    app.MapGet("/Celebrities/PhotoPathById/{id:int}", (int id) => repository.getPhotoPathById(id));
    app.MapGet("/", () => "Hello World!");
    app.Run();
}