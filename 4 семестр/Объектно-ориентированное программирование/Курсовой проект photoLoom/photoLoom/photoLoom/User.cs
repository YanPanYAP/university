using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace photoLoom
{
    public static class Session
    {
        public static User CurrentUser { get; set; }
    }
    public class User
    {
        public int id { get; set; }
        private string login, pass, email, photoPath;

        public string Login
        {
            get { return login; }
            set { login = value; }
        }

        public string Pass
        {
            get { return pass; }
            set { pass = value; }
        }

        public string Email
        {
            get { return email; }
            set { email = value; }
        }

        public string PhotoPath
        {
            get { return photoPath; }
            set { photoPath = value; }
        }

        public User() { }

        public User(string login, string pass, string email, string photoPath)
        {
            this.login = login;
            this.pass = pass;
            this.email = email;
            this.photoPath = photoPath;
        }

    }
}
