using System;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Data.SQLite;
using System.Security.Cryptography;

namespace photoLoom
{
    /// <summary>
    /// Логика взаимодействия для AuthPage.xaml
    /// </summary>
    public partial class AuthPage : Page
    {
        public AuthPage()
        {
            InitializeComponent();

            try
            {
                using (SQLiteConnection conn = new SQLiteConnection(AppContext.ConnectionString))
                {
                    conn.Open();
                    conn.Close();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка подключения: {ex.Message}");
            }
        }

        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            if (Application.Current.MainWindow is Views.MainWindow mainWindow)
            {
                mainWindow.ReturnToMainContent();
            }
        }

        private void Button_Reg_Click(object sender, RoutedEventArgs e)
        {
            string login = textBoxLogin.Text.Trim();
            string pass = textBoxPass.Password.Trim();
            string email = textBoxEmail.Text.ToLower().Trim();
            string photoPath = "C:\\Mac\\Home\\Documents\\fit\\ООП\\photoLoom\\photoLoom\\resources\\images\\photoLoom.png";

            Regex loginRegex = new Regex(@"^[a-zA-Zа-яА-Я0-9_]{3,10}$");
            Regex emailRegex = new Regex(@"^[a-zA-Z]{3,}@([a-zA-Z0-9\-]+\.)+[a-zA-Z]{2,}$");
            Regex passRegex = new Regex(@"^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[\W_]).{8,}$");

            bool isValid = true;

            textBoxLogin.ToolTip = null;
            textBoxLogin.BorderBrush = (Brush)FindResource("HintBorderBrush");

            textBoxPass.ToolTip = null;
            textBoxPass.BorderBrush = (Brush)FindResource("HintBorderBrush");

            textBoxEmail.ToolTip = null;
            textBoxEmail.BorderBrush = (Brush)FindResource("HintBorderBrush");

            
            if (!loginRegex.IsMatch(login))
            {
                textBoxLogin.ToolTip = "Только буквы, цифры и _ (3-10 символов)";
                textBoxLogin.BorderBrush = Brushes.DarkRed;
                isValid = false;
            }

            
            if (!passRegex.IsMatch(pass))
            {
                textBoxPass.ToolTip = "Пароль от 8 символов, минимум 1 заглавная, строчная, цифра и спецсимвол";
                textBoxPass.BorderBrush = Brushes.DarkRed;
                isValid = false;
            }

            
            if (!emailRegex.IsMatch(email))
            {
                textBoxEmail.ToolTip = "Некорректный формат email";
                textBoxEmail.BorderBrush = Brushes.DarkRed;
                isValid = false;
            }

            if (!isValid)
                return;

            
            string hashedPass = HashPassword(pass);

            try
            {
                using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                {
                    connection.Open();

                    

                    string adminQuery = "SELECT * FROM Admins WHERE login = @login AND email = @email AND pass = @pass";
                    using (var adminCommand = new SQLiteCommand(adminQuery, connection))
                    {
                        adminCommand.Parameters.AddWithValue("@login", login);
                        adminCommand.Parameters.AddWithValue("@email", email);
                        adminCommand.Parameters.AddWithValue("@pass", hashedPass);

                        using (var adminReader = adminCommand.ExecuteReader())
                        {
                            if (adminReader.Read())
                            {
                                Session.CurrentUser = new User
                                {
                                    Login = login,
                                    Pass = hashedPass,
                                    Email = email,
                                };

                                if (Application.Current.MainWindow is Views.MainWindow mainWindow)
                                {
                                    mainWindow.NavigateToPage("AdminPage.xaml");
                                }
                                return;
                            }
                        }
                    }

                    
                    string userQuery = "SELECT * FROM Users WHERE login = @login AND email = @email AND pass = @pass";
                    using (var userCommand = new SQLiteCommand(userQuery, connection))
                    {
                        userCommand.Parameters.AddWithValue("@login", login);
                        userCommand.Parameters.AddWithValue("@email", email);
                        userCommand.Parameters.AddWithValue("@pass", hashedPass);

                        using (var userReader = userCommand.ExecuteReader())
                        {
                            if (userReader.Read())
                            {
                                Session.CurrentUser = new User
                                {
                                    id = Convert.ToInt32(userReader["id"]),
                                    Login = userReader["login"].ToString(),
                                    Pass = userReader["pass"].ToString(),
                                    Email = userReader["email"].ToString(),
                                    PhotoPath = userReader["photoPath"].ToString()
                                };

                                if (Application.Current.MainWindow is Views.MainWindow mainWindow)
                                {
                                    mainWindow.NavigateToPage("AccountPage.xaml");
                                }
                            }
                            else
                            {
                                string checkEmailQuery = "SELECT COUNT(*) FROM Users WHERE email = @Email";
                                using (var checkEmailCmd = new SQLiteCommand(checkEmailQuery, connection))
                                {
                                    checkEmailCmd.Parameters.AddWithValue("@Email", email);
                                    long count = (long)checkEmailCmd.ExecuteScalar();

                                    if (count > 0)
                                    {
                                        textBoxEmail.ToolTip = "Этот email уже используется";
                                        textBoxEmail.BorderBrush = Brushes.DarkRed;
                                        return;
                                    }
                                }

                                string insertQuery = "INSERT INTO Users (login, pass, email, photoPath) VALUES (@login, @pass, @email, @photoPath)";
                                using (var insertCommand = new SQLiteCommand(insertQuery, connection))
                                {
                                    insertCommand.Parameters.AddWithValue("@login", login);
                                    insertCommand.Parameters.AddWithValue("@pass", hashedPass);
                                    insertCommand.Parameters.AddWithValue("@email", email);
                                    insertCommand.Parameters.AddWithValue("@photoPath", photoPath);

                                    insertCommand.ExecuteNonQuery();

                                    long newUserId = connection.LastInsertRowId;

                                    Session.CurrentUser = new User
                                    {
                                        id = (int)newUserId,
                                        Login = login,
                                        Pass = hashedPass,
                                        Email = email,
                                        PhotoPath = photoPath
                                    };

                                    if (Application.Current.MainWindow is Views.MainWindow mainWindow)
                                    {
                                        mainWindow.NavigateToPage("AccountPage.xaml");
                                    }
                                }
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка работы с базой данных: {ex.Message}");
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
