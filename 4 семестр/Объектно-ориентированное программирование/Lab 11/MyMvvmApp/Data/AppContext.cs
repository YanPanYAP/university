using Microsoft.Data.Sqlite;
using System.Data.SQLite;

namespace photoLoom.Data
{
    public class AppContext
    {
        public static string DbPath { get; } =
            @"C:\Mac\Home\Documents\fit\ООП\MyMvvmApp\MyMvvmApp\Data\photoLoomDB.db";

        public static string ConnectionString => $"Data Source={DbPath};Version=3;";
    }

}
