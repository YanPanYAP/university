using Microsoft.AspNetCore.Mvc.Filters;
using Microsoft.AspNetCore.Mvc;
using System.Threading.Tasks;
using DAL_Celebrity_MSSQL; // Для Celebrity
using System.Collections.Generic;

namespace ASPA008_1.Filters
{
    public class CelebrityWikipediaLinksFilter : IActionFilter
    {

        public void OnActionExecuting(ActionExecutingContext context) //Метод до контроллера
        {

        }

        public void OnActionExecuted(ActionExecutedContext context) //Метод восле контроллера
        {
            if (context.Result is ViewResult viewResult && viewResult.Model is Celebrity celebrity)
            {
                var wikipediaLinks = new List<string>
                {
                    $"https://ru.wikipedia.org/wiki/{Uri.EscapeDataString(celebrity.FullName)}",
                };

                viewResult.ViewData["WikipediaLinks"] = wikipediaLinks;
            }
        }
    }
}