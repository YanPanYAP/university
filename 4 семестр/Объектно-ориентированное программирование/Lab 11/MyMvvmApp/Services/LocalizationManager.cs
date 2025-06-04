using System;
using System.Linq;
using System.Windows;

namespace MyMvvmApp.Services
{
    public enum AppLanguage
    {
        English,
        Russian
    }

    public static class LocalizationManager
    {
        public static AppLanguage CurrentLanguage { get; private set; } = AppLanguage.English;

        public static void ChangeLanguage(AppLanguage lang)
        {
            try
            {
                var dict = new ResourceDictionary();

                switch (lang)
                {
                    case AppLanguage.Russian:
                        dict.Source = new Uri("Resources/Strings.ru.xaml", UriKind.Relative);
                        break;
                    default:
                        dict.Source = new Uri("Resources/Strings.en.xaml", UriKind.Relative);
                        break;
                }

                var oldDict = Application.Current.Resources.MergedDictionaries
                    .FirstOrDefault(d => d.Source != null && d.Source.OriginalString.Contains("Strings"));

                if (oldDict != null)
                    Application.Current.Resources.MergedDictionaries.Remove(oldDict);

                Application.Current.Resources.MergedDictionaries.Add(dict);
                CurrentLanguage = lang;
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при смене языка: {ex.Message}");
            }
        }
    }
}
