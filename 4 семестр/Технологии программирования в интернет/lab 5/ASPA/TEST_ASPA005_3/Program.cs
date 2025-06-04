using System.Net.Http.Json;
using System.Text.Json;
using static System.Net.Mime.MediaTypeNames;
using static System.Net.WebRequestMethods;

public class Test
{
    public class Answer<T>
    {
        public T? x { get; set; } = default(T);
        public T? y { get; set; } = default(T);
        public string? message { get; set; } = null;
    }

    public static string OK = "OK ", NOK = "NOK";
    HttpClient client = new HttpClient();

    public async Task ExecuteGET<T>(string path, Func<T?, T?, int, string> result)
    {
        await resultPRINT<T>("GET", path, await this.client.GetAsync(path), result);
    }

    public async Task ExecutePOST<T>(string path, Func<T?, T?, int, string> result)
    {
        await resultPRINT<T>("POST", path, await this.client.PostAsync(path, null), result);
    }

    public async Task ExecutePUT<T>(string path, Func<T?, T?, int, string> result)
    {
        await resultPRINT<T>("PUT", path, await this.client.PutAsync(path, null), result);
    }

    public async Task ExecuteDELETE<T>(string path, Func<T?, T?, int, string> result)
    {
        await resultPRINT<T>("DELETE", path, await this.client.DeleteAsync(path), result);
    }

    async Task resultPRINT<T>(string method, string path, HttpResponseMessage rm, Func<T?, T?, int, string> result)
    {
        int status = (int)rm.StatusCode;
        try
        {
            Answer<T>? answer = await rm.Content.ReadFromJsonAsync<Answer<T>>() ?? default(Answer<T>);
            string r = result(default(T), default(T), status);
            T? x = default(T), y = default(T);
            if (answer != null) r = result(x = answer.x, y = answer.y, status);
            Console.WriteLine($" {r}: {method} {path}, status = {status}, x = {x}, y = {y}, m = {answer?.message}");
        }
        catch (JsonException ex)
        {
            string r = result(default(T), default(T), status);
            Console.WriteLine($" {r}: {method} {path}, status = {status}, x = {null}, y = {null}, m = {ex.Message}");
        }
    }
}

internal class Program
{
    static async Task Main(string[] args)
    {
        Test test = new Test();

        string CurrentUri = "https://localhost:7220";

        Console.WriteLine("--- A ---");
        await test.ExecuteGET<int?>($"{CurrentUri}/A/3", (int? x, int? y, int status) => (x == 3 && y == null && status == 200) ? Test.OK : Test.NOK);
        await test.ExecuteGET<int?>($"{CurrentUri}/A/-3", (int? x, int? y, int status) => (x == -3 && y == null && status == 200) ? Test.OK : Test.NOK);
        await test.ExecuteGET<int?>($"{CurrentUri}/A/118", (int? x, int? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecutePOST<int?>($"{CurrentUri}/A/5", (int? x, int? y, int status) => (x == 5 && y == null && status == 200) ? Test.OK : Test.NOK);
        await test.ExecutePOST<int?>($"{CurrentUri}/A/-5", (int? x, int? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecutePOST<int?>($"{CurrentUri}/A/118", (int? x, int? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecutePUT<int?>($"{CurrentUri}/A/2/3", (int? x, int? y, int status) => (x == 2 && y == 3 && status == 200) ? Test.OK : Test.NOK);
        await test.ExecutePUT<int?>($"{CurrentUri}/A/0/3", (int? x, int? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecutePUT<int?>($"{CurrentUri}/A/25/-3", (int? x, int? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecutePUT<int?>($"{CurrentUri}/A/0/-3", (int? x, int? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecuteDELETE<int?>($"{CurrentUri}/A/1-99", (int? x, int? y, int status) => (x == 1 && y == 99 && status == 200) ? Test.OK : Test.NOK);
        await test.ExecuteDELETE<int?>($"{CurrentUri}/A/99-1", (int? x, int? y, int status) => (x == 99 && y == 1 && status == 200) ? Test.OK : Test.NOK);
        await test.ExecuteDELETE<int?>($"{CurrentUri}/A/-1-25", (int? x, int? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecuteDELETE<int?>($"{CurrentUri}/A/-1--25", (int? x, int? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecuteDELETE<int?>($"{CurrentUri}/A/25-101", (int? x, int? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);

        Console.WriteLine("--- B ---");

        await test.ExecuteGET<float?>($"{CurrentUri}/B/2.5", (float? x, float? y, int status) => (x == 2.5f && y == null && status == 200) ? Test.OK : Test.NOK);
        await test.ExecuteGET<float?>($"{CurrentUri}/B/2", (float? x, float? y, int status) => (x == 2.0f && y == null && status == 200) ? Test.OK : Test.NOK);
        await test.ExecuteGET<float?>($"{CurrentUri}/B/2X", (float? x, float? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecutePOST<float?>($"{CurrentUri}/B/2.5/3.2", (float? x, float? y, int status) => (x == 2.5f && y == 3.2f && status == 200) ? Test.OK : Test.NOK);
        await test.ExecuteDELETE<float?>($"{CurrentUri}/B/2.5-3.2", (float? x, float? y, int status) => (x == 2.5f && y == 3.2f && status == 200) ? Test.OK : Test.NOK);

        Console.WriteLine("--- C ---");

        await test.ExecuteGET<bool?>($"{CurrentUri}/C/2.5", (bool? x, bool? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecuteGET<bool?>($"{CurrentUri}/C/true", (bool? x, bool? y, int status) => (x == true && y == null && status == 200) ? Test.OK : Test.NOK);
        await test.ExecutePOST<bool?>($"{CurrentUri}/C/true,false", (bool? x, bool? y, int status) => (x == true && y == false && status == 200) ? Test.OK : Test.NOK);
        await test.ExecuteDELETE<bool?>($"{CurrentUri}/C/true,false", (bool? x, bool? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);

        Console.WriteLine("--- D ---");

        await test.ExecuteGET<DateTime?>($"{CurrentUri}/D/2025-02-25", (DateTime? x, DateTime? y, int status) => (x == new DateTime(2025, 02, 25) && y == null && status == 200) ? Test.OK : Test.NOK);
        await test.ExecuteGET<DateTime?>($"{CurrentUri}/D/2025-02-29", (DateTime? x, DateTime? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecuteGET<DateTime?>($"{CurrentUri}/D/2024-02-29", (DateTime? x, DateTime? y, int status) => (x == new DateTime(2024, 02, 29) && y == null && status == 200) ? Test.OK : Test.NOK);
        await test.ExecuteGET<DateTime?>($"{CurrentUri}/D/2025-02-25T19:25", (DateTime? x, DateTime? y, int status) => (x == new DateTime(2025, 02, 25, 19, 25, 0) && y == null && status == 200) ? Test.OK : Test.NOK);
        await test.ExecutePOST<DateTime?>($"{CurrentUri}/D/2025-02-25|2025-03-25", (DateTime? x, DateTime? y, int status) => (x == new DateTime(2025, 02, 25) && y == new DateTime(2025, 03, 25) && status == 200) ? Test.OK : Test.NOK);
        await test.ExecutePUT<DateTime?>($"{CurrentUri}/D/2025-02-25T19:25", (DateTime? x, DateTime? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);


        Console.WriteLine("--- E ---");

        await test.ExecuteGET<string?>($"{CurrentUri}/E/12-bis", (string? x, string? y, int status) => (x == "bis" && y == null && status == 200) ? Test.OK : Test.NOK);
        await test.ExecuteGET<string?>($"{CurrentUri}/E/11-bis", (string? x, string? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecuteGET<string?>($"{CurrentUri}/E/12-777", (string? x, string? y, int status) => (x == "777" && y == null && status == 200) ? Test.OK : Test.NOK);
        await test.ExecuteGET<string?>($"{CurrentUri}/E/12-", (string? x, string? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecutePUT<string?>($"{CurrentUri}/E/abcd", (string? x, string? y, int status) => (x == "abcd" && y == null && status == 200) ? Test.OK : Test.NOK);
        await test.ExecutePUT<string?>($"{CurrentUri}/E/abcd123", (string? x, string? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecutePUT<string?>($"{CurrentUri}/E/a", (string? x, string? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecutePUT<string?>($"{CurrentUri}/E/123456", (string? x, string? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecutePUT<string?>($"{CurrentUri}/E/aabbccddeeffgghh", (string? x, string? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);

        Console.WriteLine("-- /F ---------------------------------------------------");

        await test.ExecuteGET<string?>($"{CurrentUri}/F/smw@belstu.by", (string? x, string? y, int status) => (x == "smw@belstu.by" && y == null && status == 200) ? Test.OK : Test.NOK);
        await test.ExecuteGET<string?>($"{CurrentUri}/F/xxx@yyy.by", (string? x, string? y, int status) => (x == "xxx@yyy.by" && y == null && status == 200) ? Test.OK : Test.NOK);
        await test.ExecuteGET<string?>($"{CurrentUri}/F/xxx@yyy.ru", (string? x, string? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecuteGET<string?>($"{CurrentUri}/F/xxxyyy.by", (string? x, string? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);
        await test.ExecuteGET<string?>($"{CurrentUri}/F/xxx@yyy", (string? x, string? y, int status) => (x == null && y == null && status == 404) ? Test.OK : Test.NOK);



    }

}