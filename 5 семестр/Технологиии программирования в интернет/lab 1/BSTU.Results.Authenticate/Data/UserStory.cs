using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BSTU.Results.Authenticate.Data
{
    public class User
    {
        public string Login { get; set; } = String.Empty;
        public string Password { get; set; } = String.Empty;
        public string Role { get; set; } = String.Empty;
    }
    public static class UserStore
    {
        public static List<User> Users = new()
        {
            new User { Login = "reader", Password = "reader123", Role = "READER" },
            new User { Login = "writer", Password = "writer123", Role = "WRITER" }
        };

        public static User? FindUser(string login, string password)
        {
            return Users.FirstOrDefault(u => u.Login == login && u.Password == password);
        }
    }
}