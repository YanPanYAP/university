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

namespace photoLoom
{
    /// <summary>
    /// Логика взаимодействия для PhotographersPage.xaml
    /// </summary>
    public partial class PhotographersPage : Page
    {
        public PhotographersPage()
        {
            InitializeComponent();
        }

        private void LanguageButton_Click(object sender, RoutedEventArgs e)
        {
            var newLang = LocalizationManager.CurrentLanguage == AppLanguage.English
                ? AppLanguage.Russian
                : AppLanguage.English;

            LocalizationManager.ChangeLanguage(newLang);
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
            ShowMainContent();
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
            PhotographersContent.Visibility = Visibility.Collapsed;
            PhotographersFrame.Visibility = Visibility.Visible;
            PhotographersFrame.Navigate(new Uri(pageUri, UriKind.Relative));
        }

        private void ShowMainContent()
        {
            PhotographersFrame.Visibility = Visibility.Collapsed;
            PhotographersContent.Visibility = Visibility.Visible;
            PhotographersFrame.Content = null;
        }
        public void ReturnToMainContent()
        {
            PhotographersFrame.Visibility = Visibility.Collapsed;
            PhotographersContent.Visibility = Visibility.Visible;
            PhotographersFrame.Content = null;
        }
    }
}
