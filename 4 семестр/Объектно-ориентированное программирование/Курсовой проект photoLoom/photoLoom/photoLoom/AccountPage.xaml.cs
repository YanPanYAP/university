using System;
using System.Collections.Generic;
using System.Data.SQLite;
using System.Linq;
using System.Text;
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
using static photoLoom.NewReviewPage;

namespace photoLoom
{
    /// <summary>
    /// Логика взаимодействия для AccountPage.xaml
    /// </summary>
    public partial class AccountPage : Page
    {
        public AccountPage()
        {
            InitializeComponent();
            DataContext = Session.CurrentUser;
            MoveOldOrdersToArchive();
            LoadUserOrders();
            LoadUserArchivedOrders();
        }

        private void LanguageButton_Click(object sender, RoutedEventArgs e)
        {
            var newLang = LocalizationManager.CurrentLanguage == AppLanguage.English
                ? AppLanguage.Russian
                : AppLanguage.English;

            LocalizationManager.ChangeLanguage(newLang);
            LoadUserOrders();
            LoadUserArchivedOrders();
        }

        private void ThemeButton_Click(object sender, RoutedEventArgs e)
        {
            var newTheme = ThemeManager.CurrentTheme == AppTheme.Dark
                ? AppTheme.Light
                : AppTheme.Dark;

            ThemeManager.ChangeTheme(newTheme);
        }

        private void HomeButton_Click(object sender, RoutedEventArgs e)
        {
            if (Application.Current.MainWindow is Views.MainWindow mainWindow)
            {
                mainWindow.ReturnToMainContent();
            }
        }

        private void StudiosButton_Click(object sender, RoutedEventArgs e)
        {
            NavigateToPage("StudiosPage.xaml");
        }

        private void PhotographersButton_Click(object sender, RoutedEventArgs e)
        {
            NavigateToPage("PhotographersPage.xaml");
        }

        private void AccountButton_Click(object sender, RoutedEventArgs e)
        {
            
            ShowMainContent();
        }

        private void NavigateToPage(string pageUri)
        {
            AccountContent.Visibility = Visibility.Collapsed;
            AccountFrame.Visibility = Visibility.Visible;
            AccountFrame.Navigate(new Uri(pageUri, UriKind.Relative));
        }

        private void ShowMainContent()
        {
            AccountFrame.Visibility = Visibility.Collapsed;
            AccountContent.Visibility = Visibility.Visible;
            AccountFrame.Content = null;
        }

        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            if (Application.Current.MainWindow is Views.MainWindow mainWindow)
            {
                mainWindow.ReturnToMainContent();
                Session.CurrentUser = null;
            }
        }
        
        private void ChangePasswordButton_Click(object sender, RoutedEventArgs e)
        {
            NavigateToPage("ChangePasswordPage.xaml");
        }

        private void MailButton_Click(object sender, RoutedEventArgs e)
        {
            NavigateToPage("NotificationsUser.xaml");
        }

        private void ChangeEmailButton_Click(object sender, RoutedEventArgs e)
        {
            NavigateToPage("ChangeEmailPage.xaml");
        }

        private void ChangeNameButton_Click(object sender, RoutedEventArgs e)
        {
            NavigateToPage("ChangeNamePage.xaml");
        }

        private void DeleteAccountButton_Click(object sender, RoutedEventArgs e)
        {
            if (HasPendingOrders(Session.CurrentUser.id))
            {
                MessageBox.Show("У вас есть активные заказы. Удаление аккаунта невозможно.", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            var result = MessageBox.Show("Вы уверены, что хотите удалить аккаунт?", "Подтверждение", MessageBoxButton.YesNo);
            if (result == MessageBoxResult.Yes)
            {
                DeleteUserAccount();
            }
        }

        private bool HasPendingOrders(int userId)
        {
            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();

                string query = @"
            SELECT COUNT(*) 
            FROM StudioRentOrders 
            WHERE user_id = @UserId AND status = 'pending'";

                using (var command = new SQLiteCommand(query, connection))
                {
                    command.Parameters.AddWithValue("@UserId", userId);
                    long count = (long)command.ExecuteScalar();
                    return count > 0;
                }
            }
        }

        private void DeleteUserAccount()
        {
            try
            {
                using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                {
                    connection.Open();
                    string deleteQuery = "DELETE FROM Users WHERE id = @id";
                    using (var command = new SQLiteCommand(deleteQuery, connection))
                    {
                        command.Parameters.AddWithValue("@id", Session.CurrentUser.id);
                        command.ExecuteNonQuery();
                    }
                }
                Session.CurrentUser = null;

                if (Application.Current.MainWindow is Views.MainWindow mainWindow)
                {
                    mainWindow.NavigateToPage("AuthPage.xaml");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Произошла ошибка: {ex.Message}");
            }
        }


        private void Image_Drop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                string[] filePaths = (string[])e.Data.GetData(DataFormats.FileDrop);
                if (filePaths.Length > 0)
                {
                    string imagePath = filePaths[0];
                    SaveImagePath(imagePath);
                }
            }
        }

        private void Image_MouseDown(object sender, MouseButtonEventArgs e)
        {
            var dialog = new Microsoft.Win32.OpenFileDialog
            {
                Filter = "Image files (*.png;*.jpg;*.jpeg)|*.png;*.jpg;*.jpeg"
            };

            if (dialog.ShowDialog() == true)
            {
                string selectedFile = dialog.FileName;
                SaveImagePath(selectedFile);
            }
        }
        private void SaveImagePath(string path)
        {
            try
            {
                using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                {
                    connection.Open();
                    string updateQuery = "UPDATE Users SET photoPath = @photoPath WHERE id = @id";
                    using (var command = new SQLiteCommand(updateQuery, connection))
                    {
                        command.Parameters.AddWithValue("@photoPath", path);
                        command.Parameters.AddWithValue("@id", Session.CurrentUser.id);
                        command.ExecuteNonQuery();
                    }
                }

                Session.CurrentUser.PhotoPath = path;
                DataContext = null;
                DataContext = Session.CurrentUser;
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка обновления фото: {ex.Message}");
            }
        }

        private void LoadUserOrders()
        {
            AccaunRented.Children.Clear();

            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();

                string query = @"
            SELECT o.id, o.studio_id, s.name, o.rent_date, o.time_from, o.time_to, o.total_price, o.status
            FROM StudioRentOrders o
            JOIN Studios s ON o.studio_id = s.id
            WHERE o.user_id = @UserId";

                using (var command = new SQLiteCommand(query, connection))
                {
                    command.Parameters.AddWithValue("@UserId", Session.CurrentUser.id);

                    using (var reader = command.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            int orderId = reader.GetInt32(0);
                            string studioName = reader.GetString(2);
                            string rentDate = reader.GetString(3);
                            string timeFrom = reader.GetString(4);
                            string timeTo = reader.GetString(5);
                            int price = reader.GetInt32(6);
                            string status = reader.GetString(7);

                            var orderPanel = new StackPanel
                            {
                                Orientation = Orientation.Horizontal,
                                Margin = new Thickness(0, 5, 0, 5)
                            };

                            orderPanel.Children.Add(CreateTextBlock($"№ {orderId}", 80));
                            orderPanel.Children.Add(CreateTextBlock(studioName, 120));
                            orderPanel.Children.Add(CreateTextBlock(rentDate, 100));
                            orderPanel.Children.Add(CreateTextBlock(timeFrom, 70));
                            orderPanel.Children.Add(CreateTextBlock(timeTo, 70));
                            orderPanel.Children.Add(CreateTextBlock($"{price} BYN", 80));
                            orderPanel.Children.Add(CreateTextBlock(LocalizeStatus(status), 100));

                            var deleteButton = new Button
                            {
                                Content = $"{Application.Current.Resources["Delete"]}",
                                Tag = orderId
                            };
                            deleteButton.Style = (Style)Application.Current.FindResource("FlatNavButtonStyle");
                            deleteButton.Click += DeleteOrder_Click;

                            orderPanel.Children.Add(deleteButton);

                            AccaunRented.Children.Add(orderPanel);
                        }
                    }
                }
            }
        }

        private void LoadUserArchivedOrders()
        {
            AccaunRentedOld.Children.Clear();

            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();

                
                var reviewedOrderIds = new HashSet<int>();
                string reviewedQuery = @"
            SELECT archived_order_id 
            FROM Reviews 
            WHERE user_id = @UserId";

                using (var reviewedCmd = new SQLiteCommand(reviewedQuery, connection))
                {
                    reviewedCmd.Parameters.AddWithValue("@UserId", Session.CurrentUser.id);
                    using (var reader = reviewedCmd.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            reviewedOrderIds.Add(reader.GetInt32(0));
                        }
                    }
                }

                
                string query = @"
            SELECT o.id, o.studio_id, s.name, o.rent_date, o.time_from, o.time_to, o.total_price, o.status
            FROM ArchivedStudioRentOrders o
            JOIN Studios s ON o.studio_id = s.id
            WHERE o.user_id = @UserId";

                using (var command = new SQLiteCommand(query, connection))
                {
                    command.Parameters.AddWithValue("@UserId", Session.CurrentUser.id);

                    using (var reader = command.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            int orderId = reader.GetInt32(0);
                            string studioName = reader.GetString(2);
                            string rentDate = reader.GetString(3);
                            string timeFrom = reader.GetString(4);
                            string timeTo = reader.GetString(5);
                            int price = reader.GetInt32(6);
                            string status = reader.GetString(7);

                            var orderPanel = new StackPanel
                            {
                                Orientation = Orientation.Horizontal,
                                Margin = new Thickness(0, 5, 0, 5)
                            };

                            orderPanel.Children.Add(CreateTextBlock($"№ {orderId}", 80));
                            orderPanel.Children.Add(CreateTextBlock(studioName, 120));
                            orderPanel.Children.Add(CreateTextBlock(rentDate, 100));
                            orderPanel.Children.Add(CreateTextBlock(timeFrom, 70));
                            orderPanel.Children.Add(CreateTextBlock(timeTo, 70));
                            orderPanel.Children.Add(CreateTextBlock($"{price} BYN", 80));
                            orderPanel.Children.Add(CreateTextBlock(LocalizeStatus(status), 100));

                            
                            if (status.Trim().ToLower() == "approved" && !reviewedOrderIds.Contains(orderId))
                            {
                                var reviewButton = new Button
                                {
                                    Content = $"{Application.Current.Resources["Review"]}",
                                    Tag = orderId
                                };
                                reviewButton.Style = (Style)Application.Current.FindResource("FlatNavButtonStyle");
                                reviewButton.Click += LeaveReview_Click;

                                orderPanel.Children.Add(reviewButton);
                            }

                            AccaunRentedOld.Children.Add(orderPanel);
                        }
                    }
                }
            }
        }

        private string LocalizeStatus(string status)
        {
            switch (status.ToLower())
            {
                case "pending":
                    return LocalizationManager.CurrentLanguage == AppLanguage.Russian ? "Ожидает" : "Pending";
                case "cancelled":
                    return LocalizationManager.CurrentLanguage == AppLanguage.Russian ? "Отменён" : "Cancelled";
                case "processing":
                    return LocalizationManager.CurrentLanguage == AppLanguage.Russian ? "Завершён" : "Processing";
                case "approved":
                    return LocalizationManager.CurrentLanguage == AppLanguage.Russian ? "Завершён" : "Approved";
                default:
                    return status; 
            }
        }


        private void LeaveReview_Click(object sender, RoutedEventArgs e)
        {
            if (sender is Button btn && btn.Tag is int orderId)
            {

                ReviewContext.ArchivedOrderId = orderId;
                NavigateToPage("NewReviewPage.xaml");
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

        private void DeleteOrder_Click(object sender, RoutedEventArgs e)
        {
            if (sender is Button btn && btn.Tag is int orderId)
            {

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
                        selectCommand.Parameters.AddWithValue("@UserId", Session.CurrentUser.id);

                        using (var reader = selectCommand.ExecuteReader())
                        {
                            if (reader.Read())
                            {
                                int userId = reader.GetInt32(1);
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
                                    deleteCommand.Parameters.AddWithValue("@UserId", Session.CurrentUser.id);
                                    deleteCommand.ExecuteNonQuery();
                                }

                                
                                string messageEn = $"Your booking for studio '{studioName}' on {rentDate} from {timeFrom} to {timeTo} has been cancelled.";
                                string messageRu = $"Ваша бронь студии '{studioName}' на {rentDate} с {timeFrom} до {timeTo} была отменена.";

                                
                                InsertNotification(userId, messageEn, messageRu);
                            }
                        }
                    }
                }

                LoadUserOrders();
                LoadUserArchivedOrders();
            }
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


        private void MoveOldOrdersToArchive()
        {
            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();

                string moveQuery = @"
INSERT INTO ArchivedStudioRentOrders (id, user_id, studio_id, rent_date, time_from, time_to, total_price, status)
SELECT id, user_id, studio_id, rent_date, time_from, time_to, total_price, 'approved'
FROM StudioRentOrders
WHERE rent_date < date('now');

DELETE FROM StudioRentOrders
WHERE rent_date < date('now');";

                using (var command = new SQLiteCommand(moveQuery, connection))
                {
                    command.ExecuteNonQuery();
                }
            }
            LoadUserOrders();
            LoadUserArchivedOrders();
        }


    }
}
