using photoLoom;
using System;
using System.Collections.Generic;
using System.Data.SQLite;
using System.Linq;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace photoLoom
{
    public partial class AdminPage : Page
    {
        private List<EditableStudio> studios;

        public AdminPage()
        {
            InitializeComponent();
            UpdateStudiosList();
        }

        private void DisplayStudios(List<EditableStudio> studios)
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
            listView.ItemContainerStyle.Setters.Add(new Setter(ListViewItem.IsHitTestVisibleProperty, true));
            listView.ItemContainerStyle.Setters.Add(new Setter(ListViewItem.TemplateProperty, new ControlTemplate(typeof(ListViewItem))
            {
                VisualTree = new FrameworkElementFactory(typeof(ContentPresenter))
            }));

            var itemTemplate = new DataTemplate();
            var stackPanelFactory = new FrameworkElementFactory(typeof(StackPanel));
            stackPanelFactory.SetValue(StackPanel.OrientationProperty, Orientation.Horizontal);
            stackPanelFactory.SetValue(StackPanel.MarginProperty, new Thickness(0, 10, 0, 10));

            
            var imageFactory = new FrameworkElementFactory(typeof(Image));
            imageFactory.SetBinding(Image.SourceProperty, new Binding("photoPath") { Converter = new StudiosPage.StringToImageSourceConverter() });
            imageFactory.SetValue(Image.HeightProperty, 200.0);
            imageFactory.SetValue(Image.WidthProperty, 200.0);
            imageFactory.SetValue(Image.MarginProperty, new Thickness(0, 0, 20, 0));
            imageFactory.AddHandler(UIElement.DropEvent, new DragEventHandler(Image_Drop));
            imageFactory.AddHandler(UIElement.MouseDownEvent, new MouseButtonEventHandler(Image_MouseDown));

            
            var textStackFactory = new FrameworkElementFactory(typeof(StackPanel));
            textStackFactory.SetValue(StackPanel.OrientationProperty, Orientation.Vertical);
            textStackFactory.SetValue(StackPanel.VerticalAlignmentProperty, VerticalAlignment.Center);
            textStackFactory.SetValue(StackPanel.WidthProperty, 400.0);

            var nameBoxFactory = new FrameworkElementFactory(typeof(TextBox));
            nameBoxFactory.SetBinding(TextBox.TextProperty, new Binding("name") { Mode = BindingMode.TwoWay });
            nameBoxFactory.SetValue(TextBox.ForegroundProperty, Brushes.White);
            nameBoxFactory.SetValue(TextBox.FontSizeProperty, 17.0);
            nameBoxFactory.SetValue(TextBox.FontWeightProperty, FontWeights.Bold);
            nameBoxFactory.SetValue(TextBox.MarginProperty, new Thickness(0, 0, 0, 5));
            nameBoxFactory.SetValue(TextBox.BackgroundProperty, Brushes.Transparent);
            nameBoxFactory.SetValue(TextBox.BorderBrushProperty, Brushes.Gray);

            var nameRuBoxFactory = new FrameworkElementFactory(typeof(TextBox));
            nameRuBoxFactory.SetBinding(TextBox.TextProperty, new Binding("name_ru") { Mode = BindingMode.TwoWay });
            nameRuBoxFactory.SetValue(TextBox.ForegroundProperty, Brushes.White);
            nameRuBoxFactory.SetValue(TextBox.FontSizeProperty, 17.0);
            nameRuBoxFactory.SetValue(TextBox.FontWeightProperty, FontWeights.Bold);
            nameRuBoxFactory.SetValue(TextBox.MarginProperty, new Thickness(0, 0, 0, 5));
            nameRuBoxFactory.SetValue(TextBox.BackgroundProperty, Brushes.Transparent);
            nameRuBoxFactory.SetValue(TextBox.BorderBrushProperty, Brushes.Gray);

            var priceBoxFactory = new FrameworkElementFactory(typeof(TextBox));
            priceBoxFactory.SetBinding(TextBox.TextProperty, new Binding("price") { Mode = BindingMode.TwoWay, StringFormat = "F0" });
            priceBoxFactory.SetValue(TextBox.ForegroundProperty, Brushes.White);
            priceBoxFactory.SetValue(TextBox.FontWeightProperty, FontWeights.Bold);
            priceBoxFactory.SetValue(TextBox.FontSizeProperty, 12.0);
            priceBoxFactory.SetValue(TextBox.BackgroundProperty, Brushes.Transparent);
            priceBoxFactory.SetValue(TextBox.BorderBrushProperty, Brushes.Gray);
            priceBoxFactory.SetValue(TextBox.MarginProperty, new Thickness(0, 0, 0, 5));
            priceBoxFactory.AddHandler(TextBox.PreviewTextInputEvent, new TextCompositionEventHandler(NumberOnly_PreviewTextInput));
            priceBoxFactory.AddHandler(TextBox.TextChangedEvent, new TextChangedEventHandler(LimitLength_TextChanged));

            var squareBoxFactory = new FrameworkElementFactory(typeof(TextBox));
            squareBoxFactory.SetBinding(TextBox.TextProperty, new Binding("square") { Mode = BindingMode.TwoWay, StringFormat = "F0" });
            squareBoxFactory.SetValue(TextBox.ForegroundProperty, Brushes.White);
            squareBoxFactory.SetValue(TextBox.FontWeightProperty, FontWeights.Bold);
            squareBoxFactory.SetValue(TextBox.FontSizeProperty, 12.0);
            squareBoxFactory.SetValue(TextBox.BackgroundProperty, Brushes.Transparent);
            squareBoxFactory.SetValue(TextBox.BorderBrushProperty, Brushes.Gray);
            squareBoxFactory.SetValue(TextBox.MarginProperty, new Thickness(0, 0, 0, 5));
            squareBoxFactory.AddHandler(TextBox.PreviewTextInputEvent, new TextCompositionEventHandler(NumberOnly_PreviewTextInput));
            squareBoxFactory.AddHandler(TextBox.TextChangedEvent, new TextChangedEventHandler(LimitLength_TextChanged));

            var descBoxFactory = new FrameworkElementFactory(typeof(TextBox));
            descBoxFactory.SetBinding(TextBox.TextProperty, new Binding("description") { Mode = BindingMode.TwoWay });
            descBoxFactory.SetValue(TextBox.ForegroundProperty, new SolidColorBrush((Color)ColorConverter.ConvertFromString("#AAAAAA")));
            descBoxFactory.SetValue(TextBox.FontSizeProperty, 12.0);
            descBoxFactory.SetValue(TextBox.TextWrappingProperty, TextWrapping.Wrap);
            descBoxFactory.SetValue(TextBox.BackgroundProperty, Brushes.Transparent);
            descBoxFactory.SetValue(TextBox.BorderBrushProperty, Brushes.Gray);
            descBoxFactory.SetValue(TextBox.MarginProperty, new Thickness(0, 0, 0, 5));

            var descRuBoxFactory = new FrameworkElementFactory(typeof(TextBox));
            descRuBoxFactory.SetBinding(TextBox.TextProperty, new Binding("description_ru") { Mode = BindingMode.TwoWay });
            descRuBoxFactory.SetValue(TextBox.ForegroundProperty, new SolidColorBrush((Color)ColorConverter.ConvertFromString("#AAAAAA")));
            descRuBoxFactory.SetValue(TextBox.FontSizeProperty, 12.0);
            descRuBoxFactory.SetValue(TextBox.TextWrappingProperty, TextWrapping.Wrap);
            descRuBoxFactory.SetValue(TextBox.BackgroundProperty, Brushes.Transparent);
            descRuBoxFactory.SetValue(TextBox.BorderBrushProperty, Brushes.Gray);

            
            var buttonStackFactory = new FrameworkElementFactory(typeof(StackPanel));
            buttonStackFactory.SetValue(StackPanel.OrientationProperty, Orientation.Horizontal);
            buttonStackFactory.SetValue(StackPanel.VerticalAlignmentProperty, VerticalAlignment.Center);
            buttonStackFactory.SetValue(StackPanel.MarginProperty, new Thickness(20, 0, 0, 0));

            var deleteButton = new FrameworkElementFactory(typeof(Button));
            deleteButton.SetValue(Button.ContentProperty, "🗑️");
            deleteButton.SetValue(Button.ToolTipProperty, "Удалить");
            deleteButton.SetValue(Button.FontSizeProperty, 20.0);
            deleteButton.SetValue(Button.WidthProperty, 40.0);
            deleteButton.SetValue(Button.HeightProperty, 40.0);
            deleteButton.SetBinding(Button.DataContextProperty, new Binding("."));
            deleteButton.AddHandler(Button.ClickEvent, new RoutedEventHandler(DeleteStudio_Click));

            var editButton = new FrameworkElementFactory(typeof(Button));
            editButton.SetValue(Button.ContentProperty, "💾");
            editButton.SetValue(Button.ToolTipProperty, "Сохранить");
            editButton.SetValue(Button.FontSizeProperty, 20.0);
            editButton.SetValue(Button.WidthProperty, 40.0);
            editButton.SetValue(Button.HeightProperty, 40.0);
            editButton.AddHandler(Button.ClickEvent, new RoutedEventHandler(EditStudio_Click));

            buttonStackFactory.AppendChild(deleteButton);
            buttonStackFactory.AppendChild(editButton);

            textStackFactory.AppendChild(nameBoxFactory);
            textStackFactory.AppendChild(nameRuBoxFactory);
            textStackFactory.AppendChild(priceBoxFactory);
            textStackFactory.AppendChild(squareBoxFactory);
            textStackFactory.AppendChild(descBoxFactory);
            textStackFactory.AppendChild(descRuBoxFactory);

            stackPanelFactory.AppendChild(imageFactory);
            stackPanelFactory.AppendChild(textStackFactory);
            stackPanelFactory.AppendChild(buttonStackFactory);

            itemTemplate.VisualTree = stackPanelFactory;
            listView.ItemTemplate = itemTemplate;
            listView.ItemsSource = studios;

            StudiosPanel.Children.Clear();
            StudiosPanel.Children.Add(listView);
        }

        public class EditableStudio : Studio
        {
            public EditableStudio() { }
            public EditableStudio(Studio s)
            {
                id = s.id;
                name = s.name;
                name_ru = s.name_ru;
                price = s.price;
                square = s.square;
                description = s.description;
                description_ru = s.description_ru;
                photoPath = s.photoPath;
            }
        }

        private void SaveImagePath(EditableStudio studio, string path)
        {
            try
            {
                using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                {
                    connection.Open();

                    string updateQuery = "UPDATE Studios SET photoPath = @photoPath WHERE id = @id";
                    using (var command = new SQLiteCommand(updateQuery, connection))
                    {
                        command.Parameters.AddWithValue("@photoPath", path);
                        command.Parameters.AddWithValue("@id", studio.id);
                        command.ExecuteNonQuery();
                    }
                }

                
                studio.photoPath = path;

                
                var studios = (List<EditableStudio>)((ListView)StudiosPanel.Children[0]).ItemsSource;
                ((ListView)StudiosPanel.Children[0]).ItemsSource = null;
                ((ListView)StudiosPanel.Children[0]).ItemsSource = studios;
                UpdateStudiosList();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка обновления фото студии: {ex.Message}");
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
                    var image = sender as Image;
                    if (image == null) return;

                    var studio = image.DataContext as EditableStudio;
                    if (studio == null) return;

                    SaveImagePath(studio, imagePath);
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
                var image = sender as Image;
                if (image == null) return;

                var studio = image.DataContext as EditableStudio;
                if (studio == null) return;

                SaveImagePath(studio, selectedFile);
            }
        }


        private void EditStudio_Click(object sender, RoutedEventArgs e)
        {
            if (sender is Button button && button.DataContext is EditableStudio studio)
            {
                bool hasErrors = false;

                if (string.IsNullOrWhiteSpace(studio.name)) hasErrors = true;
                if (string.IsNullOrWhiteSpace(studio.description)) hasErrors = true;
                if (string.IsNullOrWhiteSpace(studio.name_ru)) hasErrors = true;
                if (string.IsNullOrWhiteSpace(studio.description_ru)) hasErrors = true;
                if (string.IsNullOrWhiteSpace(studio.description_ru)) hasErrors = true;

                if (!Regex.IsMatch(studio.name, @"^[a-zA-Z0-9\s]{1,10}$")) hasErrors = true;
                if (!Regex.IsMatch(studio.description, @"^[a-zA-Z0-9\s.,!?-]{1,530}$")) hasErrors = true;

                if (!Regex.IsMatch(studio.name_ru, @"^[а-яА-ЯёЁ0-9\s]{1,10}$")) hasErrors = true;
                if (!Regex.IsMatch(studio.description_ru, @"^[а-яА-ЯёЁ0-9\s.,!?-]{1,530}$")) hasErrors = true;

                if (string.IsNullOrWhiteSpace(studio.price.ToString()) || studio.price.ToString() == "0" || !Regex.IsMatch(studio.price.ToString(), @"\d+")) hasErrors = true;
                if (string.IsNullOrWhiteSpace(studio.square.ToString()) || studio.square.ToString() == "0" || !Regex.IsMatch(studio.square.ToString(), @"\d+")) hasErrors = true;


                if (hasErrors)
                {
                    MessageBox.Show("Проверьте корректность всех полей.", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return;
                }

                try
                {
                    using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                    {
                        connection.Open();
                        string query = @"
                    UPDATE Studios 
                    SET name = @name, name_ru = @name_ru, price = @price, square = @square, 
                        description = @desc, description_ru = @desc_ru 
                    WHERE id = @id";

                        using (var command = new SQLiteCommand(query, connection))
                        {
                            command.Parameters.AddWithValue("@name", studio.name);
                            command.Parameters.AddWithValue("@name_ru", studio.name_ru);
                            command.Parameters.AddWithValue("@price", studio.price);
                            command.Parameters.AddWithValue("@square", studio.square);
                            command.Parameters.AddWithValue("@desc", studio.description);
                            command.Parameters.AddWithValue("@desc_ru", studio.description_ru);
                            command.Parameters.AddWithValue("@id", studio.id);
                            command.ExecuteNonQuery();
                        }
                    }

                    MessageBox.Show("Изменения сохранены.", "Успешно", MessageBoxButton.OK, MessageBoxImage.Information);
                    DisplayStudios(studios);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Ошибка при сохранении: " + ex.Message);
                }
            }
        }
        private void NumberOnly_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            e.Handled = !Regex.IsMatch(e.Text, "^[0-9]+$");
        }

        private void LimitLength_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (sender is TextBox textBox)
            {
                if (textBox.Text.Length > 4)
                {
                    int selectionStart = textBox.SelectionStart;
                    textBox.Text = textBox.Text.Substring(0, 4);
                    textBox.SelectionStart = Math.Min(selectionStart, textBox.Text.Length);
                }
            }
        }

        private void DeleteStudio_Click(object sender, RoutedEventArgs e)
        {
            if (sender is Button button && button.DataContext is EditableStudio studio)
            {
                try
                {
                    using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                    {
                        connection.Open();

                        string checkOrdersQuery = "SELECT COUNT(*) FROM StudioRentOrders WHERE studio_id = @studioId";
                        using (var checkCommand = new SQLiteCommand(checkOrdersQuery, connection))
                        {
                            checkCommand.Parameters.AddWithValue("@studioId", studio.id);
                            long orderCount = (long)checkCommand.ExecuteScalar();

                            if (orderCount > 0)
                            {
                                MessageBox.Show("Нельзя удалить студию, так как по ней есть заказы.", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Warning);
                                return;
                            }
                        }

                        MessageBoxResult result = MessageBox.Show($"Удалить студию «{studio.name}»?", "Подтверждение", MessageBoxButton.YesNo);
                        if (result == MessageBoxResult.Yes)
                        {
                            string deleteQuery = "DELETE FROM Studios WHERE id = @id";
                            using (var deleteCommand = new SQLiteCommand(deleteQuery, connection))
                            {
                                deleteCommand.Parameters.AddWithValue("@id", studio.id);
                                deleteCommand.ExecuteNonQuery();
                            }

                            studios = studios.Where(s => s.id != studio.id).ToList();
                            DisplayStudios(studios);
                        }
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Ошибка при удалении студии: " + ex.Message);
                }
            }
        }


        private void Button_Create_Click(object sender, RoutedEventArgs e)
        {
            
            CreateNameText.ToolTip = "";
            CreateDescriptionText.ToolTip = "";
            CreatePathText.ToolTip = "";
            CreatePriceText.ToolTip = "";
            CreateSquareText.ToolTip = "";
            CreateNameRuText.ToolTip = "";
            CreateDescriptionRuText.ToolTip = "";
            CreateNameText.BorderBrush = Brushes.White;
            CreateDescriptionText.BorderBrush = Brushes.White;
            CreatePathText.BorderBrush = Brushes.White;
            CreatePriceText.BorderBrush = Brushes.White;
            CreateSquareText.BorderBrush = Brushes.White;
            CreateNameRuText.BorderBrush = Brushes.White;
            CreateDescriptionRuText.BorderBrush = Brushes.White;

            bool hasErrors = false;

            
            if (string.IsNullOrWhiteSpace(CreateNameText.Text))
            {
                CreateNameText.ToolTip = "Поле не может быть пустым";
                CreateNameText.BorderBrush = Brushes.DarkRed;
                hasErrors = true;
            }

            if (string.IsNullOrWhiteSpace(CreateDescriptionText.Text))
            {
                CreateDescriptionText.ToolTip = "Поле не может быть пустым";
                CreateDescriptionText.BorderBrush = Brushes.DarkRed;
                hasErrors = true;
            }

            if (string.IsNullOrWhiteSpace(CreatePathText.Text))
            {
                CreatePathText.ToolTip = "Поле не может быть пустым";
                CreatePathText.BorderBrush = Brushes.DarkRed;
                hasErrors = true;
            }

            if (string.IsNullOrWhiteSpace(CreatePriceText.Text))
            {
                CreatePriceText.ToolTip = "Поле не может быть пустым";
                CreatePriceText.BorderBrush = Brushes.DarkRed;
                hasErrors = true;
            }

            if (string.IsNullOrWhiteSpace(CreateSquareText.Text))
            {
                CreateSquareText.ToolTip = "Поле не может быть пустым";
                CreateSquareText.BorderBrush = Brushes.DarkRed;
                hasErrors = true;
            }

            if (string.IsNullOrWhiteSpace(CreateNameRuText.Text))
            {
                CreateNameRuText.ToolTip = "Поле не может быть пустым";
                CreateNameRuText.BorderBrush = Brushes.DarkRed;
                hasErrors = true;
            }

            if (string.IsNullOrWhiteSpace(CreateDescriptionRuText.Text))
            {
                CreateDescriptionRuText.ToolTip = "Поле не может быть пустым";
                CreateDescriptionRuText.BorderBrush = Brushes.DarkRed;
                hasErrors = true;
            }

                if (!Regex.IsMatch(CreateNameText.Text, @"^[a-zA-Z0-9\s]{1,10}$"))
                {
                    CreateNameText.ToolTip = "Только английские буквы и цифры";
                    CreateNameText.BorderBrush = Brushes.DarkRed;
                    hasErrors = true;
                }

                if (!Regex.IsMatch(CreateDescriptionText.Text, @"^[a-zA-Z][a-zA-Z0-9\s.,!?-]{0,529}$"))
                {
                    CreateDescriptionText.ToolTip = "Должно начинаться с английской буквы";
                    CreateDescriptionText.BorderBrush = Brushes.DarkRed;
                    hasErrors = true;
                }

                if (!System.IO.File.Exists(CreatePathText.Text))
                {
                    CreatePathText.ToolTip = "Файл не найден";
                    CreatePathText.BorderBrush = Brushes.DarkRed;
                    hasErrors = true;
                }

                if (!Regex.IsMatch(CreatePriceText.Text, @"^\d+$"))
                {
                    CreatePriceText.ToolTip = "Только цифры";
                    CreatePriceText.BorderBrush = Brushes.DarkRed;
                    hasErrors = true;
                }
                else if (Convert.ToInt32(CreatePriceText.Text) <= 0)
                {
                    CreatePriceText.ToolTip = "Цена должна быть больше 0";
                    CreatePriceText.BorderBrush = Brushes.DarkRed;
                    hasErrors = true;
                }

                if (!Regex.IsMatch(CreateSquareText.Text, @"^\d+$"))
                {
                    CreateSquareText.ToolTip = "Только цифры";
                    CreateSquareText.BorderBrush = Brushes.DarkRed;
                    hasErrors = true;
                }
                else if (Convert.ToInt32(CreateSquareText.Text) <= 0)
                {
                    CreateSquareText.ToolTip = "Площадь должна быть больше 0";
                    CreateSquareText.BorderBrush = Brushes.DarkRed;
                    hasErrors = true;
                }

                if (!Regex.IsMatch(CreateNameRuText.Text, @"^[а-яА-ЯёЁ0-9\s]{1,10}$"))
                {
                    CreateNameRuText.ToolTip = "Только русские буквы и цифры";
                    CreateNameRuText.BorderBrush = Brushes.DarkRed;
                    hasErrors = true;
                }

                if (!Regex.IsMatch(CreateDescriptionRuText.Text, @"^[а-яА-ЯёЁ][а-яА-ЯёЁ0-9\s.,!?-]{0,529}$"))
                {
                    CreateDescriptionRuText.ToolTip = "Должно начинаться с русской буквы";
                    CreateDescriptionRuText.BorderBrush = Brushes.DarkRed;
                    hasErrors = true;
                }

            if (hasErrors)
            {
                return;
            }

            try
            {
                using (var connection = new SQLiteConnection(AppContext.ConnectionString))
                {
                    connection.Open();

                    string query = @"INSERT INTO Studios 
                  (name, description, name_ru, description_ru, price, photoPath, square) 
                  VALUES 
                  (@name, @description, @name_ru, @description_ru, @price, @photoPath, @square)";

                    using (var command = new SQLiteCommand(query, connection))
                    {
                        command.Parameters.AddWithValue("@name", CreateNameText.Text);
                        command.Parameters.AddWithValue("@description", CreateDescriptionText.Text);
                        command.Parameters.AddWithValue("@name_ru", CreateNameRuText.Text);
                        command.Parameters.AddWithValue("@description_ru", CreateDescriptionRuText.Text);
                        command.Parameters.AddWithValue("@price", Convert.ToDouble(CreatePriceText.Text));
                        command.Parameters.AddWithValue("@photoPath", CreatePathText.Text);
                        command.Parameters.AddWithValue("@square", Convert.ToInt32(CreateSquareText.Text));

                        command.ExecuteNonQuery();
                    }
                }

                MessageBox.Show("Студия успешно добавлена!");

                
                CreateNameText.Text = "";
                CreateDescriptionText.Text = "";
                CreatePathText.Text = "";
                CreatePriceText.Text = "";
                CreateSquareText.Text = "";
                CreateNameRuText.Text = "";
                CreateDescriptionRuText.Text = "";

                
                CreateNameText.ToolTip = "";
                CreateDescriptionText.ToolTip = "";
                CreatePathText.ToolTip = "";
                CreatePriceText.ToolTip = "";
                CreateSquareText.ToolTip = "";
                CreateNameRuText.ToolTip = "";
                CreateDescriptionRuText.ToolTip = "";
                CreateNameText.BorderBrush = Brushes.White;
                CreateDescriptionText.BorderBrush = Brushes.White;
                CreatePathText.BorderBrush = Brushes.White;
                CreatePriceText.BorderBrush = Brushes.White;
                CreateSquareText.BorderBrush = Brushes.White;
                CreateNameRuText.BorderBrush = Brushes.White;
                CreateDescriptionRuText.BorderBrush = Brushes.White;

                UpdateStudiosList();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при добавлении студии: {ex.Message}");
            }
        }

        private void UpdateStudiosList()
        {
            studios = new Studio().LoadAllStudios().Select(s => new EditableStudio(s)).ToList();
            DisplayStudios(studios);
        }

        private void StatusButton(object sender, RoutedEventArgs e)
        {
            NavigateToPage("StatusPage.xaml");
        }

        private void ReviewButton(object sender, RoutedEventArgs e)
        {
            NavigateToPage("ReviewsPage.xaml");
        }
        private void TimetableButton(object sender, RoutedEventArgs e)
        {
            NavigateToPage("TimetablePage.xaml");
        }

        private void RentButton(object sender, RoutedEventArgs e)
        {
            NavigateToPage("BookingPage.xaml");
        }

        private void StudiosButton(object sender, RoutedEventArgs e)
        {
            ShowMainContent(); 
        }

        public void NavigateToPage(string pageUri)
        {
            AdminPageContent.Visibility = Visibility.Collapsed;
            AdminPageFrame.Visibility = Visibility.Visible;
            AdminPageFrame.Navigate(new Uri(pageUri, UriKind.Relative));
        }

        private void ShowMainContent()
        {
            AdminPageFrame.Visibility = Visibility.Collapsed;
            AdminPageContent.Visibility = Visibility.Visible;
            AdminPageFrame.Content = null;
        }

        public void ReturnToMainContent()
        {
            AdminPageFrame.Visibility = Visibility.Collapsed;
            AdminPageContent.Visibility = Visibility.Visible;
            AdminPageFrame.Content = null;
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
