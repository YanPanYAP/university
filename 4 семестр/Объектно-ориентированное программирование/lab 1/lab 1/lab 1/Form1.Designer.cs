namespace lab_1
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
            this.inputText = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.replaceButton = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.fromReplace = new System.Windows.Forms.TextBox();
            this.toReplace = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.deleteTextArea = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.indxArea = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.lengthResualt = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.gCount = new System.Windows.Forms.Label();
            this.sCount = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.wordCount = new System.Windows.Forms.Label();
            this.resultReplace = new System.Windows.Forms.Label();
            this.resultDelete = new System.Windows.Forms.Label();
            this.deleteButton = new System.Windows.Forms.Button();
            this.indexAnswer = new System.Windows.Forms.Label();
            this.clearButton = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.sentenceCount = new System.Windows.Forms.Label();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // inputText
            // 
            this.inputText.Location = new System.Drawing.Point(113, 215);
            this.inputText.Name = "inputText";
            this.inputText.Size = new System.Drawing.Size(442, 31);
            this.inputText.TabIndex = 0;
            this.inputText.TextChanged += new System.EventHandler(this.inputText_TextChanged);
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(0, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(971, 109);
            this.label1.TabIndex = 1;
            this.label1.Text = "ТЕКСТОВЫЙ КАЛЬКУЛЯТОР";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // replaceButton
            // 
            this.replaceButton.Location = new System.Drawing.Point(634, 395);
            this.replaceButton.Name = "replaceButton";
            this.replaceButton.Size = new System.Drawing.Size(158, 45);
            this.replaceButton.TabIndex = 2;
            this.replaceButton.Text = "Заменить";
            this.replaceButton.UseVisualStyleBackColor = true;
            this.replaceButton.Click += new System.EventHandler(this.replaceButton_Click);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.Coral;
            this.panel1.Controls.Add(this.label1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(971, 109);
            this.panel1.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.label2.Location = new System.Drawing.Point(106, 174);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(253, 38);
            this.label2.TabIndex = 0;
            this.label2.Text = "Введите строку";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.label3.Location = new System.Drawing.Point(106, 292);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(462, 38);
            this.label3.TabIndex = 4;
            this.label3.Text = "Замена на другую подстроку";
            // 
            // fromReplace
            // 
            this.fromReplace.Location = new System.Drawing.Point(182, 345);
            this.fromReplace.Name = "fromReplace";
            this.fromReplace.Size = new System.Drawing.Size(373, 31);
            this.fromReplace.TabIndex = 5;
            this.fromReplace.TextChanged += new System.EventHandler(this.fromReplace_TextChanged);
            // 
            // toReplace
            // 
            this.toReplace.Location = new System.Drawing.Point(182, 397);
            this.toReplace.Name = "toReplace";
            this.toReplace.Size = new System.Drawing.Size(373, 31);
            this.toReplace.TabIndex = 6;
            this.toReplace.TextChanged += new System.EventHandler(this.toReplace_TextChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.label4.Location = new System.Drawing.Point(106, 338);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 38);
            this.label4.TabIndex = 7;
            this.label4.Text = "с";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.label5.Location = new System.Drawing.Point(106, 390);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(54, 38);
            this.label5.TabIndex = 8;
            this.label5.Text = "на";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.label6.Location = new System.Drawing.Point(106, 517);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(331, 38);
            this.label6.TabIndex = 9;
            this.label6.Text = "Удаление подстроки";
            // 
            // deleteTextArea
            // 
            this.deleteTextArea.Location = new System.Drawing.Point(113, 571);
            this.deleteTextArea.Name = "deleteTextArea";
            this.deleteTextArea.Size = new System.Drawing.Size(442, 31);
            this.deleteTextArea.TabIndex = 10;
            this.deleteTextArea.TextChanged += new System.EventHandler(this.deleteTextArea_TextChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.label7.Location = new System.Drawing.Point(106, 685);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(232, 38);
            this.label7.TabIndex = 11;
            this.label7.Text = "Найти символ";
            // 
            // indxArea
            // 
            this.indxArea.Location = new System.Drawing.Point(192, 740);
            this.indxArea.Name = "indxArea";
            this.indxArea.Size = new System.Drawing.Size(295, 31);
            this.indxArea.TabIndex = 12;
            this.indxArea.TextChanged += new System.EventHandler(this.indxArea_TextChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.label8.Location = new System.Drawing.Point(106, 735);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(80, 38);
            this.label8.TabIndex = 13;
            this.label8.Text = "indx";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.label9.Location = new System.Drawing.Point(106, 830);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(237, 38);
            this.label9.TabIndex = 14;
            this.label9.Text = "Длина строки:";
            this.label9.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // lengthResualt
            // 
            this.lengthResualt.AutoSize = true;
            this.lengthResualt.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lengthResualt.ForeColor = System.Drawing.Color.Black;
            this.lengthResualt.Location = new System.Drawing.Point(520, 830);
            this.lengthResualt.Name = "lengthResualt";
            this.lengthResualt.Size = new System.Drawing.Size(35, 38);
            this.lengthResualt.TabIndex = 15;
            this.lengthResualt.Text = "0";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label12.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.label12.Location = new System.Drawing.Point(106, 884);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(345, 38);
            this.label12.TabIndex = 17;
            this.label12.Text = "Количество гласных:";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label13.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.label13.Location = new System.Drawing.Point(106, 937);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(381, 38);
            this.label13.TabIndex = 18;
            this.label13.Text = "Количество согласных:";
            // 
            // gCount
            // 
            this.gCount.AutoSize = true;
            this.gCount.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gCount.ForeColor = System.Drawing.Color.Black;
            this.gCount.Location = new System.Drawing.Point(520, 884);
            this.gCount.Name = "gCount";
            this.gCount.Size = new System.Drawing.Size(35, 38);
            this.gCount.TabIndex = 19;
            this.gCount.Text = "0";
            // 
            // sCount
            // 
            this.sCount.AutoSize = true;
            this.sCount.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.sCount.ForeColor = System.Drawing.Color.Black;
            this.sCount.Location = new System.Drawing.Point(520, 937);
            this.sCount.Name = "sCount";
            this.sCount.Size = new System.Drawing.Size(35, 38);
            this.sCount.TabIndex = 20;
            this.sCount.Text = "0";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label15.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.label15.Location = new System.Drawing.Point(106, 990);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(286, 38);
            this.label15.TabIndex = 21;
            this.label15.Text = "Количество слов:";
            // 
            // wordCount
            // 
            this.wordCount.AutoSize = true;
            this.wordCount.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.wordCount.ForeColor = System.Drawing.Color.Black;
            this.wordCount.Location = new System.Drawing.Point(520, 990);
            this.wordCount.Name = "wordCount";
            this.wordCount.Size = new System.Drawing.Size(35, 38);
            this.wordCount.TabIndex = 22;
            this.wordCount.Text = "0";
            // 
            // resultReplace
            // 
            this.resultReplace.AutoSize = true;
            this.resultReplace.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.resultReplace.ForeColor = System.Drawing.Color.Black;
            this.resultReplace.Location = new System.Drawing.Point(106, 453);
            this.resultReplace.Name = "resultReplace";
            this.resultReplace.Size = new System.Drawing.Size(0, 38);
            this.resultReplace.TabIndex = 23;
            // 
            // resultDelete
            // 
            this.resultDelete.AutoSize = true;
            this.resultDelete.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.resultDelete.ForeColor = System.Drawing.Color.Black;
            this.resultDelete.Location = new System.Drawing.Point(106, 619);
            this.resultDelete.Name = "resultDelete";
            this.resultDelete.Size = new System.Drawing.Size(0, 38);
            this.resultDelete.TabIndex = 24;
            // 
            // deleteButton
            // 
            this.deleteButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.875F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.deleteButton.Location = new System.Drawing.Point(634, 569);
            this.deleteButton.Name = "deleteButton";
            this.deleteButton.Size = new System.Drawing.Size(158, 43);
            this.deleteButton.TabIndex = 25;
            this.deleteButton.Text = "Удалить";
            this.deleteButton.UseVisualStyleBackColor = true;
            this.deleteButton.Click += new System.EventHandler(this.deleteButton_Click);
            // 
            // indexAnswer
            // 
            this.indexAnswer.AutoSize = true;
            this.indexAnswer.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.indexAnswer.ForeColor = System.Drawing.Color.Black;
            this.indexAnswer.Location = new System.Drawing.Point(520, 738);
            this.indexAnswer.Name = "indexAnswer";
            this.indexAnswer.Size = new System.Drawing.Size(0, 38);
            this.indexAnswer.TabIndex = 28;
            this.indexAnswer.Click += new System.EventHandler(this.indexAnswer_Click);
            // 
            // clearButton
            // 
            this.clearButton.Location = new System.Drawing.Point(634, 213);
            this.clearButton.Name = "clearButton";
            this.clearButton.Size = new System.Drawing.Size(158, 46);
            this.clearButton.TabIndex = 29;
            this.clearButton.Text = "Очистить";
            this.clearButton.UseVisualStyleBackColor = true;
            this.clearButton.Click += new System.EventHandler(this.clearButton_Click);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.label10.Location = new System.Drawing.Point(106, 1042);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(239, 38);
            this.label10.TabIndex = 30;
            this.label10.Text = "Предложений:";
            // 
            // sentenceCount
            // 
            this.sentenceCount.AutoSize = true;
            this.sentenceCount.Font = new System.Drawing.Font("Arial Black", 10.125F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.sentenceCount.ForeColor = System.Drawing.Color.Black;
            this.sentenceCount.Location = new System.Drawing.Point(520, 1042);
            this.sentenceCount.Name = "sentenceCount";
            this.sentenceCount.Size = new System.Drawing.Size(35, 38);
            this.sentenceCount.TabIndex = 31;
            this.sentenceCount.Text = "0";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.ClientSize = new System.Drawing.Size(971, 1175);
            this.Controls.Add(this.sentenceCount);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.clearButton);
            this.Controls.Add(this.indexAnswer);
            this.Controls.Add(this.deleteButton);
            this.Controls.Add(this.resultDelete);
            this.Controls.Add(this.resultReplace);
            this.Controls.Add(this.wordCount);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.sCount);
            this.Controls.Add(this.gCount);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.lengthResualt);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.indxArea);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.deleteTextArea);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.toReplace);
            this.Controls.Add(this.fromReplace);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.replaceButton);
            this.Controls.Add(this.inputText);
            this.Name = "Form1";
            this.Text = "Form1";
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox inputText;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button replaceButton;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox fromReplace;
        private System.Windows.Forms.TextBox toReplace;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox deleteTextArea;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox indxArea;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label lengthResualt;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label gCount;
        private System.Windows.Forms.Label sCount;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label wordCount;
        private System.Windows.Forms.Label resultReplace;
        private System.Windows.Forms.Label resultDelete;
        private System.Windows.Forms.Button deleteButton;
        private System.Windows.Forms.Label indexAnswer;
        private System.Windows.Forms.Button clearButton;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label sentenceCount;
    }
}

