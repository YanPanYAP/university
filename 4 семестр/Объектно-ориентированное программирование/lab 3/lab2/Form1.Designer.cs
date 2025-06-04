namespace lab2
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.lblFullName = new System.Windows.Forms.Label();
            this.txtFullName = new System.Windows.Forms.TextBox();
            this.lblAge = new System.Windows.Forms.Label();
            this.numAge = new System.Windows.Forms.NumericUpDown();
            this.lblSpecialty = new System.Windows.Forms.Label();
            this.txtSpecialty = new System.Windows.Forms.TextBox();
            this.lblBirthDate = new System.Windows.Forms.Label();
            this.dtpBirthDate = new System.Windows.Forms.DateTimePicker();
            this.lblCourse = new System.Windows.Forms.Label();
            this.lblGroup = new System.Windows.Forms.Label();
            this.txtGroup = new System.Windows.Forms.TextBox();
            this.lblGender = new System.Windows.Forms.Label();
            this.rbMale = new System.Windows.Forms.RadioButton();
            this.rbFemale = new System.Windows.Forms.RadioButton();
            this.lblGrade = new System.Windows.Forms.Label();
            this.trackBarGrade = new System.Windows.Forms.TrackBar();
            this.grpAddress = new System.Windows.Forms.GroupBox();
            this.txtApartment = new System.Windows.Forms.TextBox();
            this.lblApartment = new System.Windows.Forms.Label();
            this.txtHouse = new System.Windows.Forms.TextBox();
            this.lblHouse = new System.Windows.Forms.Label();
            this.txtStreet = new System.Windows.Forms.TextBox();
            this.lblStreet = new System.Windows.Forms.Label();
            this.txtIndex = new System.Windows.Forms.TextBox();
            this.lblIndex = new System.Windows.Forms.Label();
            this.txtCity = new System.Windows.Forms.TextBox();
            this.lblCity = new System.Windows.Forms.Label();
            this.grpWorkplace = new System.Windows.Forms.GroupBox();
            this.numExperience = new System.Windows.Forms.NumericUpDown();
            this.lblExperience = new System.Windows.Forms.Label();
            this.txtPosition = new System.Windows.Forms.TextBox();
            this.lblPosition = new System.Windows.Forms.Label();
            this.txtCompany = new System.Windows.Forms.TextBox();
            this.lblCompany = new System.Windows.Forms.Label();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnLoad = new System.Windows.Forms.Button();
            this.btnCalculate = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.findButton = new System.Windows.Forms.ToolStripButton();
            this.btnDelete = new System.Windows.Forms.ToolStripButton();
            this.clearWindow = new System.Windows.Forms.ToolStripButton();
            this.btnPrev = new System.Windows.Forms.ToolStripButton();
            this.btnNext = new System.Windows.Forms.ToolStripButton();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.findButton2 = new System.Windows.Forms.ToolStripMenuItem();
            this.сортировкаПоToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fromalphabet = new System.Windows.Forms.ToolStripMenuItem();
            this.toAlphabet = new System.Windows.Forms.ToolStripMenuItem();
            this.btnSave2 = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutBtn = new System.Windows.Forms.ToolStripMenuItem();
            this.hideShowMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.writeDate = new System.Windows.Forms.MonthCalendar();
            this.numCourse = new System.Windows.Forms.ComboBox();
            this.findTextBox = new System.Windows.Forms.TextBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.lblObjectCount = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblLastAction = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblDateTime = new System.Windows.Forms.ToolStripStatusLabel();
            this.aboutText = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.numAge)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGrade)).BeginInit();
            this.grpAddress.SuspendLayout();
            this.grpWorkplace.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numExperience)).BeginInit();
            this.panel1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // lblFullName
            // 
            this.lblFullName.AutoSize = true;
            this.lblFullName.Location = new System.Drawing.Point(98, 230);
            this.lblFullName.Name = "lblFullName";
            this.lblFullName.Size = new System.Drawing.Size(61, 25);
            this.lblFullName.TabIndex = 0;
            this.lblFullName.Text = "ФИО";
            // 
            // txtFullName
            // 
            this.txtFullName.Location = new System.Drawing.Point(341, 230);
            this.txtFullName.Name = "txtFullName";
            this.txtFullName.Size = new System.Drawing.Size(291, 31);
            this.txtFullName.TabIndex = 1;
            // 
            // lblAge
            // 
            this.lblAge.AutoSize = true;
            this.lblAge.Location = new System.Drawing.Point(98, 281);
            this.lblAge.Name = "lblAge";
            this.lblAge.Size = new System.Drawing.Size(94, 25);
            this.lblAge.TabIndex = 2;
            this.lblAge.Text = "Возраст";
            // 
            // numAge
            // 
            this.numAge.Location = new System.Drawing.Point(494, 279);
            this.numAge.Maximum = new decimal(new int[] {
            120,
            0,
            0,
            0});
            this.numAge.Minimum = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.numAge.Name = "numAge";
            this.numAge.Size = new System.Drawing.Size(138, 31);
            this.numAge.TabIndex = 3;
            this.numAge.Value = new decimal(new int[] {
            16,
            0,
            0,
            0});
            // 
            // lblSpecialty
            // 
            this.lblSpecialty.AutoSize = true;
            this.lblSpecialty.Location = new System.Drawing.Point(98, 333);
            this.lblSpecialty.Name = "lblSpecialty";
            this.lblSpecialty.Size = new System.Drawing.Size(166, 25);
            this.lblSpecialty.TabIndex = 4;
            this.lblSpecialty.Text = "Специальность";
            // 
            // txtSpecialty
            // 
            this.txtSpecialty.Location = new System.Drawing.Point(341, 327);
            this.txtSpecialty.Name = "txtSpecialty";
            this.txtSpecialty.Size = new System.Drawing.Size(291, 31);
            this.txtSpecialty.TabIndex = 5;
            // 
            // lblBirthDate
            // 
            this.lblBirthDate.AutoSize = true;
            this.lblBirthDate.Location = new System.Drawing.Point(98, 390);
            this.lblBirthDate.Name = "lblBirthDate";
            this.lblBirthDate.Size = new System.Drawing.Size(166, 25);
            this.lblBirthDate.TabIndex = 6;
            this.lblBirthDate.Text = "Дата рождения";
            // 
            // dtpBirthDate
            // 
            this.dtpBirthDate.Location = new System.Drawing.Point(341, 385);
            this.dtpBirthDate.Name = "dtpBirthDate";
            this.dtpBirthDate.Size = new System.Drawing.Size(291, 31);
            this.dtpBirthDate.TabIndex = 7;
            // 
            // lblCourse
            // 
            this.lblCourse.AutoSize = true;
            this.lblCourse.Location = new System.Drawing.Point(99, 448);
            this.lblCourse.Name = "lblCourse";
            this.lblCourse.Size = new System.Drawing.Size(60, 25);
            this.lblCourse.TabIndex = 8;
            this.lblCourse.Text = "Курс";
            // 
            // lblGroup
            // 
            this.lblGroup.AutoSize = true;
            this.lblGroup.Location = new System.Drawing.Point(99, 504);
            this.lblGroup.Name = "lblGroup";
            this.lblGroup.Size = new System.Drawing.Size(83, 25);
            this.lblGroup.TabIndex = 10;
            this.lblGroup.Text = "Группа";
            // 
            // txtGroup
            // 
            this.txtGroup.Location = new System.Drawing.Point(494, 498);
            this.txtGroup.Name = "txtGroup";
            this.txtGroup.Size = new System.Drawing.Size(138, 31);
            this.txtGroup.TabIndex = 11;
            // 
            // lblGender
            // 
            this.lblGender.AutoSize = true;
            this.lblGender.Location = new System.Drawing.Point(99, 548);
            this.lblGender.Name = "lblGender";
            this.lblGender.Size = new System.Drawing.Size(51, 25);
            this.lblGender.TabIndex = 12;
            this.lblGender.Text = "Пол";
            // 
            // rbMale
            // 
            this.rbMale.AutoSize = true;
            this.rbMale.Location = new System.Drawing.Point(341, 548);
            this.rbMale.Name = "rbMale";
            this.rbMale.Size = new System.Drawing.Size(133, 29);
            this.rbMale.TabIndex = 13;
            this.rbMale.TabStop = true;
            this.rbMale.Text = "Мужской";
            this.rbMale.UseVisualStyleBackColor = true;
            // 
            // rbFemale
            // 
            this.rbFemale.AutoSize = true;
            this.rbFemale.Location = new System.Drawing.Point(500, 548);
            this.rbFemale.Name = "rbFemale";
            this.rbFemale.Size = new System.Drawing.Size(132, 29);
            this.rbFemale.TabIndex = 14;
            this.rbFemale.TabStop = true;
            this.rbFemale.Text = "Женский";
            this.rbFemale.UseVisualStyleBackColor = true;
            // 
            // lblGrade
            // 
            this.lblGrade.AutoSize = true;
            this.lblGrade.Location = new System.Drawing.Point(98, 601);
            this.lblGrade.Name = "lblGrade";
            this.lblGrade.Size = new System.Drawing.Size(153, 25);
            this.lblGrade.TabIndex = 15;
            this.lblGrade.Text = "Средний балл";
            // 
            // trackBarGrade
            // 
            this.trackBarGrade.Location = new System.Drawing.Point(309, 601);
            this.trackBarGrade.Name = "trackBarGrade";
            this.trackBarGrade.Size = new System.Drawing.Size(323, 90);
            this.trackBarGrade.TabIndex = 16;
            // 
            // grpAddress
            // 
            this.grpAddress.Controls.Add(this.txtApartment);
            this.grpAddress.Controls.Add(this.lblApartment);
            this.grpAddress.Controls.Add(this.txtHouse);
            this.grpAddress.Controls.Add(this.lblHouse);
            this.grpAddress.Controls.Add(this.txtStreet);
            this.grpAddress.Controls.Add(this.lblStreet);
            this.grpAddress.Controls.Add(this.txtIndex);
            this.grpAddress.Controls.Add(this.lblIndex);
            this.grpAddress.Controls.Add(this.txtCity);
            this.grpAddress.Controls.Add(this.lblCity);
            this.grpAddress.Location = new System.Drawing.Point(103, 672);
            this.grpAddress.Name = "grpAddress";
            this.grpAddress.Size = new System.Drawing.Size(516, 336);
            this.grpAddress.TabIndex = 17;
            this.grpAddress.TabStop = false;
            this.grpAddress.Text = "Адрес";
            // 
            // txtApartment
            // 
            this.txtApartment.Location = new System.Drawing.Point(243, 261);
            this.txtApartment.Name = "txtApartment";
            this.txtApartment.Size = new System.Drawing.Size(207, 31);
            this.txtApartment.TabIndex = 9;
            // 
            // lblApartment
            // 
            this.lblApartment.AutoSize = true;
            this.lblApartment.Location = new System.Drawing.Point(31, 264);
            this.lblApartment.Name = "lblApartment";
            this.lblApartment.Size = new System.Drawing.Size(107, 25);
            this.lblApartment.TabIndex = 8;
            this.lblApartment.Text = "Квартира";
            // 
            // txtHouse
            // 
            this.txtHouse.Location = new System.Drawing.Point(243, 206);
            this.txtHouse.Name = "txtHouse";
            this.txtHouse.Size = new System.Drawing.Size(207, 31);
            this.txtHouse.TabIndex = 7;
            // 
            // lblHouse
            // 
            this.lblHouse.AutoSize = true;
            this.lblHouse.Location = new System.Drawing.Point(31, 209);
            this.lblHouse.Name = "lblHouse";
            this.lblHouse.Size = new System.Drawing.Size(55, 25);
            this.lblHouse.TabIndex = 6;
            this.lblHouse.Text = "Дом";
            // 
            // txtStreet
            // 
            this.txtStreet.Location = new System.Drawing.Point(243, 155);
            this.txtStreet.Name = "txtStreet";
            this.txtStreet.Size = new System.Drawing.Size(207, 31);
            this.txtStreet.TabIndex = 5;
            // 
            // lblStreet
            // 
            this.lblStreet.AutoSize = true;
            this.lblStreet.Location = new System.Drawing.Point(31, 158);
            this.lblStreet.Name = "lblStreet";
            this.lblStreet.Size = new System.Drawing.Size(73, 25);
            this.lblStreet.TabIndex = 4;
            this.lblStreet.Text = "Улица";
            // 
            // txtIndex
            // 
            this.txtIndex.Location = new System.Drawing.Point(243, 97);
            this.txtIndex.Name = "txtIndex";
            this.txtIndex.Size = new System.Drawing.Size(207, 31);
            this.txtIndex.TabIndex = 3;
            // 
            // lblIndex
            // 
            this.lblIndex.AutoSize = true;
            this.lblIndex.Location = new System.Drawing.Point(31, 103);
            this.lblIndex.Name = "lblIndex";
            this.lblIndex.Size = new System.Drawing.Size(85, 25);
            this.lblIndex.TabIndex = 2;
            this.lblIndex.Text = "Индекс";
            // 
            // txtCity
            // 
            this.txtCity.Location = new System.Drawing.Point(243, 50);
            this.txtCity.Name = "txtCity";
            this.txtCity.Size = new System.Drawing.Size(207, 31);
            this.txtCity.TabIndex = 1;
            // 
            // lblCity
            // 
            this.lblCity.AutoSize = true;
            this.lblCity.Location = new System.Drawing.Point(31, 53);
            this.lblCity.Name = "lblCity";
            this.lblCity.Size = new System.Drawing.Size(72, 25);
            this.lblCity.TabIndex = 0;
            this.lblCity.Text = "Город";
            // 
            // grpWorkplace
            // 
            this.grpWorkplace.Controls.Add(this.numExperience);
            this.grpWorkplace.Controls.Add(this.lblExperience);
            this.grpWorkplace.Controls.Add(this.txtPosition);
            this.grpWorkplace.Controls.Add(this.lblPosition);
            this.grpWorkplace.Controls.Add(this.txtCompany);
            this.grpWorkplace.Controls.Add(this.lblCompany);
            this.grpWorkplace.Location = new System.Drawing.Point(687, 672);
            this.grpWorkplace.Name = "grpWorkplace";
            this.grpWorkplace.Size = new System.Drawing.Size(491, 336);
            this.grpWorkplace.TabIndex = 18;
            this.grpWorkplace.TabStop = false;
            this.grpWorkplace.Text = "Место работы";
            // 
            // numExperience
            // 
            this.numExperience.Location = new System.Drawing.Point(214, 156);
            this.numExperience.Name = "numExperience";
            this.numExperience.Size = new System.Drawing.Size(219, 31);
            this.numExperience.TabIndex = 5;
            // 
            // lblExperience
            // 
            this.lblExperience.AutoSize = true;
            this.lblExperience.Location = new System.Drawing.Point(33, 158);
            this.lblExperience.Name = "lblExperience";
            this.lblExperience.Size = new System.Drawing.Size(64, 25);
            this.lblExperience.TabIndex = 4;
            this.lblExperience.Text = "Стаж";
            // 
            // txtPosition
            // 
            this.txtPosition.Location = new System.Drawing.Point(214, 100);
            this.txtPosition.Name = "txtPosition";
            this.txtPosition.Size = new System.Drawing.Size(219, 31);
            this.txtPosition.TabIndex = 3;
            // 
            // lblPosition
            // 
            this.lblPosition.AutoSize = true;
            this.lblPosition.Location = new System.Drawing.Point(33, 103);
            this.lblPosition.Name = "lblPosition";
            this.lblPosition.Size = new System.Drawing.Size(123, 25);
            this.lblPosition.TabIndex = 2;
            this.lblPosition.Text = "Должность";
            // 
            // txtCompany
            // 
            this.txtCompany.Location = new System.Drawing.Point(214, 50);
            this.txtCompany.Name = "txtCompany";
            this.txtCompany.Size = new System.Drawing.Size(219, 31);
            this.txtCompany.TabIndex = 1;
            // 
            // lblCompany
            // 
            this.lblCompany.AutoSize = true;
            this.lblCompany.Location = new System.Drawing.Point(33, 53);
            this.lblCompany.Name = "lblCompany";
            this.lblCompany.Size = new System.Drawing.Size(112, 25);
            this.lblCompany.TabIndex = 0;
            this.lblCompany.Text = "Компания";
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(103, 1042);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(183, 52);
            this.btnSave.TabIndex = 19;
            this.btnSave.Text = "Сохранить";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // btnLoad
            // 
            this.btnLoad.Location = new System.Drawing.Point(557, 1042);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(183, 52);
            this.btnLoad.TabIndex = 20;
            this.btnLoad.Text = "Загрузить";
            this.btnLoad.UseVisualStyleBackColor = true;
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // btnCalculate
            // 
            this.btnCalculate.Location = new System.Drawing.Point(995, 1042);
            this.btnCalculate.Name = "btnCalculate";
            this.btnCalculate.Size = new System.Drawing.Size(183, 52);
            this.btnCalculate.TabIndex = 21;
            this.btnCalculate.Text = "Рассчитать";
            this.btnCalculate.UseVisualStyleBackColor = true;
            this.btnCalculate.Click += new System.EventHandler(this.btnCalculate_Click);
            // 
            // panel1
            // 
            this.panel1.AutoScroll = true;
            this.panel1.BackColor = System.Drawing.Color.CornflowerBlue;
            this.panel1.Controls.Add(this.toolStrip1);
            this.panel1.Controls.Add(this.menuStrip1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1272, 128);
            this.panel1.TabIndex = 22;
            // 
            // toolStrip1
            // 
            this.toolStrip1.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.findButton,
            this.btnDelete,
            this.clearWindow,
            this.btnPrev,
            this.btnNext});
            this.toolStrip1.Location = new System.Drawing.Point(0, 40);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(1272, 42);
            this.toolStrip1.TabIndex = 1;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // findButton
            // 
            this.findButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.findButton.Image = ((System.Drawing.Image)(resources.GetObject("findButton.Image")));
            this.findButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.findButton.Name = "findButton";
            this.findButton.Size = new System.Drawing.Size(46, 36);
            this.findButton.Text = "toolStripButton1";
            this.findButton.Click += new System.EventHandler(this.findButton_Click);
            // 
            // btnDelete
            // 
            this.btnDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnDelete.Image = ((System.Drawing.Image)(resources.GetObject("btnDelete.Image")));
            this.btnDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnDelete.Name = "btnDelete";
            this.btnDelete.Size = new System.Drawing.Size(46, 36);
            this.btnDelete.Click += new System.EventHandler(this.btnDelete_Click);
            // 
            // clearWindow
            // 
            this.clearWindow.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.clearWindow.Image = ((System.Drawing.Image)(resources.GetObject("clearWindow.Image")));
            this.clearWindow.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.clearWindow.Name = "clearWindow";
            this.clearWindow.Size = new System.Drawing.Size(46, 36);
            this.clearWindow.Click += new System.EventHandler(this.clearWindow_Click);
            // 
            // btnPrev
            // 
            this.btnPrev.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnPrev.Image = ((System.Drawing.Image)(resources.GetObject("btnPrev.Image")));
            this.btnPrev.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnPrev.Name = "btnPrev";
            this.btnPrev.Size = new System.Drawing.Size(46, 36);
            this.btnPrev.Click += new System.EventHandler(this.btnPrev_Click);
            // 
            // btnNext
            // 
            this.btnNext.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnNext.Image = ((System.Drawing.Image)(resources.GetObject("btnNext.Image")));
            this.btnNext.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnNext.Name = "btnNext";
            this.btnNext.Size = new System.Drawing.Size(46, 36);
            this.btnNext.Click += new System.EventHandler(this.btnNext_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.GripMargin = new System.Windows.Forms.Padding(2, 2, 0, 2);
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.findButton2,
            this.сортировкаПоToolStripMenuItem,
            this.btnSave2,
            this.aboutBtn,
            this.hideShowMenu});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1272, 40);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // findButton2
            // 
            this.findButton2.Name = "findButton2";
            this.findButton2.Size = new System.Drawing.Size(102, 36);
            this.findButton2.Text = "Поиск";
            this.findButton2.Click += new System.EventHandler(this.findButton2_Click);
            // 
            // сортировкаПоToolStripMenuItem
            // 
            this.сортировкаПоToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fromalphabet,
            this.toAlphabet});
            this.сортировкаПоToolStripMenuItem.Name = "сортировкаПоToolStripMenuItem";
            this.сортировкаПоToolStripMenuItem.Size = new System.Drawing.Size(166, 36);
            this.сортировкаПоToolStripMenuItem.Text = "Сортировка";
            // 
            // fromalphabet
            // 
            this.fromalphabet.Name = "fromalphabet";
            this.fromalphabet.Size = new System.Drawing.Size(418, 44);
            this.fromalphabet.Text = "в обратном алфавитном";
            this.fromalphabet.Click += new System.EventHandler(this.fromalphabet_Click);
            // 
            // toAlphabet
            // 
            this.toAlphabet.Name = "toAlphabet";
            this.toAlphabet.Size = new System.Drawing.Size(418, 44);
            this.toAlphabet.Text = "в алфавитном порядке";
            this.toAlphabet.Click += new System.EventHandler(this.toAlphabet_Click);
            // 
            // btnSave2
            // 
            this.btnSave2.Name = "btnSave2";
            this.btnSave2.Size = new System.Drawing.Size(150, 36);
            this.btnSave2.Text = "Сохранить";
            this.btnSave2.Click += new System.EventHandler(this.btnSave2_Click);
            // 
            // aboutBtn
            // 
            this.aboutBtn.Name = "aboutBtn";
            this.aboutBtn.Size = new System.Drawing.Size(183, 36);
            this.aboutBtn.Text = "О программе";
            this.aboutBtn.Click += new System.EventHandler(this.aboutBtn_Click);
            // 
            // hideShowMenu
            // 
            this.hideShowMenu.Name = "hideShowMenu";
            this.hideShowMenu.Size = new System.Drawing.Size(185, 36);
            this.hideShowMenu.Text = "Скрыть меню";
            this.hideShowMenu.Click += new System.EventHandler(this.hideShowMenu_Click);
            // 
            // writeDate
            // 
            this.writeDate.Location = new System.Drawing.Point(850, 230);
            this.writeDate.Name = "writeDate";
            this.writeDate.TabIndex = 23;
            // 
            // numCourse
            // 
            this.numCourse.FormattingEnabled = true;
            this.numCourse.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4"});
            this.numCourse.Location = new System.Drawing.Point(494, 445);
            this.numCourse.Name = "numCourse";
            this.numCourse.Size = new System.Drawing.Size(138, 33);
            this.numCourse.TabIndex = 24;
            // 
            // findTextBox
            // 
            this.findTextBox.Location = new System.Drawing.Point(0, 125);
            this.findTextBox.Name = "findTextBox";
            this.findTextBox.Size = new System.Drawing.Size(1272, 31);
            this.findTextBox.TabIndex = 2;
            // 
            // statusStrip1
            // 
            this.statusStrip1.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lblObjectCount,
            this.lblLastAction,
            this.lblDateTime});
            this.statusStrip1.Location = new System.Drawing.Point(0, 1130);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1272, 42);
            this.statusStrip1.TabIndex = 25;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // lblObjectCount
            // 
            this.lblObjectCount.Name = "lblObjectCount";
            this.lblObjectCount.Size = new System.Drawing.Size(237, 32);
            this.lblObjectCount.Text = "toolStripStatusLabel1";
            // 
            // lblLastAction
            // 
            this.lblLastAction.Name = "lblLastAction";
            this.lblLastAction.Size = new System.Drawing.Size(237, 32);
            this.lblLastAction.Text = "toolStripStatusLabel2";
            // 
            // lblDateTime
            // 
            this.lblDateTime.Name = "lblDateTime";
            this.lblDateTime.Size = new System.Drawing.Size(237, 32);
            this.lblDateTime.Text = "toolStripStatusLabel3";
            // 
            // aboutText
            // 
            this.aboutText.AutoSize = true;
            this.aboutText.Location = new System.Drawing.Point(990, 1145);
            this.aboutText.Name = "aboutText";
            this.aboutText.Size = new System.Drawing.Size(0, 25);
            this.aboutText.TabIndex = 26;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Coral;
            this.ClientSize = new System.Drawing.Size(1272, 1172);
            this.Controls.Add(this.aboutText);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.findTextBox);
            this.Controls.Add(this.numCourse);
            this.Controls.Add(this.writeDate);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.btnCalculate);
            this.Controls.Add(this.btnLoad);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.grpWorkplace);
            this.Controls.Add(this.grpAddress);
            this.Controls.Add(this.trackBarGrade);
            this.Controls.Add(this.lblGrade);
            this.Controls.Add(this.rbFemale);
            this.Controls.Add(this.rbMale);
            this.Controls.Add(this.lblGender);
            this.Controls.Add(this.txtGroup);
            this.Controls.Add(this.lblGroup);
            this.Controls.Add(this.lblCourse);
            this.Controls.Add(this.dtpBirthDate);
            this.Controls.Add(this.lblBirthDate);
            this.Controls.Add(this.txtSpecialty);
            this.Controls.Add(this.lblSpecialty);
            this.Controls.Add(this.numAge);
            this.Controls.Add(this.lblAge);
            this.Controls.Add(this.txtFullName);
            this.Controls.Add(this.lblFullName);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.numAge)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGrade)).EndInit();
            this.grpAddress.ResumeLayout(false);
            this.grpAddress.PerformLayout();
            this.grpWorkplace.ResumeLayout(false);
            this.grpWorkplace.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numExperience)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblFullName;
        private System.Windows.Forms.TextBox txtFullName;
        private System.Windows.Forms.Label lblAge;
        private System.Windows.Forms.NumericUpDown numAge;
        private System.Windows.Forms.Label lblSpecialty;
        private System.Windows.Forms.TextBox txtSpecialty;
        private System.Windows.Forms.Label lblBirthDate;
        private System.Windows.Forms.DateTimePicker dtpBirthDate;
        private System.Windows.Forms.Label lblCourse;
        private System.Windows.Forms.Label lblGroup;
        private System.Windows.Forms.TextBox txtGroup;
        private System.Windows.Forms.Label lblGender;
        private System.Windows.Forms.RadioButton rbMale;
        private System.Windows.Forms.RadioButton rbFemale;
        private System.Windows.Forms.Label lblGrade;
        private System.Windows.Forms.TrackBar trackBarGrade;
        private System.Windows.Forms.GroupBox grpAddress;
        private System.Windows.Forms.TextBox txtApartment;
        private System.Windows.Forms.Label lblApartment;
        private System.Windows.Forms.TextBox txtHouse;
        private System.Windows.Forms.Label lblHouse;
        private System.Windows.Forms.TextBox txtStreet;
        private System.Windows.Forms.Label lblStreet;
        private System.Windows.Forms.TextBox txtIndex;
        private System.Windows.Forms.Label lblIndex;
        private System.Windows.Forms.TextBox txtCity;
        private System.Windows.Forms.Label lblCity;
        private System.Windows.Forms.GroupBox grpWorkplace;
        private System.Windows.Forms.NumericUpDown numExperience;
        private System.Windows.Forms.Label lblExperience;
        private System.Windows.Forms.TextBox txtPosition;
        private System.Windows.Forms.Label lblPosition;
        private System.Windows.Forms.TextBox txtCompany;
        private System.Windows.Forms.Label lblCompany;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Button btnLoad;
        private System.Windows.Forms.Button btnCalculate;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.MonthCalendar writeDate;
        private System.Windows.Forms.ComboBox numCourse;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem findButton2;
        private System.Windows.Forms.ToolStripMenuItem сортировкаПоToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem btnSave2;
        private System.Windows.Forms.ToolStripMenuItem aboutBtn;
        private System.Windows.Forms.ToolStripButton findButton;
        private System.Windows.Forms.ToolStripButton btnDelete;
        private System.Windows.Forms.ToolStripButton clearWindow;
        private System.Windows.Forms.ToolStripButton btnPrev;
        private System.Windows.Forms.ToolStripButton btnNext;
        private System.Windows.Forms.ToolStripMenuItem fromalphabet;
        private System.Windows.Forms.ToolStripMenuItem toAlphabet;
        private System.Windows.Forms.TextBox findTextBox;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel lblObjectCount;
        private System.Windows.Forms.ToolStripStatusLabel lblLastAction;
        private System.Windows.Forms.ToolStripStatusLabel lblDateTime;
        private System.Windows.Forms.ToolStripMenuItem hideShowMenu;
        private System.Windows.Forms.Label aboutText;
    }
}

