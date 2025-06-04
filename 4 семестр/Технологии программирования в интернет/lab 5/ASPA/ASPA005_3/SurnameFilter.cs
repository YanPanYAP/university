using DAL004;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Http.HttpResults;
using System.IO;
using CustomExceptions;

public class SurnameFilter : IEndpointFilter
{
    public static IRepository repository;

    public async ValueTask<object?> InvokeAsync(EndpointFilterInvocationContext context, EndpointFilterDelegate next)
    {
        var celebrity = context.GetArgument<Celebrity>(0);
        if (celebrity == null)
        {
            throw new CelebryIsNull("Celebity is null");
        }

        if (string.IsNullOrWhiteSpace(celebrity.Surname) || celebrity.Surname.Length < 2)
        {
            throw new SurnameIsNull("Surname is null");
        }

        var existingCelebrities = repository.GetAllCelebrities();
        if (existingCelebrities.Any(c => c.Surname.Equals(celebrity.Surname, StringComparison.OrdinalIgnoreCase)))
        {
            throw new SurnameEquals("Surname is Equals");

        }

        return await next(context);
    }
}