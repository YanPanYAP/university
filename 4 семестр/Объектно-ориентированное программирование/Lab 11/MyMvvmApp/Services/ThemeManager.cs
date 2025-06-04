using System;
using System.Linq;
using System.Windows;

namespace MyMvvmApp.Services
{
    public enum AppTheme
    {
        Light,
        Dark
    }

    public static class ThemeManager
    {
        public static AppTheme CurrentTheme { get; private set; } = AppTheme.Dark;

        public static void ChangeTheme(AppTheme theme)
        {
            try
            {
                var dict = new ResourceDictionary();

                switch (theme)
                {
                    case AppTheme.Light:
                        dict.Source = new Uri("/Resources/Theme.Light.xaml", UriKind.Relative);
                        break;
                    default:
                        dict.Source = new Uri("/Resources/Theme.Dark.xaml", UriKind.Relative);
                        break;
                }

                var oldDict = Application.Current.Resources.MergedDictionaries
                    .FirstOrDefault(d => d.Source != null && d.Source.OriginalString.Contains("Theme"));

                if (oldDict != null)
                    Application.Current.Resources.MergedDictionaries.Remove(oldDict);

                Application.Current.Resources.MergedDictionaries.Add(dict);
                CurrentTheme = theme;
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при смене темы: {ex.Message}", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
    }
}
