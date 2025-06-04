using System;
using System.Data.SQLite;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace photoLoom
{
    public partial class ChangeEmailPage : Page
    {
        public ChangeEmailPage()
        {
            InitializeComponent();
        }

        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            NavigateToPage("AccountPage.xaml");
        }

        public void NavigateToPage(string pageUri)
        {
            ChangeEmailContent.Visibility = Visibility.Collapsed;
            ChangeEmailFrame.Visibility = Visibility.Visible;
            ChangeEmailFrame.Navigate(new Uri(pageUri, UriKind.Relative));
        }

        private void ChangeEmailButton_Click(object sender, RoutedEventArgs e)
        {
            string newEmail = NewEmailBox.Text.Trim();

            Regex emailRegex = new Regex(@"^[a-zA-Z]{3,}@([a-zA-Z0-9\-]+\.)+[a-zA-Z]{2,}$");

            if (!emailRegex.IsMatch(newEmail))
            {
                NewEmailBox.ToolTip = "Некорректный формат email";
                NewEmailBox.BorderBrush = Brushes.DarkRed;
                return;
            }

            try
            {
                using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                {
                    connection.Open();
                    string updateQuery = "UPDATE Users SET email = @newEmail WHERE id = @id";
                    using (var command = new SQLiteCommand(updateQuery, connection))
                    {
                        command.Parameters.AddWithValue("@newEmail", newEmail);
                        command.Parameters.AddWithValue("@id", Session.CurrentUser.id);
                        command.ExecuteNonQuery();
                    }
                }

                Session.CurrentUser.Email = newEmail;
                NavigateToPage("AccountPage.xaml");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при смене email: {ex.Message}");
            }
        }

    }
}
