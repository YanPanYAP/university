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

namespace photoLoom
{
    /// <summary>
    /// Логика взаимодействия для NotificationsUser.xaml
    /// </summary>
    public partial class NotificationsUser : Page
    {
        public NotificationsUser()
        {
            InitializeComponent();
            LoadUserNotifications();
        }

        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            NavigateToPage("AccountPage.xaml");
        }

        private void LoadUserNotifications()
        {
            NotificationPanel.Children.Clear();

            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();

                string query = @"
    SELECT id, message_en, message_ru, send_datetime 
    FROM Notifications 
    WHERE user_id = @UserId 
    ORDER BY send_datetime DESC";

                using (var command = new SQLiteCommand(query, connection))
                {
                    command.Parameters.AddWithValue("@UserId", Session.CurrentUser.id);

                    using (var reader = command.ExecuteReader())
                    {
                        bool hasNotifications = false;

                        while (reader.Read())
                        {
                            hasNotifications = true;

                            int notifId = reader.GetInt32(0);
                            string messageEn = reader.GetString(1);
                            string messageRu = reader.GetString(2);
                            string dateTime = reader.GetString(3);

                            string message = LocalizationManager.CurrentLanguage == AppLanguage.Russian ? messageRu : messageEn;

                            var notifPanel = new StackPanel
                            {
                                Orientation = Orientation.Horizontal,
                                Margin = new Thickness(0, 5, 0, 5)
                            };

                            notifPanel.Children.Add(CreateTextBlock(dateTime, 130));
                            notifPanel.Children.Add(CreateTextBlock(message, 400));

                            var deleteButton = new Button
                            {
                                Content = $"{Application.Current.Resources["Delete"]}",
                                Tag = notifId
                            };
                            deleteButton.Style = (Style)Application.Current.FindResource("FlatNavButtonStyle");
                            deleteButton.Click += DeleteNotification_Click;

                            notifPanel.Children.Add(deleteButton);

                            NotificationPanel.Children.Add(notifPanel);
                        }

                        if (!hasNotifications)
                        {
                            string emptyMessage = LocalizationManager.CurrentLanguage == AppLanguage.Russian
                                ? "Уведомлений нет"
                                : "No notifications";

                            NotificationPanel.Children.Add(new TextBlock
                            {
                                Text = emptyMessage,
                                Foreground = Brushes.Gray,
                                FontSize = 14.0,
                                FontStyle = FontStyles.Italic,
                                Margin = new Thickness(10)
                            });
                        }
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
                Foreground = Brushes.White,
                FontSize = 12,
                FontWeight = FontWeights.Bold,
                Margin = new Thickness(5, 0, 5, 0),
                TextWrapping = TextWrapping.Wrap,
                VerticalAlignment = VerticalAlignment.Center
            };
        }

        private void DeleteNotification_Click(object sender, RoutedEventArgs e)
        {
            if (sender is Button button && button.Tag is int notifId)
            {
                using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                {
                    connection.Open();

                    string deleteQuery = "DELETE FROM Notifications WHERE id = @id";
                    using (var command = new SQLiteCommand(deleteQuery, connection))
                    {
                        command.Parameters.AddWithValue("@id", notifId);
                        command.ExecuteNonQuery();
                    }
                }

                LoadUserNotifications();
            }
        }


        public void NavigateToPage(string pageUri)
        {
            NotificationsUserContent.Visibility = Visibility.Collapsed;
            NotificationsUserFrame.Visibility = Visibility.Visible;
            NotificationsUserFrame.Navigate(new Uri(pageUri, UriKind.Relative));
        }
    }
}
