using ASPA_006_1;
using DAL_Celebrity_MSSQL;
using Microsoft.AspNetCore.Diagnostics;
using Microsoft.Extensions.Options;
using Exceptions;
internal class Program
{
    private static void Main(string[] args)
    {
        string CS = "Server=192.168.1.111,1433; Database=Lab6_Db; User Id=sa; Password=YourStrong@Passw0rd; TrustServerCertificate=True";
        Init init = new Init(CS);

        Init.Execute(create: true, delete: true);

        IConfiguration configuration = new ConfigurationBuilder().AddJsonFile("Celebrities.config.json").Build();
        var builder = WebApplication.CreateBuilder(args);
        builder.Services.AddControllersWithViews();
        builder.Services.AddScoped<IRepository, Repository>((IServiceProvider p) =>
                                                                                   
        {
            // Получаем настройки из конфигурационного файла через механизм IOptions.
            CelebritiesConfig config = p.GetRequiredService<IOptions<CelebritiesConfig>>().Value;
            return new Repository(config.ConnectionString);
        });

        var app = builder.Build();

        app.UseDefaultFiles();
        app.UseStaticFiles();

        app.UseExceptionHandler("/Errors");

        var celebrities = app.MapGroup("/api/Celebrities");

        // Добавьте после app.MapGroup("/api/celebrities")

        // Обработка страницы добавления новой знаменитости (/0)
        app.MapGet("/0", (HttpContext context) =>
        {
            return Results.Content(
                """
        <!DOCTYPE html>
        <html>
        <head>
            <title>New Celebrity</title>
            <style>
                body { font-family: Arial, sans-serif; margin: 20px; }
                h2 { color: #333; }
                table { margin-bottom: 15px; }
                input[type="text"], input[type="file"] { padding: 5px; width: 200px; }
                button, .button { 
                    padding: 5px 15px; 
                    margin-right: 10px; 
                    cursor: pointer; 
                }
            </style>
        </head>
        <body>
            <h2>New Celebrity</h2>
            <form id="celebrityForm" enctype="multipart/form-data">
                <table>
                    <tr>
                        <td>Celebrity Name:</td>
                        <td><input type="text" id="FullName" required></td>
                    </tr>
                    <tr>
                        <td>Nation:</td>
                        <td><input type="text" id="Nationality" required maxlength="2"></td>
                    </tr>
                    <tr>
                        <td>Upload Photo:</td>
                        <td><input type="file" id="Photo" accept=".jpg,.jpeg,.png" required></td>
                    </tr>
                </table>
                <div>
                    <button type="button" onclick="submitForm()">OK</button>
                    <a href="/" class="button">CANCEL</a>
                </div>
            </form>

            <script>
                async function submitForm() {
                    const formData = new FormData();
                    formData.append('FullName', document.getElementById('FullName').value);
                    formData.append('Nationality', document.getElementById('Nationality').value);
                    formData.append('Photo', document.getElementById('Photo').files[0]);

                    try {
                        const response = await fetch('/api/celebrities/upload', {
                            method: 'POST',
                            body: formData
                        });

                        if (response.ok) {
                            window.location.href = '/';
                        } else {
                            const error = await response.text();
                            alert('Error: ' + error);
                        }
                    } catch (error) {
                        alert('Network error: ' + error);
                    }
                }
            </script>
        </body>
        </html>
        """,
                "text/html"
            );
        });

        // Обработка POST запроса для добавления
        app.MapPost("/api/celebrities/upload", async (IRepository repo, IFormFile Photo, string FullName, string Nationality) =>
        {
            var config = app.Configuration.GetSection("Celebrities");
            var photosFolder = config["PhotosFolder"];
            var maxSize = int.Parse(config["MaxPhotoSizeMB"] ?? "5") * 1024 * 1024;

            // Валидация
            if (Photo == null || Photo.Length == 0)
                return Results.BadRequest("Photo is required");

            if (Photo.Length > maxSize)
                return Results.BadRequest($"Photo size exceeds {maxSize / 1024 / 1024}MB limit");

            // Сохранение файла
            var fileName = $"{Guid.NewGuid()}{Path.GetExtension(Photo.FileName)}";
            var filePath = Path.Combine(photosFolder!, fileName);

            Directory.CreateDirectory(photosFolder!);
            using (var stream = new FileStream(filePath, FileMode.Create))
            {
                await Photo.CopyToAsync(stream);
            }

            // Добавление в БД
            var celebrity = new Celebrity
            {
                FullName = FullName,
                Nationality = Nationality,
                ReqPhotoPath = fileName
            };

            if (!repo.AddCelebrity(celebrity))
                return Results.Problem("Failed to add celebrity");

            return Results.Created($"/api/celebrities/{celebrity.Id}", celebrity);
        });

        //все знаменитости
        celebrities.MapGet("/", (IRepository repo) => repo.GetAllCelebrities());
            
        //знаменитость по  id

        celebrities.MapGet("/{id:int:min(1)}", (IRepository repo, int id) =>
        {
            if (repo.GetCelebrityById(id) != null)
            {
                return repo.GetCelebrityById(id);
            }
            else
            {
                throw new GetByIdException($"Failed to get celebrity by {id} id. Such id does not exist");
            }
        });

        //хнаментость по id событиям

        celebrities.MapGet("/LifeEvents/{id:int:min(1)}", (IRepository repo, int id) =>
        {
            Celebrity? celeb = repo.GetCelebrityByLifeEventId(id);
            if(celeb != null)
            {
                return celeb;
            }
            else
            {
                throw new GetByIdException($"Could not get a celebrity by {id} id . Such id does not exist");
            }
        });

        //удалить по id

        celebrities.MapDelete("/{id:int:min(1)}", (IRepository repo, int id) =>
        {
            if (repo.DeleteCelebrity(id))
            {
                return $"Celebrity with id {id} was successfully deleted!";
            }
            else
            {
                throw new DeleteByIdException($"Failed to delete celebrity by {id} id. Celebrity with such id does not exist");
            }
        });

        //добавить новую знаменитость

        celebrities.MapPost("/", (IRepository repo, Celebrity celeb) =>
        {
            if (repo.AddCelebrity(celeb))
            {
                celeb.Id = repo.GetCelebrityByName(celeb.FullName);
                return celeb;
               
            }
            else
            {
                throw new AddException("Failed to add celebrity");
            }
        });

        // Добавление новой знаменитости с фото
        celebrities.MapPost("/upload", async (IRepository repo, IFormFile photo, string fullName, string nationality) =>
        {
            var config = builder.Configuration.GetSection("Celebrities");
            var photosFolder = config["PhotosFolder"];
            var maxSize = int.Parse(config["MaxPhotoSizeMB"] ?? "5") * 1024 * 1024;

            // Валидация
            if (photo == null || photo.Length == 0)
                throw new ArgumentException("Photo is required");

            if (photo.Length > maxSize)
                throw new ArgumentException($"Photo size exceeds {maxSize / 1024 / 1024}MB limit");

            // Сохранение файла
            var fileName = $"{Guid.NewGuid()}{Path.GetExtension(photo.FileName)}";
            var filePath = Path.Combine(photosFolder, fileName);

            Directory.CreateDirectory(photosFolder);
            using (var stream = new FileStream(filePath, FileMode.Create))
            {
                await photo.CopyToAsync(stream);
            }

            // Создание записи
            var celebrity = new Celebrity
            {
                FullName = fullName,
                Nationality = nationality,
                ReqPhotoPath = fileName
            };

            if (!repo.AddCelebrity(celebrity))
                throw new Exception("Failed to add celebrity");

            return Results.Created($"/api/celebrities/{celebrity.Id}", celebrity);
        });

        //изменить знаемнитость по  id

        celebrities.MapPut("/{id:int:min(1)}", (IRepository repo, int id, Celebrity newCeleb) =>
        {
            if(repo.UpdateCelebrity(id, newCeleb))
            {
                newCeleb.Id = repo.GetCelebrityByName(newCeleb.FullName);
                return newCeleb;
            }
            else
            {
                throw new UpdateException($"Failed to update celebrity with id {id}. Something went wrong");
            }
        });

        //получить файл фотографии по имени файла (fname)

        celebrities.MapGet("/photo/{fname}", async (IRepository repo, string fname) =>
        {
            var photoFoler = configuration.GetSection("Celebrities").GetSection("PhotosFolder").Value;
            var photoPath = Path.Combine(photoFoler, fname);
            if (!File.Exists(photoPath))
            {
                throw new FileNotFoundException($"Photo file was not found by requirement({fname})");
            }
            else
            {
                try
                {
                    var bytes = await File.ReadAllBytesAsync(photoPath);
                    string contentType = GetContentTypeByExtension(Path.GetExtension(photoPath));
                    return Results.File(bytes, contentType);//Возвращаем файл как HTTP-ответ с правильным типом содержимого
                }
                catch(Exception ex)
                {
                    Console.WriteLine(ex.Message);
                    return Results.Problem(ex.Message);
                }
            }

        });
        //////Событияяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяя

        var lifeEvents = app.MapGroup("/api/lifeEvents");

        //все события

        lifeEvents.MapGet("/", (IRepository repo) => repo.GetAllLifeEvents());

        //события по  id

        lifeEvents.MapGet("/{id:int:min(1)}", (IRepository repo, int id) =>
        {
            LifeEvent?lifeEvent = repo.GetLifeEventById(id);
            if (lifeEvent != null)
            {
                return lifeEvent;
            }
            else
            {
                throw new GetByIdException($"Failed to get LifeEvent by {id} id");
            }
        });

        //все события по id знаменитости

        lifeEvents.MapGet("/Celebrities/{id:int:min(1)}", (IRepository repo, int id) =>
        {
            List<LifeEvent>lifeEventList = repo.GetLifeEventsByCelebrityId(id);
            if (lifeEventList.Count == 0)
            {
                throw new GetByIdException("Failed to get life events by celebritie's id. Something went wrong");
            }
            else
            {
                return lifeEventList;
            }
        });

        //удалить события по id


        lifeEvents.MapDelete("/{id:int:min(1)}", (IRepository repo, int id) =>
        {
            if (repo.DeleteLifeEvent(id))
            {
                return $"Life Event with id {id} was deleted";
            }
            else
            {
                throw new DeleteByIdException($"Failed to delete life event by id {id} such id does not exist");
            }
        });

        //добавить новое событие

        lifeEvents.MapPost("/",(IRepository repo,LifeEvent LifeEvent) =>
        {
            if (repo.AddLifeEvent(LifeEvent))
            {
                return LifeEvent;
            }
            else
            {
                throw new AddException("Failed to add life event. Something went wrong");
            }
        });

        //изменить событие по  id

        lifeEvents.MapPut("/{id:int:min(1)}", (IRepository repo, int id, LifeEvent newEvent) =>
        {
            if(repo.UpdateLifeEvent(id, newEvent))
            {
                return newEvent;
            }
            else
            {
                throw new UpdateException($"Failed to update celebrity with id {id}. Something went wrong");
            }
        });


        app.Map("/Errors", (HttpContext ctx) =>
        {
            Exception? ex = ctx.Features.Get<IExceptionHandlerFeature>()?.Error;
            IResult rc = Results.Problem(detail: ex.Message, instance: app.Environment.EnvironmentName, title: "ASPA006", statusCode: 500);
            if (ex != null)
            {
                if(ex is AddException )
                {
                    rc = Results.NotFound(ex.Message);
                }
                if(ex is DeleteByIdException)
                {
                    rc= Results.NotFound(ex.Message);
                }
                if(ex is UpdateException)
                {
                    rc = Results.NotFound(ex.Message);
                }
                if(ex is GetByIdException)
                {
                    rc = Results.NotFound(ex.Message);
                }
            }
            return rc;
        });

        app.Run();
    }


    static string GetContentTypeByExtension(string extension)
    {
        return extension.ToLower() switch
        {
            ".jpg" => "image/jpeg",
            ".jpeg" => "image/jpeg",
            ".png" => "image/png",
            ".gif" => "image/gif",
            ".bmp" => "image/bmp",
            _ => "application/octet-stream",
        };
    }
}