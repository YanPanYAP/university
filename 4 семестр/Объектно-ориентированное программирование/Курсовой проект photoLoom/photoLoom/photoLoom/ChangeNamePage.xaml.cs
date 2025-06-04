using System;
using System.Collections.Generic;
using System.Data.SQLite;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using photoLoom;

namespace photoLoom
{
    /// <summary>
    /// Логика взаимодействия для ChangeNamePage.xaml
    /// </summary>
    public partial class ChangeNamePage : Page
    {
        public ChangeNamePage()
        {
            InitializeComponent();
        }

        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            NavigateToPage("AccountPage.xaml");
        }

        public void NavigateToPage(string pageUri)
        {
            ChangeNameContent.Visibility = Visibility.Collapsed;
            ChangeNameFrame.Visibility = Visibility.Visible;
            ChangeNameFrame.Navigate(new Uri(pageUri, UriKind.Relative));
        }

        private void ChangeNameButton_Click(object sender, RoutedEventArgs e)
        {
            string newName = NewNameBox.Text.Trim();

            Regex nameRegex = new Regex(@"^[a-zA-Zа-яА-Я0-9_]{3,10}$");

            if (!nameRegex.IsMatch(newName))
            {
                NewNameBox.ToolTip = "Некорректный формат email";
                NewNameBox.BorderBrush = Brushes.DarkRed;
                return;
            }

            try
            {
                using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                {
                    connection.Open();
                    string updateQuery = "UPDATE Users SET login = @newName WHERE id = @id";
                    using (var command = new SQLiteCommand(updateQuery, connection))
                    {
                        command.Parameters.AddWithValue("@newName", newName);
                        command.Parameters.AddWithValue("@id", Session.CurrentUser.id);
                        command.ExecuteNonQuery();
                    }
                }

                Session.CurrentUser.Login = newName;
                NavigateToPage("AccountPage.xaml");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при смене имени: {ex.Message}");
            }
        }

    }
}
