﻿#pragma checksum "..\..\StatusPage.xaml" "{8829d00f-11b8-4213-878b-770e8597ac16}" "F216F4EEC93073C53BF3A5825D6CC8D404352C0C6EDEBDA2BB8F1D308B2EFEA5"
//------------------------------------------------------------------------------
// <auto-generated>
//     Этот код создан программой.
//     Исполняемая версия:4.0.30319.42000
//
//     Изменения в этом файле могут привести к неправильной работе и будут потеряны в случае
//     повторной генерации кода.
// </auto-generated>
//------------------------------------------------------------------------------

using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Media.TextFormatting;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Shell;
using photoLoom;


namespace photoLoom {
    
    
    /// <summary>
    /// StatusPage
    /// </summary>
    public partial class StatusPage : System.Windows.Controls.Page, System.Windows.Markup.IComponentConnector {
        
        
        #line 12 "..\..\StatusPage.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Frame StatusPageFrame;
        
        #line default
        #line hidden
        
        
        #line 13 "..\..\StatusPage.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Grid StatusPageContent;
        
        #line default
        #line hidden
        
        
        #line 18 "..\..\StatusPage.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button StudiosButton3;
        
        #line default
        #line hidden
        
        
        #line 19 "..\..\StatusPage.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button TimetableButton3;
        
        #line default
        #line hidden
        
        
        #line 20 "..\..\StatusPage.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button StatusButton3;
        
        #line default
        #line hidden
        
        
        #line 21 "..\..\StatusPage.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button ReviewButton3;
        
        #line default
        #line hidden
        
        
        #line 22 "..\..\StatusPage.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button RentButton3;
        
        #line default
        #line hidden
        
        
        #line 44 "..\..\StatusPage.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.StackPanel StatusPanel;
        
        #line default
        #line hidden
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Uri resourceLocater = new System.Uri("/photoLoom;component/statuspage.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\StatusPage.xaml"
            System.Windows.Application.LoadComponent(this, resourceLocater);
            
            #line default
            #line hidden
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        [System.ComponentModel.EditorBrowsableAttribute(System.ComponentModel.EditorBrowsableState.Never)]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Design", "CA1033:InterfaceMethodsShouldBeCallableByChildTypes")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Maintainability", "CA1502:AvoidExcessiveComplexity")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1800:DoNotCastUnnecessarily")]
        void System.Windows.Markup.IComponentConnector.Connect(int connectionId, object target) {
            switch (connectionId)
            {
            case 1:
            this.StatusPageFrame = ((System.Windows.Controls.Frame)(target));
            return;
            case 2:
            this.StatusPageContent = ((System.Windows.Controls.Grid)(target));
            return;
            case 3:
            
            #line 17 "..\..\StatusPage.xaml"
            ((System.Windows.Controls.Button)(target)).Click += new System.Windows.RoutedEventHandler(this.BackButton_Click);
            
            #line default
            #line hidden
            return;
            case 4:
            this.StudiosButton3 = ((System.Windows.Controls.Button)(target));
            
            #line 18 "..\..\StatusPage.xaml"
            this.StudiosButton3.Click += new System.Windows.RoutedEventHandler(this.StudiosButton);
            
            #line default
            #line hidden
            return;
            case 5:
            this.TimetableButton3 = ((System.Windows.Controls.Button)(target));
            
            #line 19 "..\..\StatusPage.xaml"
            this.TimetableButton3.Click += new System.Windows.RoutedEventHandler(this.TimetableButton);
            
            #line default
            #line hidden
            return;
            case 6:
            this.StatusButton3 = ((System.Windows.Controls.Button)(target));
            
            #line 20 "..\..\StatusPage.xaml"
            this.StatusButton3.Click += new System.Windows.RoutedEventHandler(this.StatusButton);
            
            #line default
            #line hidden
            return;
            case 7:
            this.ReviewButton3 = ((System.Windows.Controls.Button)(target));
            
            #line 21 "..\..\StatusPage.xaml"
            this.ReviewButton3.Click += new System.Windows.RoutedEventHandler(this.ReviewButton);
            
            #line default
            #line hidden
            return;
            case 8:
            this.RentButton3 = ((System.Windows.Controls.Button)(target));
            
            #line 22 "..\..\StatusPage.xaml"
            this.RentButton3.Click += new System.Windows.RoutedEventHandler(this.RentButton);
            
            #line default
            #line hidden
            return;
            case 9:
            this.StatusPanel = ((System.Windows.Controls.StackPanel)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}

