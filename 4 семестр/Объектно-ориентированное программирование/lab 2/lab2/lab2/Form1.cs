using System;
using System.IO;
using System.Linq;
using System.Text.Json;
using System.Windows.Forms;

namespace lab2
{
    public partial class Form1 : Form
    {
        private Student student = new Student();

        public Form1()
        {
            InitializeComponent();

            txtFullName.KeyPress += txtFullName_KeyPress;
            txtSpecialty.KeyPress += txtSpecialty_KeyPress;
            txtGroup.KeyPress += txtGroup_KeyPress;
            txtIndex.KeyPress += txtIndex_KeyPress;
            txtHouse.KeyPress += txtHouse_KeyPress;
            txtApartment.KeyPress += txtApartment_KeyPress;
            txtCity.KeyPress += txtCity_KeyPress;
            txtStreet.KeyPress += txtStreet_KeyPress;
            txtCompany.KeyPress += txtCompany_KeyPress;
            txtPosition.KeyPress += txtPosition_KeyPress;
        }

        private void txtFullName_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsLetter(e.KeyChar) && !char.IsWhiteSpace(e.KeyChar) && e.KeyChar != '\b')
            {
                e.Handled = true;
            }
        }

        private void txtSpecialty_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsLetter(e.KeyChar) && e.KeyChar != '\b')
            {
                e.Handled = true;
            }
        }

        private void txtGroup_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsLetterOrDigit(e.KeyChar) && e.KeyChar != '\b')
            {
                e.Handled = true;
            }
        }

        private void txtIndex_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsDigit(e.KeyChar) && e.KeyChar != '\b')
            {
                e.Handled = true;
            }
        }

        private void txtHouse_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsDigit(e.KeyChar) && e.KeyChar != '\b')
            {
                e.Handled = true;
            }
        }

        private void txtApartment_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsDigit(e.KeyChar) && e.KeyChar != '\b')
            {
                e.Handled = true;
            }
        }

        private void txtCity_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsLetter(e.KeyChar) && !char.IsWhiteSpace(e.KeyChar) && e.KeyChar != '\b')
            {
                e.Handled = true;
            }
        }

        private void txtStreet_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsLetter(e.KeyChar) && !char.IsWhiteSpace(e.KeyChar) && e.KeyChar != '\b')
            {
                e.Handled = true;
            }
        }

        private void txtCompany_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsLetter(e.KeyChar) && !char.IsWhiteSpace(e.KeyChar) && e.KeyChar != '\b')
            {
                e.Handled = true;
            }
        }

        private void txtPosition_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsLetter(e.KeyChar) && !char.IsWhiteSpace(e.KeyChar) && e.KeyChar != '\b')
            {
                e.Handled = true;
            }
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            if (numCourse.SelectedItem != null && int.TryParse(numCourse.SelectedItem.ToString(), out int selectedCourse))
            {
                student.Course = selectedCourse;
            }
            else
            {
                MessageBox.Show("⚠️ Выберите курс!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (writeDate.SelectionStart > DateTime.Today)
            {
                MessageBox.Show("⚠️ Дата записи не может быть в будущем!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }


            if (!ValidateInput())
                return;

            student.FullName = txtFullName.Text;
            student.Age = (int)numAge.Value;
            student.Specialty = txtSpecialty.Text;
            student.BirthDate = dtpBirthDate.Value;
            student.Group = txtGroup.Text;
            student.AverageGrade = trackBarGrade.Value;
            student.Gender = rbMale.Checked ? "Мужской" : "Женский";
            student.Address = new Address(txtCity.Text, txtIndex.Text, txtStreet.Text, txtHouse.Text, txtApartment.Text);
            student.Workplace = new Workplace(txtCompany.Text, txtPosition.Text, (int)numExperience.Value);
            student.WriteDate = writeDate.SelectionStart;

            SaveToJson(student);
            MessageBox.Show("✅ Данные успешно сохранены!", "Успех", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            student = LoadFromJson();
            if (student != null)
            {
                txtFullName.Text = student.FullName;
                numAge.Value = student.Age;
                txtSpecialty.Text = student.Specialty;
                dtpBirthDate.Value = student.BirthDate;
                numCourse.SelectedItem = student.Course.ToString();
                txtGroup.Text = student.Group;
                trackBarGrade.Value = student.AverageGrade;
                rbMale.Checked = student.Gender == "Мужской";
                rbFemale.Checked = student.Gender == "Женский";
                writeDate.SetDate(student.WriteDate);

                if (student.Address != null)
                {
                    txtCity.Text = student.Address.City;
                    txtIndex.Text = student.Address.Index;
                    txtStreet.Text = student.Address.Street;
                    txtHouse.Text = student.Address.House;
                    txtApartment.Text = student.Address.Apartment;
                }

                if (student.Workplace != null)
                {
                    txtCompany.Text = student.Workplace.Company;
                    txtPosition.Text = student.Workplace.Position;
                    numExperience.Value = student.Workplace.Experience;
                }

                MessageBox.Show("✅ Данные успешно загружены!", "Успех", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                MessageBox.Show("⚠️ Не удалось загрузить данные. Проверьте файл!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void btnCalculate_Click(object sender, EventArgs e)
        {
            if (student == null)
            {
                MessageBox.Show("⚠️ Сначала загрузите или сохраните данные студента!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            decimal budget = student.CalculateUniversityBudget();
            MessageBox.Show($"💰 Рассчитанный бюджет университета: {budget} руб.", "Бюджет", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void SaveToJson(Student student)
        {
            try
            {
                string json = JsonSerializer.Serialize(student, new JsonSerializerOptions { WriteIndented = true });
                File.WriteAllText("student.json", json);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка сохранения: {ex.Message}", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private Student LoadFromJson()
        {
            try
            {
                if (File.Exists("student.json"))
                {
                    string json = File.ReadAllText("student.json");
                    return JsonSerializer.Deserialize<Student>(json);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка загрузки: {ex.Message}", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            return null;
        }

        private bool ValidateInput()
        {
            if (string.IsNullOrWhiteSpace(txtFullName.Text) ||
            txtFullName.Text.Length < 5 ||
            txtFullName.Text.Trim().Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries).Length != 3)
            {
                MessageBox.Show("⚠️ Введите корректное ФИО!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }

            if (numAge.Value < 16)
            {
                MessageBox.Show("⚠️ Возраст должен быть не менее 16 лет!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }
            if (string.IsNullOrWhiteSpace(txtSpecialty.Text))
            {
                MessageBox.Show("⚠️ Введите специальность!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }
            if (string.IsNullOrWhiteSpace(txtGroup.Text))
            {
                MessageBox.Show("⚠️ Введите группу!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }
            if (trackBarGrade.Value < 2)
            {
                MessageBox.Show("⚠️ Укажите средний балл!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }
            if (!rbMale.Checked && !rbFemale.Checked)
            {
                MessageBox.Show("⚠️ Выберите пол!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }

            int calculatedAge = DateTime.Today.Year - dtpBirthDate.Value.Year;
            if (dtpBirthDate.Value > DateTime.Today.AddYears(-calculatedAge))
            {
                calculatedAge--;
            }

            if (calculatedAge != numAge.Value)
            {
                MessageBox.Show("⚠️ Возраст не соответствует дате рождения!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }


            if (txtIndex.Text.Length != 6 || !txtIndex.Text.All(char.IsDigit))
            {
                MessageBox.Show("⚠️ Индекс должен содержать ровно 6 цифр!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }

            if (numExperience.Value>numAge.Value-16)
            {
                MessageBox.Show("⚠️ Стаж слишком большой!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }

            if (string.IsNullOrWhiteSpace(txtCity.Text) ||
                string.IsNullOrWhiteSpace(txtIndex.Text) ||
                string.IsNullOrWhiteSpace(txtStreet.Text) ||
                string.IsNullOrWhiteSpace(txtHouse.Text) ||
                string.IsNullOrWhiteSpace(txtApartment.Text))
            {
                MessageBox.Show("⚠️ Заполните все поля адреса!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }

            if (string.IsNullOrWhiteSpace(txtCompany.Text) ||
                string.IsNullOrWhiteSpace(txtPosition.Text) ||
                numExperience.Value < 0)
            {
                MessageBox.Show("⚠️ Заполните все поля места работы!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }

            return true;
        }


        public class Student
        {
            public string FullName { get; set; }
            public int Age { get; set; }
            public string Specialty { get; set; }
            public DateTime BirthDate { get; set; }
            public int Course { get; set; }
            public string Group { get; set; }
            public int AverageGrade { get; set; }
            public string Gender { get; set; }
            public Address Address { get; set; }
            public Workplace Workplace { get; set; }
            public DateTime WriteDate { get; set; }

            public decimal CalculateUniversityBudget()
            {
                decimal baseBudget = 1000000;
                decimal bonusPerStudent = AverageGrade * 1000;
                decimal stipendCost = (AverageGrade >= 4) ? 50000 : 0;
                return baseBudget + bonusPerStudent - stipendCost;
            }
        }

        public class Workplace
        {
            public string Company { get; set; }
            public string Position { get; set; }
            public int Experience { get; set; }

            public Workplace() { }

            public Workplace(string company, string position, int experience)
            {
                Company = company;
                Position = position;
                Experience = experience;
            }
        }

        public class Address
        {
            public string City { get; set; }
            public string Index { get; set; }
            public string Street { get; set; }
            public string House { get; set; }
            public string Apartment { get; set; }

            public Address() { }

            public Address(string city, string index, string street, string house, string apartment)
            {
                City = city;
                Index = index;
                Street = street;
                House = house;
                Apartment = apartment;
            }
        }
    }
}
