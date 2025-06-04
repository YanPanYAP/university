using DAL004;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Http.HttpResults;
using System.IO;

public class IdFilter : IEndpointFilter
{
    public static IRepository repository;

    public async ValueTask<object?> InvokeAsync(EndpointFilterInvocationContext context, EndpointFilterDelegate next)
    {
        int id = context.GetArgument<int>(0);

        if (repository.GetCelebrityById(id) == null)
        {
            return Results.Conflict($"Celebrity by id={id} Not Found");
        }

        return await next(context);
    }
}