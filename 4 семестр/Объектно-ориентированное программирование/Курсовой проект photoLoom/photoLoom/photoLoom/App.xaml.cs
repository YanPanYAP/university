using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using static photoLoom.MainWindow;

namespace photoLoom
{
    /// <summary>
    /// Логика взаимодействия для App.xaml
    /// </summary>
    public partial class App : Application
    {

        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            Mouse.OverrideCursor = new Cursor("C:\\Mac\\Home\\Documents\\fit\\ООП\\photoLoom\\photoLoom\\resources\\images\\gam1502.cur");
        }
    }
}
