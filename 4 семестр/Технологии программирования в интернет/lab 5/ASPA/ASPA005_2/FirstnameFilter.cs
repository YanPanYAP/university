//using dal004;
//using microsoft.aspnetcore.http;
//using microsoft.aspnetcore.http.httpresults;
//using system.io;

//public class firstnamefilter : iendpointfilter
//{
//    public static irepository repository;

//    public async valuetask<object?> invokeasync(endpointfilterinvocationcontext context, endpointfilterdelegate next)
//    {
//        var celebrity = context.getargument<celebrity>(0);
//        if (string.isnullorwhitespace(celebrity.firstname) || celebrity.firstname.length < 2)
//        {
//            return results.conflict("firstname is wrong");
//        }

//        var existingcelebrities = repository.getallcelebrities();
//        if (existingcelebrities.any(c => c.firstname.equals(celebrity.firstname, stringcomparison.ordinalignorecase)))
//        {
//            return results.conflict("firstname is doubled");

//        }

//        return await next(context);
//    }
//}