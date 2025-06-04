using System;
using System.Collections.Generic;
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
using static photoLoom.Views.MainWindow;
using photoLoom;
using System.Web.SessionState;
using System.Data.SQLite;
using System.Text.RegularExpressions;

namespace photoLoom
{
    /// <summary>
    /// Логика взаимодействия для StudiosPage.xaml
    /// </summary>
    public partial class StudiosPage : Page
    {
        private double? currentMinPrice;
        private double? currentMaxPrice;
        private double? minSquareFilter;
        private double? maxSquareFilter;
        private Func<List<Studio>, List<Studio>> currentSort;
        private List<Studio> allStudios;
        private List<Studio> filteredStudios;
        private Button clickedButton;
        private string nameFilter = "";

        public StudiosPage()
        {
            InitializeComponent();
            allStudios = LoadStudios();
            filteredStudios = new List<Studio>(allStudios);
            clickedButton = GridModeButton;
            ApplyFiltersAndDisplay();
        }

        private void LanguageButton_Click(object sender, RoutedEventArgs e)
        {
            var newLang = LocalizationManager.CurrentLanguage == AppLanguage.English
                ? AppLanguage.Russian
                : AppLanguage.English;

            LocalizationManager.ChangeLanguage(newLang);
            allStudios = LoadStudios();
            filteredStudios = new List<Studio>(allStudios);

            if (NoResultsTextBlock.Visibility != Visibility.Visible)
            {
                if (clickedButton == GridModeButton)
                    DisplayGridMode(filteredStudios);
                else if (clickedButton == ListModeButton)
                    DisplayListMode(filteredStudios);
            }
        }

        private void ThemeButton_Click(object sender, RoutedEventArgs e)
        {
            var newTheme = ThemeManager.CurrentTheme == AppTheme.Dark
                ? AppTheme.Light
                : AppTheme.Dark;

            ThemeManager.ChangeTheme(newTheme);
        }

        private void SwitchViewMode(object sender, RoutedEventArgs e)
        {
            GridModeButton.Tag = "null";
            ListModeButton.Tag = "null";

            clickedButton = (Button)sender;
            clickedButton.Tag = "Active";

            ApplyFiltersAndDisplay();
        }

        private void FilterViewMode(object sender, RoutedEventArgs e)
        {
            if (StudiosFilterButton.Tag?.ToString() == "Active")
            {
                StudiosFilterButton.Tag = "null";
                FilterPopup.IsOpen = false;

                MinPriceTextBox.Text = "";
                MaxPriceTextBox.Text = "";
                MinSquareTextBox.Text = "";
                MaxSquareTextBox.Text = "";
                currentMinPrice = null;
                currentMaxPrice = null;
                minSquareFilter = null;
                maxSquareFilter = null;
                nameFilter = "";
                currentSort = null;

                ApplyFiltersAndDisplay();
            }
            else
            {
                StudiosFilterButton.Tag = "Active";
                FilterPopup.IsOpen = true;
            }
        }
        private void ApplyPriceAsc(object sender, RoutedEventArgs e) => ApplyFilter(s => s.OrderBy(st => st.price).ToList());
        private void ApplyPriceDesc(object sender, RoutedEventArgs e) => ApplyFilter(s => s.OrderByDescending(st => st.price).ToList());
        private void ApplySquareAsc(object sender, RoutedEventArgs e) => ApplyFilter(s => s.OrderBy(st => st.square).ToList());
        private void ApplySquareDesc(object sender, RoutedEventArgs e) => ApplyFilter(s => s.OrderByDescending(st => st.square).ToList());

        private void ApplyFilter(Func<List<Studio>, List<Studio>> sortFunc)
        {
            currentSort = sortFunc;
            StudiosFilterButton.Tag = "Active";
            FilterPopup.IsOpen = false;

            var studios = ApplyPriceFilter(allStudios);
            studios = ApplySquareFilter(studios);

            filteredStudios = currentSort != null ? currentSort(studios) : studios;

            NoResultsTextBlock.Visibility = filteredStudios.Count == 0 ? Visibility.Visible : Visibility.Collapsed;

            if (clickedButton == GridModeButton)
                DisplayGridMode(filteredStudios);
            else
                DisplayListMode(filteredStudios);
        }

        private List<Studio> ApplyPriceFilter(List<Studio> source)
        {
            double min = currentMinPrice ?? 0;
            double max = currentMaxPrice ?? double.MaxValue;
            return source.Where(s => s.price >= min && s.price <= max).ToList();
        }

        private List<Studio> ApplySquareFilter(List<Studio> source)
        {
            double min = minSquareFilter ?? 0;
            double max = maxSquareFilter ?? double.MaxValue;
            return source.Where(s => s.square >= min && s.square <= max).ToList();
        }

        private void ApplyFiltersAndDisplay()
        {
            IEnumerable<Studio> studios = allStudios;

            if (currentMinPrice.HasValue || currentMaxPrice.HasValue)
                studios = ApplyPriceFilter(studios.ToList());

            if (minSquareFilter.HasValue || maxSquareFilter.HasValue)
                studios = ApplySquareFilter(studios.ToList());

            if (!string.IsNullOrWhiteSpace(nameFilter))
                studios = studios.Where(s => s.name?.ToLower().Contains(nameFilter.ToLower()) == true);

            var result = studios.ToList();

            if (currentSort != null)
                result = currentSort(result);

            filteredStudios = result;

            NoResultsTextBlock.Visibility = filteredStudios.Count == 0 ? Visibility.Visible : Visibility.Collapsed;

            if (clickedButton == ListModeButton)
                DisplayListMode(filteredStudios);
            else
                DisplayGridMode(filteredStudios);
        }

        private void ApplyPriceRange(object sender, RoutedEventArgs e)
        {
            if (!double.TryParse(MinPriceTextBox.Text, out double min))
                min = 0;
            if (!double.TryParse(MaxPriceTextBox.Text, out double max))
                max = double.MaxValue;

            if (min > max)
                return;

            currentMinPrice = min;
            currentMaxPrice = max;

            ApplyFiltersAndDisplay();
        }

        private void ApplySquareRange(object sender, RoutedEventArgs e)
        {
            if (!double.TryParse(MinSquareTextBox.Text, out double min))
                min = 0;
            if (!double.TryParse(MaxSquareTextBox.Text, out double max))
                max = double.MaxValue;

            if (min > max)
                return;

            minSquareFilter = min;
            maxSquareFilter = max;

            ApplyFiltersAndDisplay();
        }





        private void NumberOnly_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            e.Handled = !Regex.IsMatch(e.Text, "^[0-9]+$");
        }


        private void DisplayGridMode(List<Studio> studios)
        {
            var wrapPanel = new WrapPanel
            {
                HorizontalAlignment = HorizontalAlignment.Stretch,
                VerticalAlignment = VerticalAlignment.Top,
                ItemWidth = 300,
                ItemHeight = 300,
                Margin = new Thickness(10)
            };

            foreach (var studio in studios)
            {
                var studioCard = new StackPanel
                {
                    Margin = new Thickness(10),
                    HorizontalAlignment = HorizontalAlignment.Left
                };

                var image = new Image
                {
                    Source = new BitmapImage(new Uri(studio.photoPath)),
                    Height = 200,
                    Margin = new Thickness(0, 0, 0, 10)
                };

                var topGrid = new Grid { Margin = new Thickness(0, 0, 0, 5) };
                topGrid.ColumnDefinitions.Add(new ColumnDefinition());
                topGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = GridLength.Auto });

                var nameText = new TextBlock
                {
                    Text = studio.name,
                    Foreground = Brushes.White,
                    FontSize = 17,
                    FontWeight = FontWeights.Bold,
                    VerticalAlignment = VerticalAlignment.Center,
                    HorizontalAlignment = HorizontalAlignment.Left
                };

                var takeButton = new Button
                {
                    Content = (string)Application.Current.Resources["TakeItButton"],
                    Style = (Style)FindResource("FlatNavButtonStyle"),
                    Padding = new Thickness(15, 8, 15, 8),
                    FontSize = 12,
                    Margin = new Thickness(10, 0, 0, 0),
                    Cursor = Cursors.Hand,
                    HorizontalAlignment = HorizontalAlignment.Right

                };
                takeButton.DataContext = studio;
                takeButton.Click += RentStudioButton_Click;
                Grid.SetColumn(nameText, 0);
                Grid.SetColumn(takeButton, 1);
                topGrid.Children.Add(nameText);
                topGrid.Children.Add(takeButton);

                var priceText = new TextBlock { HorizontalAlignment = HorizontalAlignment.Left };
                priceText.Inlines.Add(new Run((string)Application.Current.Resources["PriceLabel"]) { Foreground = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#AAAAAA")) });
                priceText.Inlines.Add(new Run($" • {studio.price} Byn") { Foreground = Brushes.White, FontWeight = FontWeights.Bold, FontSize = 12 });
                priceText.Inlines.Add(new Run("  "));
                priceText.Inlines.Add(new Run((string)Application.Current.Resources["SquareLabel"]) { Foreground = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#AAAAAA")) });
                priceText.Inlines.Add(new Run($" • {studio.square} м²") { Foreground = Brushes.White, FontWeight = FontWeights.Bold, FontSize = 12});

                studioCard.Children.Add(image);
                studioCard.Children.Add(topGrid);
                studioCard.Children.Add(priceText);

                wrapPanel.Children.Add(studioCard);
            }

            StudiosPanel.Children.Clear();
            StudiosPanel.Children.Add(wrapPanel);
        }

        private void DisplayListMode(List<Studio> studios)
        {
            var listView = new ListView
            {
                HorizontalAlignment = HorizontalAlignment.Stretch,
                VerticalAlignment = VerticalAlignment.Top,
                Margin = new Thickness(10),
                Background = Brushes.Transparent,
                BorderThickness = new Thickness(0),
                SelectionMode = SelectionMode.Single
            };

            listView.ItemContainerStyle = new Style(typeof(ListViewItem));
            listView.ItemContainerStyle.Setters.Add(new Setter(ListViewItem.BackgroundProperty, Brushes.Transparent));
            listView.ItemContainerStyle.Setters.Add(new Setter(ListViewItem.BorderBrushProperty, Brushes.Transparent));
            listView.ItemContainerStyle.Setters.Add(new Setter(ListViewItem.FocusVisualStyleProperty, null));
            listView.ItemContainerStyle.Setters.Add(new Setter(ListViewItem.FocusableProperty, false));
            listView.ItemContainerStyle.Setters.Add(new Setter(ListViewItem.IsTabStopProperty, false));
            listView.ItemContainerStyle.Setters.Add(new Setter(ListViewItem.TemplateProperty, new ControlTemplate(typeof(ListViewItem))
            {
                VisualTree = new FrameworkElementFactory(typeof(ContentPresenter))
            }));

            var itemTemplate = new DataTemplate();
            var stackPanelFactory = new FrameworkElementFactory(typeof(StackPanel));
            stackPanelFactory.SetValue(StackPanel.OrientationProperty, Orientation.Horizontal);
            stackPanelFactory.SetValue(StackPanel.MarginProperty, new Thickness(0, 10, 0, 10));

            var imageFactory = new FrameworkElementFactory(typeof(Image));
            imageFactory.SetBinding(Image.SourceProperty, new Binding("photoPath") { Converter = new StringToImageSourceConverter() });
            imageFactory.SetValue(Image.HeightProperty, 200.0);
            imageFactory.SetValue(Image.WidthProperty, 200.0);
            imageFactory.SetValue(Image.MarginProperty, new Thickness(0, 0, 20, 0));

            var textStackFactory = new FrameworkElementFactory(typeof(StackPanel));
            textStackFactory.SetValue(StackPanel.OrientationProperty, Orientation.Vertical);
            textStackFactory.SetValue(StackPanel.VerticalAlignmentProperty, VerticalAlignment.Center);
            textStackFactory.SetValue(StackPanel.WidthProperty, 200.0);

            var nameFactory = new FrameworkElementFactory(typeof(TextBlock));
            nameFactory.SetBinding(TextBlock.TextProperty, new Binding("name"));
            nameFactory.SetValue(TextBlock.ForegroundProperty, Brushes.White);
            nameFactory.SetValue(TextBlock.FontSizeProperty, 17.0);
            nameFactory.SetValue(TextBlock.FontWeightProperty, FontWeights.Bold);
            nameFactory.SetValue(TextBlock.MarginProperty, new Thickness(0, 0, 0, 5));

            var priceFactory = new FrameworkElementFactory(typeof(TextBlock));
            priceFactory.SetValue(TextBlock.MarginProperty, new Thickness(0, 0, 0, 5));
            priceFactory.SetValue(TextBlock.HorizontalAlignmentProperty, HorizontalAlignment.Left);
            priceFactory.AddHandler(FrameworkElement.LoadedEvent, new RoutedEventHandler((s, e) =>
            {
                if (s is TextBlock textBlock && textBlock.DataContext is Studio studio)
                {
                    textBlock.Inlines.Clear();
                    textBlock.Inlines.Add(new Run((string)Application.Current.Resources["PriceLabel"]) { Foreground = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#AAAAAA")) });
                    textBlock.Inlines.Add(new Run($" • {studio.price} Byn") { Foreground = Brushes.White, FontWeight = FontWeights.Bold, FontSize = 12 });
                }
            }));

            var squareFactory = new FrameworkElementFactory(typeof(TextBlock));
            squareFactory.SetValue(TextBlock.MarginProperty, new Thickness(0, 0, 0, 5));
            squareFactory.SetValue(TextBlock.HorizontalAlignmentProperty, HorizontalAlignment.Left);
            squareFactory.AddHandler(FrameworkElement.LoadedEvent, new RoutedEventHandler((s, e) =>
            {
                if (s is TextBlock textBlock && textBlock.DataContext is Studio studio)
                {
                    textBlock.Inlines.Clear();
                    textBlock.Inlines.Add(new Run((string)Application.Current.Resources["SquareLabel"]) { Foreground = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#AAAAAA")) });
                    textBlock.Inlines.Add(new Run($" • {studio.square} м²") { Foreground = Brushes.White, FontWeight = FontWeights.Bold, FontSize = 12 });
                }
            }));

            var buttonFactory = new FrameworkElementFactory(typeof(Button));
            buttonFactory.SetValue(Button.ContentProperty, (string)Application.Current.Resources["TakeItButton"]);
            buttonFactory.SetValue(Button.StyleProperty, FindResource("FlatNavButtonStyle"));
            buttonFactory.SetValue(Button.PaddingProperty, new Thickness(15, 8, 15, 8));
            buttonFactory.SetValue(Button.FontSizeProperty, 12.0);
            buttonFactory.SetValue(Button.MarginProperty, new Thickness(20, 0, 0, 0));
            buttonFactory.SetValue(Button.CursorProperty, Cursors.Hand);
            buttonFactory.SetValue(Button.VerticalAlignmentProperty, VerticalAlignment.Center);
            buttonFactory.SetValue(Button.HeightProperty, 40.0);
            buttonFactory.AddHandler(Button.ClickEvent, new RoutedEventHandler(RentStudioButton_Click));

            textStackFactory.AppendChild(nameFactory);
            textStackFactory.AppendChild(priceFactory);
            textStackFactory.AppendChild(squareFactory);

            stackPanelFactory.AppendChild(imageFactory);
            stackPanelFactory.AppendChild(textStackFactory);
            stackPanelFactory.AppendChild(buttonFactory);

            itemTemplate.VisualTree = stackPanelFactory;
            listView.ItemTemplate = itemTemplate;
            listView.ItemsSource = studios;

            StudiosPanel.Children.Clear();
            StudiosPanel.Children.Add(listView);
        }

        private void RentStudioButton_Click(object sender, RoutedEventArgs e)
        {
            if (sender is Button button && button.DataContext is Studio studio)
            {
                using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                {
                    connection.Open();

                    string checkQuery = "SELECT click_count FROM StudioClicks WHERE studio_id = @studioId";
                    using (var checkCmd = new SQLiteCommand(checkQuery, connection))
                    {
                        checkCmd.Parameters.AddWithValue("@studioId", studio.id);
                        var result = checkCmd.ExecuteScalar();

                        if (result != null)
                        {
                            string updateQuery = "UPDATE StudioClicks SET click_count = click_count + 1 WHERE studio_id = @studioId";
                            using (var updateCmd = new SQLiteCommand(updateQuery, connection))
                            {
                                updateCmd.Parameters.AddWithValue("@studioId", studio.id);
                                updateCmd.ExecuteNonQuery();
                            }
                        }
                        else
                        {
                            string insertQuery = "INSERT INTO StudioClicks (studio_id, click_count) VALUES (@studioId, 1)";
                            using (var insertCmd = new SQLiteCommand(insertQuery, connection))
                            {
                                insertCmd.Parameters.AddWithValue("@studioId", studio.id);
                                insertCmd.ExecuteNonQuery();
                            }
                        }
                    }
                }
                Studio.SelectedStudio = studio;
                NavigateToPage("RentStudioPage.xaml");
            }
        }




        public class StringToImageSourceConverter : IValueConverter
        {
            public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
            {
                try
                {
                    return new BitmapImage(new Uri(value.ToString()));
                }
                catch
                {
                    return DependencyProperty.UnsetValue;
                }
            }

            public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
            {
                throw new NotImplementedException();
            }
        }

        public class StudioDetailsConverter : IValueConverter
        {
            public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
            {
                if (value is Studio studio)
                {
                    return $"${studio.price} • {studio.square}м²";
                }
                return string.Empty;
            }

            public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
            {
                throw new NotImplementedException();
            }
        }


        private List<Studio> LoadStudios()
        {
            return new Studio().LoadStudios();
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
            ShowMainContent();
        }

        private void PhotographersButton_Click(object sender, RoutedEventArgs e)
        {
            NavigateToPage("PhotographersPage.xaml");
        }

        private void AccountButton_Click(object sender, RoutedEventArgs e)
        {
            if (Session.CurrentUser != null)
            {
                NavigateToPage("AccountPage.xaml");
            }
            else
            {
                NavigateToPage("AuthPage.xaml");
            }
        }

        private void NavigateToPage(string pageUri)
        {
            StudiosContent.Visibility = Visibility.Collapsed;
            StudiosFrame.Visibility = Visibility.Visible;
            StudiosFrame.Navigate(new Uri(pageUri, UriKind.Relative));
        }

        private void ShowMainContent()
        {
            StudiosFrame.Visibility = Visibility.Collapsed;
            StudiosContent.Visibility = Visibility.Visible;
            StudiosFrame.Content = null;
        }

        public void ReturnToMainContent()
        {
            StudiosFrame.Visibility = Visibility.Collapsed;
            StudiosContent.Visibility = Visibility.Visible;
            StudiosFrame.Content = null;
        }
    }

}
