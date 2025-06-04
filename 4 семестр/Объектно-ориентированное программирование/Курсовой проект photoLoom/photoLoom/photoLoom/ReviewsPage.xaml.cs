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
    /// Логика взаимодействия для ReviewsPage.xaml
    /// </summary>
    public partial class ReviewsPage : Page
    {
        public ReviewsPage()
        {
            InitializeComponent();
            LoadReviews(); 
        }

        private void LoadReviews()
        {
            try
            {
                ReviewsPanel.Children.Clear();

                using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                {
                    connection.Open();

                    string query = @"
            SELECT r.id, r.archived_order_id, u.login, r.review_text
            FROM Reviews r
            JOIN Users u ON r.user_id = u.id";

                    using (var command = new SQLiteCommand(query, connection))
                    using (var reader = command.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            int reviewId = reader.GetInt32(0);
                            int orderId = reader.GetInt32(1);
                            string userName = reader.GetString(2);
                            string reviewText = reader.GetString(3);

                            var reviewPanel = new StackPanel
                            {
                                Orientation = Orientation.Horizontal,
                                Margin = new Thickness(0, 5, 0, 5)
                            };

                            reviewPanel.Children.Add(CreateTextBlock($"№ {reviewId}", 60));
                            reviewPanel.Children.Add(CreateTextBlock($"Order: {orderId}", 80));
                            reviewPanel.Children.Add(CreateTextBlock(userName, 100));
                            reviewPanel.Children.Add(CreateTextBlock(reviewText, 400));

                            var deleteButton = new Button
                            {
                                Content = $"{Application.Current.Resources["Delete"]}",
                                Tag = reviewId
                            };
                            deleteButton.Style = (Style)Application.Current.FindResource("FlatNavButtonStyle");
                            deleteButton.Click += DeleteReview_Click;

                            reviewPanel.Children.Add(deleteButton);

                            ReviewsPanel.Children.Add(reviewPanel);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при загрузке отзывов:\n{ex.Message}", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Error);
            }

        }

        private TextBlock CreateTextBlock(string text, double width)
        {
            return new TextBlock
            {
                Text = text,
                Width = width,
                Foreground = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#DDDDDD")), // мягкий белый
                FontSize = 14,
                FontWeight = FontWeights.SemiBold,
                VerticalAlignment = VerticalAlignment.Center,
                TextWrapping = TextWrapping.Wrap,
                TextTrimming = TextTrimming.CharacterEllipsis,
                Background = Brushes.Transparent,
                Margin = new Thickness(5, 2, 5, 2),
                Padding = new Thickness(2, 0, 2, 0)
            };
        }


        private void DeleteReview_Click(object sender, RoutedEventArgs e)
        {
            if (sender is Button button && button.Tag is int reviewId)
            {
                using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                {
                    connection.Open();

                    string query = "DELETE FROM Reviews WHERE id = @ReviewId";
                    using (var command = new SQLiteCommand(query, connection))
                    {
                        command.Parameters.AddWithValue("@ReviewId", reviewId);
                        command.ExecuteNonQuery();
                    }
                }

                LoadReviews(); 
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
            ShowMainContent();
        }

        private void StudiosButton(object sender, RoutedEventArgs e)
        {
            NavigateToPage("AdminPage.xaml");
        }

        private void RentButton(object sender, RoutedEventArgs e)
        {
            NavigateToPage("BookingPage.xaml");
        }

        private void NavigateToPage(string pageUri)
        {
            ReviewsPageContent.Visibility = Visibility.Collapsed;
            ReviewsPageFrame.Visibility = Visibility.Visible;
            ReviewsPageFrame.Navigate(new Uri(pageUri, UriKind.Relative));
        }

        private void ShowMainContent()
        {
            ReviewsPageFrame.Visibility = Visibility.Collapsed;
            ReviewsPageContent.Visibility = Visibility.Visible;
            ReviewsPageFrame.Content = null;
        }

        private void ReturnToMainContent()
        {
            ReviewsPageFrame.Visibility = Visibility.Collapsed;
            ReviewsPageContent.Visibility = Visibility.Visible;
            ReviewsPageFrame.Content = null;
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
