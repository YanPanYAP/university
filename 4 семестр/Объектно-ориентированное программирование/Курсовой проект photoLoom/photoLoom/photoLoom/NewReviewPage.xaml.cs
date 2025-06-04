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
    /// Логика взаимодействия для NewReviewPage.xaml
    /// </summary>
    public static class ReviewContext
    {
        public static int ArchivedOrderId { get; set; }
    }
    public partial class NewReviewPage : Page
    {
        public NewReviewPage()
        {
            InitializeComponent();

        }

        


        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            NavigateToPage("AccountPage.xaml");
        }

        public void NavigateToPage(string pageUri)
        {
            NewReviewContent.Visibility = Visibility.Collapsed;
            NewReviewFrame.Visibility = Visibility.Visible;
            NewReviewFrame.Navigate(new Uri(pageUri, UriKind.Relative));
        }

        private void SendReviewButton_Click(object sender, RoutedEventArgs e)
        {
            NewReviewBox.BorderBrush = (Brush)FindResource("HintBorderBrush");
            string reviewText = NewReviewBox.Text.Trim();
            if (string.IsNullOrWhiteSpace(reviewText))
            {
                NewReviewBox.BorderBrush = Brushes.DarkRed;
                return;
            }

            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();

                string insertQuery = @"
            INSERT INTO Reviews (user_id, archived_order_id, review_text, created_at)
            VALUES (@UserId, @ArchivedOrderId, @ReviewText, date('now'))";

                using (var command = new SQLiteCommand(insertQuery, connection))
                {
                    command.Parameters.AddWithValue("@UserId", Session.CurrentUser.id);
                    command.Parameters.AddWithValue("@ArchivedOrderId", ReviewContext.ArchivedOrderId);
                    command.Parameters.AddWithValue("@ReviewText", reviewText);
                    command.ExecuteNonQuery();
                }
            }

            NavigateToPage("AccountPage.xaml");
        }
    }
}
