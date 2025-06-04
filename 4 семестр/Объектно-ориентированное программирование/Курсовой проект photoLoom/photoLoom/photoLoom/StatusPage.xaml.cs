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
    /// Логика взаимодействия для StatusPage.xaml
    /// </summary>
    public partial class StatusPage : Page
    {
        public StatusPage()
        {
            InitializeComponent();
            LoadStatistics();
        }

        private void LoadStatistics()
        {
            StatusPanel.Children.Clear();

            using (var connection = new SQLiteConnection(AppContext.ConnectionString))
            {
                connection.Open();

                
                string query = @"
            SELECT 
                s.id,
                s.name,
                IFNULL(sc.click_count, 0) AS clicks,
                IFNULL(COUNT(a.id), 0) AS rents,
                IFNULL(SUM(a.total_price), 0) AS revenue
            FROM Studios s
            LEFT JOIN StudioClicks sc ON s.id = sc.studio_id
            LEFT JOIN ArchivedStudioRentOrders a ON s.id = a.studio_id AND a.status = 'approved'
            GROUP BY s.id, s.name, sc.click_count
            ORDER BY s.name;
        ";

                using (var cmd = new SQLiteCommand(query, connection))
                using (var reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        string studioName = reader["name"].ToString();
                        int clicks = Convert.ToInt32(reader["clicks"]);
                        int rents = Convert.ToInt32(reader["rents"]);
                        int revenue = Convert.ToInt32(reader["revenue"]);
                        StackPanel row = new StackPanel
                        {
                            Orientation = Orientation.Horizontal,
                            Margin = new Thickness(0, 5, 0, 5)
                        };

                        
                        row.Children.Add(new TextBlock
                        {
                            Text = studioName,
                            Width = 170,
                            FontWeight = FontWeights.Bold,
                            Margin = new Thickness(5),
                            Foreground = Brushes.White,
                            FontSize = 16
                        });

                        
                        row.Children.Add(new TextBlock
                        {
                            Text = clicks.ToString(),
                            Width = 120,
                            Margin = new Thickness(5),
                            Foreground = Brushes.White,
                            TextAlignment = TextAlignment.Center,
                            FontSize = 16
                        });

                        
                        row.Children.Add(new TextBlock
                        {
                            Text = rents.ToString(),
                            Width = 80,
                            Margin = new Thickness(5),
                            Foreground = Brushes.White,
                            TextAlignment = TextAlignment.Center,
                            FontSize = 16
                        });

                        
                        row.Children.Add(new TextBlock
                        {
                            Text = revenue.ToString(),
                            Width = 80,
                            Margin = new Thickness(5),
                            Foreground = Brushes.White,
                            TextAlignment = TextAlignment.Center,
                            FontSize = 16
                        });

                        
                        StatusPanel.Children.Add(row);
                    }
                }
            }
        }


        private void TimetableButton(object sender, RoutedEventArgs e)
        {

            NavigateToPage("TimetablePage.xaml");
        }

        private void StatusButton(object sender, RoutedEventArgs e)
        {

            ShowMainContent();
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
            NavigateToPage("BookingPage.xaml");
        }

        private void NavigateToPage(string pageUri)
        {
            StatusPageContent.Visibility = Visibility.Collapsed;
            StatusPageFrame.Visibility = Visibility.Visible;
            StatusPageFrame.Navigate(new Uri(pageUri, UriKind.Relative));
        }

        private void ShowMainContent()
        {
            StatusPageFrame.Visibility = Visibility.Collapsed;
            StatusPageContent.Visibility = Visibility.Visible;
            StatusPageFrame.Content = null;
        }

        private void ReturnToMainContent()
        {
            StatusPageFrame.Visibility = Visibility.Collapsed;
            StatusPageContent.Visibility = Visibility.Visible;
            StatusPageFrame.Content = null;
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
