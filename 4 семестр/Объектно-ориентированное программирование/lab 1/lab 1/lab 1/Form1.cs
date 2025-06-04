using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lab_1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            inputText.KeyPress += inputText_KeyPress;
            fromReplace.KeyPress += inputText_KeyPress;
            toReplace.KeyPress += inputText_KeyPress;
            deleteTextArea.KeyPress += inputText_KeyPress;
            indxArea.KeyPress += indxArea_KeyPress;
            indxArea.TextChanged += indxArea_TextChanged;
            replaceButton.Click += replaceButton_Click;
            deleteButton.Click += deleteButton_Click;
        }

        private void inputText_KeyPress(object sender, KeyPressEventArgs e)
        {

        }

        private void indxArea_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsDigit(e.KeyChar) && e.KeyChar != '\b')
            {
                e.Handled = true;
            }
        }

        private void inputText_TextChanged(object sender, EventArgs e)
        {
            string input = inputText.Text.Trim();

            lengthResualt.Text = input.Length.ToString();

            int vowels = 0, consonants = 0;
            string vowelsList = "аеёиоуыэюяaeiouyАЕЁИОУЫЭЮЯAEIOUY";
            string consonantsList = "бвгджзйклмнпрстфхцчшщBCDFGHJKLMNPQRSTVWXYZbcdfghjklmnpqrstvwxyz";

            foreach (char c in input)
            {
                if (vowelsList.Contains(c))
                {
                    vowels++;
                }
                else if (consonantsList.Contains(c))
                {
                    consonants++;
                }
            }

            gCount.Text = vowels.ToString();
            sCount.Text = consonants.ToString();

            string[] words = input.Split(new char[] { ' ', '\t', '\n' }, StringSplitOptions.RemoveEmptyEntries);
            wordCount.Text = words.Length.ToString();

            string[] sentences = System.Text.RegularExpressions.Regex.Split(input, @"(?<=[.!?])\s+");
            int sentenceCountValue = sentences.Count(s => !string.IsNullOrWhiteSpace(s));

            sentenceCount.Text = sentenceCountValue.ToString();
        }

        private void fromReplace_TextChanged(object sender, EventArgs e)
        {

        }

        private void toReplace_TextChanged(object sender, EventArgs e)
        {

        }

        private void deleteTextArea_TextChanged(object sender, EventArgs e)
        {

        }

        private void indxArea_TextChanged(object sender, EventArgs e)
        {
            if (string.IsNullOrWhiteSpace(indxArea.Text))
            {
                indexAnswer.Text = string.Empty;
                return;
            }

            if (int.TryParse(indxArea.Text, out int index))
            {
                if (inputText.Text.Length > 0 && index >= 0 && index < inputText.Text.Length)
                {
                    indexAnswer.Text = inputText.Text[index].ToString();
                }
                else
                {
                    indexAnswer.Text = "-1";
                }
            }
            else
            {
                indexAnswer.Text = "-1";
            }
        }

        private void replaceButton_Click(object sender, EventArgs e)
        {
            string input = inputText.Text;
            string from = fromReplace.Text;
            string to = toReplace.Text;

            if (string.IsNullOrEmpty(from))
            {
                resultReplace.Text = "-1";
                return;
            }

            if (input.Contains(from))
            {
                resultReplace.Text = input.Replace(from, to);
            }
            else
            {
                resultReplace.Text = "-1";
            }
        }

        private void deleteButton_Click(object sender, EventArgs e)
        {
            string input = inputText.Text;
            string toDelete = deleteTextArea.Text;

            if (string.IsNullOrEmpty(toDelete))
            {
                resultDelete.Text = "-1";
                return;
            }

            if (input.Contains(toDelete))
            {
                resultDelete.Text = input.Replace(toDelete, "");
            }
            else
            {
                resultDelete.Text = "-1";
            }
        }

        private void clearButton_Click(object sender, EventArgs e)
        {
            inputText.Text = string.Empty;
            resultReplace.Text = string.Empty;
            resultDelete.Text = string.Empty;
            indexAnswer.Text = string.Empty;
            indxArea.Text = string.Empty;
            deleteTextArea.Text = string.Empty;
            fromReplace.Text = string.Empty;
            toReplace.Text = string.Empty;
        }

        private void indexAnswer_Click(object sender, EventArgs e)
        {

        }
    }
}
