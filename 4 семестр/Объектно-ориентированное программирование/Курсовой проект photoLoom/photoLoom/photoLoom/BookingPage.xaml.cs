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

namespace photoLoom
{
    /// <summary>
    /// Логика взаимодействия для BookingPage.xaml
    /// </summary>
    public partial class BookingPage : Page
    {
        public BookingPage()
        {
            InitializeComponent();
            LoadAllOrders();
        }
        private void LoadAllOrders()
        {
            BookingPanel.Children.Clear(); 

            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();

                string query = @"
            SELECT o.id, o.user_id, o.studio_id, s.name, o.rent_date, o.time_from, o.time_to, o.total_price, o.status
            FROM StudioRentOrders o
            JOIN Studios s ON o.studio_id = s.id";

                using (var command = new SQLiteCommand(query, connection))
                using (var reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        int orderId = reader.GetInt32(0);
                        int userId = reader.GetInt32(1);
                        string studioName = reader.GetString(3);
                        string rentDate = reader.GetString(4);
                        string timeFrom = reader.GetString(5);
                        string timeTo = reader.GetString(6);
                        int price = reader.GetInt32(7);
                        string status = reader.GetString(8);

                        var orderPanel = new StackPanel
                        {
                            Orientation = Orientation.Horizontal,
                            Margin = new Thickness(0, 5, 0, 5)
                        };

                        orderPanel.Children.Add(CreateTextBlock($"№ {orderId}", 80));
                        orderPanel.Children.Add(CreateTextBlock($"User: {userId}", 80));
                        orderPanel.Children.Add(CreateTextBlock(studioName, 120));
                        orderPanel.Children.Add(CreateTextBlock(rentDate, 100));
                        orderPanel.Children.Add(CreateTextBlock(timeFrom, 70));
                        orderPanel.Children.Add(CreateTextBlock(timeTo, 70));
                        orderPanel.Children.Add(CreateTextBlock($"{price} BYN", 80));
                        orderPanel.Children.Add(CreateTextBlock(status, 100));

                        var deleteButton = new Button
                        {
                            Content = $"{Application.Current.Resources["Delete"]}",
                            Tag = new Tuple<int, int>(orderId, userId)
                        };
                        deleteButton.Style = (Style)Application.Current.FindResource("FlatNavButtonStyle");
                        deleteButton.Click += DeleteAnyOrder_Click;

                        orderPanel.Children.Add(deleteButton);

                        BookingPanel.Children.Add(orderPanel);
                    }
                }
            }
        }
        private TextBlock CreateTextBlock(string text, double width)
        {
            return new TextBlock
            {
                Text = text,
                Width = width,
                VerticalAlignment = VerticalAlignment.Center,
                Foreground = Brushes.White,
                Margin = new Thickness(5, 0, 5, 0)
            };
        }

        private void DeleteAnyOrder_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (sender is Button btn && btn.Tag is Tuple<int, int> data)
                {
                    int orderId = data.Item1;
                    int userId = data.Item2;

                    using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                    {
                        connection.Open();

                        string selectQuery = @"
                    SELECT id, user_id, studio_id, rent_date, time_from, time_to, total_price
                    FROM StudioRentOrders
                    WHERE id = @OrderId AND user_id = @UserId";

                        using (var selectCommand = new SQLiteCommand(selectQuery, connection))
                        {
                            selectCommand.Parameters.AddWithValue("@OrderId", orderId);
                            selectCommand.Parameters.AddWithValue("@UserId", userId);

                            using (var reader = selectCommand.ExecuteReader())
                            {
                                if (reader.Read())
                                {
                                    int studioId = reader.GetInt32(2);
                                    string rentDate = reader.GetString(3);
                                    string timeFrom = reader.GetString(4);
                                    string timeTo = reader.GetString(5);
                                    int totalPrice = reader.GetInt32(6);

                                    reader.Close();

                                    
                                    string studioName = "";
                                    string studioQuery = "SELECT name FROM Studios WHERE id = @StudioId";
                                    using (var studioCmd = new SQLiteCommand(studioQuery, connection))
                                    {
                                        studioCmd.Parameters.AddWithValue("@StudioId", studioId);
                                        studioName = (string)studioCmd.ExecuteScalar() ?? "студия";
                                    }

                                    string insertArchiveQuery = @"
                                INSERT INTO ArchivedStudioRentOrders 
                                (id, user_id, studio_id, rent_date, time_from, time_to, total_price, status)
                                VALUES (@Id, @UserId, @StudioId, @RentDate, @TimeFrom, @TimeTo, @TotalPrice, 'cancelled')";

                                    using (var insertCommand = new SQLiteCommand(insertArchiveQuery, connection))
                                    {
                                        insertCommand.Parameters.AddWithValue("@Id", orderId);
                                        insertCommand.Parameters.AddWithValue("@UserId", userId);
                                        insertCommand.Parameters.AddWithValue("@StudioId", studioId);
                                        insertCommand.Parameters.AddWithValue("@RentDate", rentDate);
                                        insertCommand.Parameters.AddWithValue("@TimeFrom", timeFrom);
                                        insertCommand.Parameters.AddWithValue("@TimeTo", timeTo);
                                        insertCommand.Parameters.AddWithValue("@TotalPrice", totalPrice);
                                        insertCommand.ExecuteNonQuery();
                                    }

                                    string deleteQuery = "DELETE FROM StudioRentOrders WHERE id = @OrderId AND user_id = @UserId";
                                    using (var deleteCommand = new SQLiteCommand(deleteQuery, connection))
                                    {
                                        deleteCommand.Parameters.AddWithValue("@OrderId", orderId);
                                        deleteCommand.Parameters.AddWithValue("@UserId", userId);
                                        deleteCommand.ExecuteNonQuery();
                                    }

                                    
                                    string messageEn = $"Your booking for studio '{studioName}' on {rentDate} from {timeFrom} to {timeTo} has been cancelled by administrator.";
                                    string messageRu = $"Ваша бронь студии '{studioName}' на {rentDate} с {timeFrom} до {timeTo} была отменена администратором.";

                                    InsertNotification(userId, messageEn, messageRu);
                                }
                            }
                        }
                    }

                    LoadAllOrders(); 
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при удалении заказа:\n{ex.Message}", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
        private void Button_Create_Click(object sender, RoutedEventArgs e)
        {
            bool hasError = false;

            // Сброс стилей
            ResetTextBoxStyle(UserId);
            ResetTextBoxStyle(RuText);
            ResetTextBoxStyle(EnText);

            // Проверка ID
            if (!int.TryParse(UserId.Text, out int userId))
            {
                SetError(UserId, "ID должен содержать только цифры");
                hasError = true;
            }
            else if (!UserExists(userId))
            {
                SetError(UserId, "Пользователь с таким ID не найден");
                hasError = true;
            }

            // Проверка русского текста
            string ruText = RuText.Text.Trim();
            if (string.IsNullOrWhiteSpace(ruText))
            {
                SetError(RuText, "Поле обязательно для заполнения");
                hasError = true;
            }
            else if (ruText.Length > 200 || !Regex.IsMatch(ruText, @"^[а-яёА-ЯЁ0-9\s.,!?-]+$"))
            {
                SetError(RuText, "Разрешены только русские буквы и до 200 символов");
                hasError = true;
            }

            // Проверка английского текста
            string enText = EnText.Text.Trim();
            if (string.IsNullOrWhiteSpace(enText))
            {
                SetError(EnText, "Поле обязательно для заполнения");
                hasError = true;
            }
            else if (enText.Length > 200 || !Regex.IsMatch(enText, @"^[a-zA-Z0-9\s.,!?-]+$"))
            {
                SetError(EnText, "Разрешены только английские буквы и до 200 символов");
                hasError = true;
            }

            if (hasError)
                return;

            // Отправка
            string sendDateTime = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");

            try
            {
                using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                {
                    connection.Open();

                    string insertQuery = @"
                INSERT INTO Notifications (user_id, message_en, message_ru, send_datetime)
                VALUES (@UserId, @MessageEn, @MessageRu, @SendDateTime)";

                    using (var command = new SQLiteCommand(insertQuery, connection))
                    {
                        command.Parameters.AddWithValue("@UserId", userId);
                        command.Parameters.AddWithValue("@MessageEn", enText);
                        command.Parameters.AddWithValue("@MessageRu", ruText);
                        command.Parameters.AddWithValue("@SendDateTime", sendDateTime);

                        command.ExecuteNonQuery();
                    }
                }

                MessageBox.Show("Уведомление успешно отправлено.", "Успех", MessageBoxButton.OK, MessageBoxImage.Information);

                UserId.Clear();
                RuText.Clear();
                EnText.Clear();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при отправке уведомления:\n{ex.Message}", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void SetError(TextBox textBox, string message)
        {
            textBox.ToolTip = message;
            textBox.BorderBrush = Brushes.DarkRed;
        }

        private void ResetTextBoxStyle(TextBox textBox)
        {
            textBox.ToolTip = null;
            textBox.BorderBrush = (Brush)FindResource("HintBorderBrush");
        }

        private bool UserExists(int userId)
        {
            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();
                string query = "SELECT COUNT(*) FROM Users WHERE id = @id";

                using (var command = new SQLiteCommand(query, connection))
                {
                    command.Parameters.AddWithValue("@id", userId);
                    long count = (long)command.ExecuteScalar();
                    return count > 0;
                }
            }
        }

        private void UserId_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9]+"); 
            e.Handled = regex.IsMatch(e.Text);
        }

        private void RuText_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex(@"[^а-яёА-ЯЁ\s.,!?-]"); 
            e.Handled = regex.IsMatch(e.Text);
        }

        private void EnText_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex(@"[^a-zA-Z\s.,!?-]"); 
            e.Handled = regex.IsMatch(e.Text);
        }

        private void InsertNotification(int userId, string messageEn, string messageRu)
        {
            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();

                string insertNotifQuery = @"
            INSERT INTO Notifications (user_id, message_en, message_ru, send_datetime) 
            VALUES (@UserId, @MessageEn, @MessageRu, @SendDateTime)";

                using (var command = new SQLiteCommand(insertNotifQuery, connection))
                {
                    command.Parameters.AddWithValue("@UserId", userId);
                    command.Parameters.AddWithValue("@MessageEn", messageEn);
                    command.Parameters.AddWithValue("@MessageRu", messageRu);
                    command.Parameters.AddWithValue("@SendDateTime", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));

                    command.ExecuteNonQuery();
                }
            }
        }


        private void TimetableButton(object sender, RoutedEventArgs e)
        {

            NavigateToPage("TimetablePage.xaml");
        }

        private void StatusButton(object sender, RoutedEventArgs e)
        {
            NavigateToPage("StatusPage.xaml");
        }

        private void ReviewButton(object sender, RoutedEventArgs e)
        {
            NavigateToPage("ReviewsPage.xaml");
        }

        private void StudiosButton(object sender, RoutedEventArgs e)
        {
            NavigateToPage("AdminPage.xaml");
        }

        private void RentButton(object sender, RoutedEventArgs e)
        {
            ShowMainContent();
        }

        private void NavigateToPage(string pageUri)
        {
            BookingPageContent.Visibility = Visibility.Collapsed;
            BookingPageFrame.Visibility = Visibility.Visible;
            BookingPageFrame.Navigate(new Uri(pageUri, UriKind.Relative));
        }

        private void ShowMainContent()
        {
            BookingPageFrame.Visibility = Visibility.Collapsed;
            BookingPageContent.Visibility = Visibility.Visible;
            BookingPageFrame.Content = null;
        }

        private void ReturnToMainContent()
        {
            BookingPageFrame.Visibility = Visibility.Collapsed;
            BookingPageContent.Visibility = Visibility.Visible;
            BookingPageFrame.Content = null;
        }
        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            if (Application.Current.MainWindow is Views.MainWindow mainWindow)
            {
                mainWindow.ReturnToMainContent();
                Session.CurrentUser = null;
            }
        }
    }
}
