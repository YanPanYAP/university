using System;
using System.Collections.Generic;
using System.Data.SQLite;
using System.Linq;
using System.Security.Cryptography;
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
    /// Логика взаимодействия для ChangePasswordPage.xaml
    /// </summary>
    public partial class ChangePasswordPage : Page
    {
        public ChangePasswordPage()
        {
            InitializeComponent();
        }

        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            NavigateToPage("AccountPage.xaml");
        }

        public void NavigateToPage(string pageUri)
        {
            ChangePasswodContent.Visibility = Visibility.Collapsed;
            ChangePasswodFrame.Visibility = Visibility.Visible;
            ChangePasswodFrame.Navigate(new Uri(pageUri, UriKind.Relative));
        }

        private void ChangePasswordButton_Click(object sender, RoutedEventArgs e)
        {
            string oldPass = OldPasswordBox.Password.Trim();
            string newPass = NewPasswordBox.Password.Trim();
            string confirmPass = ConfirmPasswordBox.Password.Trim();

            OldPasswordBox.ToolTip = null;
            OldPasswordBox.BorderBrush = (Brush)FindResource("HintBorderBrush");

            NewPasswordBox.ToolTip = null;
            NewPasswordBox.BorderBrush = (Brush)FindResource("HintBorderBrush");

            ConfirmPasswordBox.ToolTip = null;
            ConfirmPasswordBox.BorderBrush = (Brush)FindResource("HintBorderBrush");

            Regex passRegex = new Regex(@"^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[\W_]).{8,}$");

            if (!passRegex.IsMatch(newPass))
            {
                NewPasswordBox.ToolTip = "Пароль должен содержать минимум 8 символов, включая заглавную букву, строчную букву, цифру и спецсимвол.";
                NewPasswordBox.BorderBrush = Brushes.DarkRed;
                return;
            }

            if (newPass != confirmPass)
            {
                ConfirmPasswordBox.ToolTip = "Новый пароль и подтверждение не совпадают.";
                ConfirmPasswordBox.BorderBrush = Brushes.DarkRed;
                return;
            }

            if (Session.CurrentUser == null)
            {
                MessageBox.Show("Пользователь не авторизован.");
                return;
            }

            string hashedOldPass = HashPassword(oldPass);
            if (hashedOldPass != Session.CurrentUser.Pass)
            {
                OldPasswordBox.ToolTip = "Старый пароль введён неверно.";
                OldPasswordBox.BorderBrush = Brushes.DarkRed;
                return;
            }

            string hashedNewPass = HashPassword(newPass);

            try
            {
                using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                {
                    connection.Open();
                    string updateQuery = "UPDATE Users SET pass = @newPass WHERE id = @id";
                    using (var command = new SQLiteCommand(updateQuery, connection))
                    {
                        command.Parameters.AddWithValue("@newPass", hashedNewPass);
                        command.Parameters.AddWithValue("@id", Session.CurrentUser.id);
                        command.ExecuteNonQuery();
                    }
                }

                Session.CurrentUser.Pass = hashedNewPass;
                NavigateToPage("AccountPage.xaml");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при смене пароля: {ex.Message}");
            }
        }

        public static string HashPassword(string password)
        {
            using (SHA256 sha256 = SHA256.Create())
            {
                byte[] bytes = Encoding.UTF8.GetBytes(password);
                byte[] hash = sha256.ComputeHash(bytes);
                return Convert.ToBase64String(hash);
            }
        }



    }
}
