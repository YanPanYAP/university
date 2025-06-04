using DAL004;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Http.HttpResults;
using System.IO;
using CustomExceptions;

public class PhotoExistFilter : IEndpointFilter
{
    public static IRepository repository;

    public async ValueTask<object?> InvokeAsync(EndpointFilterInvocationContext context, EndpointFilterDelegate next)
    {
        var celebrity = context.GetArgument<Celebrity>(0);
        string dirPath = "C:\\4sem\\TPI\\lab5\\ASPA\\Celebrities";
        string basePath = Path.Combine(Directory.GetCurrentDirectory(), dirPath);
        string photoFilePath = Path.Combine(basePath, Path.GetFileName(celebrity.PhotoPath));

        if (!File.Exists(photoFilePath))
        {
            context.HttpContext.Response.Headers.Add("X-Celebrity", Path.GetFileName(celebrity.PhotoPath));
        }

        return await next(context);
    }
}