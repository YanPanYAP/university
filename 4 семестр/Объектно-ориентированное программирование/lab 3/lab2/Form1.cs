using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.Json;
using System.Windows.Forms;
using System.ComponentModel.DataAnnotations;
using System.Text.RegularExpressions;
using static lab2.Form1;

namespace lab2
{
    public partial class Form1 : Form
    {
        private Student student = new Student();

        public Form1()
        {
            InitializeComponent();

            //txtFullName.KeyPress += txtFullName_KeyPress;
            //txtSpecialty.KeyPress += txtSpecialty_KeyPress;
            //txtGroup.KeyPress += txtGroup_KeyPress;
            //txtIndex.KeyPress += txtIndex_KeyPress;
            //txtHouse.KeyPress += txtHouse_KeyPress;
            //txtApartment.KeyPress += txtApartment_KeyPress;
            //txtCity.KeyPress += txtCity_KeyPress;
            //txtStreet.KeyPress += txtStreet_KeyPress;
            //txtCompany.KeyPress += txtCompany_KeyPress;
            //txtPosition.KeyPress += txtPosition_KeyPress;


            UpdateObjectCount();
            UpdateLastAction("Программа запущена");
            UpdateDateTime();

            Timer timer = new Timer();
            timer.Interval = 1000; 
            timer.Tick += Timer_Tick;
            timer.Start();
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            UpdateDateTime(); 
        }

        private void UpdateObjectCount()
        {
            lblObjectCount.Text = $"Объектов: {students.Count}";
        }

        private void UpdateLastAction(string action)
        {
            lblLastAction.Text = $"Последнее действие: {action}";
        }

        private void UpdateDateTime()
        {
            lblDateTime.Text = DateTime.Now.ToString("dd.MM.yyyy HH:mm:ss");
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

            if (!ValidateStudent(student))
                return;

            SaveToJson(student);
            UpdateObjectCount();
            UpdateLastAction("Данные сохранены");
            MessageBox.Show("✅ Данные успешно сохранены!", "Успех", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            LoadFromJson();
            ResetSearch();
            

            if (students.Count > 0)
            {
                currentIndex = 0;
                DisplayStudent(students[currentIndex]);
                UpdateObjectCount();
                UpdateLastAction("Данные загружены");

                MessageBox.Show("✅ Данные успешно загружены!", "Успех", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                MessageBox.Show("⚠️ Файл пуст или не найден!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
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

        private List<Student> students = new List<Student>();
        private int currentIndex = 0;

        private void SaveToJson(Student student)
        {
            try
            {
                if (File.Exists("students.json"))
                {
                    string existingJson = File.ReadAllText("students.json");
                    students = JsonSerializer.Deserialize<List<Student>>(existingJson) ?? new List<Student>();
                }

                // Добавляем нового студента
                students.Add(student);

                // Сохраняем обновленный список
                string json = JsonSerializer.Serialize(students, new JsonSerializerOptions { WriteIndented = true });
                File.WriteAllText("students.json", json);

                MessageBox.Show("✅ Данные успешно сохранены!", "Успех", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка сохранения: {ex.Message}", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void LoadFromJson()
        {
            try
            {
                if (File.Exists("students.json"))
                {
                    string json = File.ReadAllText("students.json");
                    students = JsonSerializer.Deserialize<List<Student>>(json) ?? new List<Student>();

                    if (students.Count > 0)
                    {
                        currentIndex = 0; // Загружаем первый объект
                        DisplayStudent(students[currentIndex]);
                    }
                    else
                    {
                        MessageBox.Show("⚠️ Файл пуст!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
                }
                else
                {
                    MessageBox.Show("⚠️ Файл не найден!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка загрузки: {ex.Message}", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnNext_Click(object sender, EventArgs e)
        {
            if (foundStudents.Count > 0) // Если есть найденные студенты
            {
                if (foundIndex < foundStudents.Count - 1)
                {
                    foundIndex++;
                    DisplayStudent(foundStudents[foundIndex]);
                }
                else
                {
                    MessageBox.Show("⚠️ Достигнут конец списка найденных студентов!", "Переключение", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else if (students.Count > 0) // Иначе листаем основной список
            {
                if (currentIndex < students.Count - 1)
                {
                    currentIndex++;
                    DisplayStudent(students[currentIndex]);
                }
                else
                {
                    MessageBox.Show("⚠️ Достигнут конец списка!", "Переключение", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                MessageBox.Show("⚠️ Список студентов пуст!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void btnPrev_Click(object sender, EventArgs e)
        {
            if (foundStudents.Count > 0) // Если есть найденные студенты
            {
                if (foundIndex > 0)
                {
                    foundIndex--;
                    DisplayStudent(foundStudents[foundIndex]);
                }
                else
                {
                    MessageBox.Show("⚠️ Достигнуто начало списка найденных студентов!", "Переключение", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else if (students.Count > 0) // Иначе листаем основной список
            {
                if (currentIndex > 0)
                {
                    currentIndex--;
                    DisplayStudent(students[currentIndex]);
                }
                else
                {
                    MessageBox.Show("⚠️ Достигнуто начало списка!", "Переключение", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                MessageBox.Show("⚠️ Список студентов пуст!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void DisplayStudent(Student student)
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
            [Required(ErrorMessage = "ФИО обязательно для заполнения")]
            [StringLength(100, MinimumLength = 5, ErrorMessage = "ФИО должно содержать от 5 символов")]
            [RegularExpression(@"^[А-Яа-яA-Za-z\s]+$", ErrorMessage = "ФИО может содержать только буквы и пробелы")]
            [ThreeWords(ErrorMessage = "ФИО должно содержать ровно 3 слова")]
            public string FullName { get; set; }

            [Range(16, 100, ErrorMessage = "Возраст должен быть от 16 до 100 лет")]
            public int Age { get; set; }

            [Required(ErrorMessage = "Специальность обязательна для заполнения")]
            [RegularExpression(@"^[А-Яа-яA-Za-z\s]+$", ErrorMessage = "Специальность может содержать только буквы")]
            public string Specialty { get; set; }

            [CustomDateValidation(ErrorMessage = "Дата рождения должна быть корректной и соответствовать возрасту")]
            public DateTime BirthDate { get; set; }

            [Required(ErrorMessage = "Курс обязателен для заполнения")]
            [Range(1, 4, ErrorMessage = "Курс должен быть от 1 до 4")]
            public int Course { get; set; }

            [RegularExpression(@"^[0-9]+$", ErrorMessage = "Группа может содержать только цифры")]
            public string Group { get; set; }

            public int AverageGrade { get; set; }

            [Required(ErrorMessage = "Пол обязателен для заполнения")]
            public string Gender { get; set; }

            [Required]
            public Address Address { get; set; }
            [Required]
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

        public class ThreeWordsAttribute : ValidationAttribute
        {
            protected override ValidationResult IsValid(object value, ValidationContext validationContext)
            {
                if (value is string fullName)
                {
                    var words = fullName.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);

                    if (words.Length == 3)
                    {
                        return ValidationResult.Success;
                    }
                }

                return new ValidationResult("ФИО должно содержать ровно 3 слова");
            }
        }

        public class CustomDateValidationAttribute : ValidationAttribute
        {
            protected override ValidationResult IsValid(object value, ValidationContext validationContext)
            {
                if (value is DateTime birthDate)
                {
                    if (birthDate > DateTime.Now)
                    {
                        return new ValidationResult("Дата рождения не может быть в будущем");
                    }

                    var student = (Student)validationContext.ObjectInstance;
                    int calculatedAge = DateTime.Today.Year - birthDate.Year;
                    if (birthDate > DateTime.Today.AddYears(-calculatedAge))
                    {
                        calculatedAge--;
                    }

                    if (calculatedAge != student.Age)
                    {
                        return new ValidationResult("Возраст не соответствует дате рождения");
                    }

                    return ValidationResult.Success;
                }

                return new ValidationResult("Некорректная дата рождения");
            }
        }

        private bool ValidateStudent(Student student)
        {
            var validationContext = new ValidationContext(student);
            var validationResults = new List<ValidationResult>();

            bool isValid = Validator.TryValidateObject(student, validationContext, validationResults, true);

            if (student.Address != null)
            {
                var addressContext = new ValidationContext(student.Address);
                var addressResults = new List<ValidationResult>();
                isValid &= Validator.TryValidateObject(student.Address, addressContext, addressResults, true);
                validationResults.AddRange(addressResults);
            }

            if (student.Workplace != null)
            {
                var workplaceContext = new ValidationContext(student.Workplace);
                var workplaceResults = new List<ValidationResult>();
                isValid &= Validator.TryValidateObject(student.Workplace, workplaceContext, workplaceResults, true);
                validationResults.AddRange(workplaceResults);
            }

            if (!isValid)
            {
                foreach (var validationResult in validationResults)
                {
                    MessageBox.Show(validationResult.ErrorMessage, "Ошибка валидации", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }

            return isValid;
        }

        public class Workplace
        {
            [RegularExpression(@"^[А-Яа-яA-Za-z\s]+$", ErrorMessage = "Компания может содержать только буквы")]
            public string Company { get; set; }
            [RegularExpression(@"^[А-Яа-яA-Za-z\s]+$", ErrorMessage = "Позиция может содержать только буквы")]
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
            [RegularExpression(@"^[А-Яа-яA-Za-z\s]+$", ErrorMessage = "Город может содержать только буквы")]
            public string City { get; set; }

            [RegularExpression(@"^\d{6}$", ErrorMessage = "Индекс должен содержать только цифры (6)")]
            public string Index { get; set; }
            [RegularExpression(@"^[А-Яа-яA-Za-z]+$", ErrorMessage = "Улица может содержать только буквы")]
            public string Street { get; set; }

            [RegularExpression(@"^[0-9]+$", ErrorMessage = "Номер дома может содержать только цифры")]
            public string House { get; set; }
            [RegularExpression(@"^[0-9]+$", ErrorMessage = "Номер квартиры может содержать только цифры")]
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

        private void clearWindow_Click(object sender, EventArgs e)
        {
            UpdateLastAction("Очистка");
            ClearWindow();
        }

        private void ClearWindow()
        {
            txtFullName.Clear();
            numAge.Value = numAge.Minimum;
            txtSpecialty.Clear();
            dtpBirthDate.Value = DateTime.Today;
            numCourse.SelectedItem = null;
            txtGroup.Clear();
            trackBarGrade.Value = trackBarGrade.Minimum;
            rbMale.Checked = false;
            rbFemale.Checked = false;
            writeDate.SetDate(DateTime.Today);

            txtCity.Clear();
            txtIndex.Clear();
            txtStreet.Clear();
            txtHouse.Clear();
            txtApartment.Clear();

            txtCompany.Clear();
            txtPosition.Clear();
            numExperience.Value = numExperience.Minimum;
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            if (students.Count == 0)
            {
                MessageBox.Show("⚠️ Список пуст, нечего удалять!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (currentIndex >= 0 && currentIndex < students.Count)
            {
                students.RemoveAt(currentIndex);

                try
                {
                    string json = JsonSerializer.Serialize(students, new JsonSerializerOptions { WriteIndented = true });
                    File.WriteAllText("students.json", json);

                    if (students.Count == 0)
                    {
                        ClearWindow();
                        UpdateObjectCount();
                        UpdateLastAction("Студент удален");
                        MessageBox.Show("✅ Все студенты удалены.", "Удалено", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                    {
                        if (currentIndex >= students.Count)
                        {
                            currentIndex = students.Count - 1;
                        }
                        UpdateObjectCount();
                        UpdateLastAction("Студент удален");
                        DisplayStudent(students[currentIndex]);
                        MessageBox.Show("✅ Студент успешно удален.", "Удалено", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }

                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Ошибка при сохранении изменений: {ex.Message}", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void toAlphabet_Click(object sender, EventArgs e)
        {
            if (students == null || students.Count == 0)
            {
                MessageBox.Show("⚠️ Файл не загружен или список пуст!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            UpdateLastAction("Сортировка по алфавиту");
            students = students.OrderBy(s => s.FullName).ToList();
            SaveSortedStudents();
            DisplayStudent(students.FirstOrDefault());
        }

        private void fromalphabet_Click(object sender, EventArgs e)
        {
            if (students == null || students.Count == 0)
            {
                MessageBox.Show("⚠️ Файл не загружен или список пуст!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            UpdateLastAction("Сортировка по алфавиту в обратном");
            students = students.OrderByDescending(s => s.FullName).ToList();
            SaveSortedStudents();
            DisplayStudent(students.FirstOrDefault());
        }

        private void SaveSortedStudents()
        {
            try
            {
                string json = JsonSerializer.Serialize(students, new JsonSerializerOptions { WriteIndented = true });
                File.WriteAllText("students.json", json);
                MessageBox.Show("✅ Список успешно отсортирован и сохранен.", "Успех", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при сохранении отсортированного списка: {ex.Message}", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private List<Student> foundStudents = new List<Student>(); 
        private int foundIndex = -1;

        //private void findButton_Click(object sender, EventArgs e)
        //{
        //    findTextBox.Focus(); 
        //    UpdateLastAction($"Выполнен поиск: {findTextBox.Text}");
        //    string searchText = findTextBox.Text.Trim().ToLower();

        //    if (string.IsNullOrEmpty(searchText))
        //    {
        //        MessageBox.Show("⚠️ Введите текст для поиска!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
        //        return;
        //    }

        //    if (students == null || students.Count == 0)
        //    {
        //        MessageBox.Show("⚠️ Файл не загружен или список пуст!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
        //        return;
        //    }

        //    foundStudents = students
        //        .Where(s => s.FullName.ToLower().Contains(searchText) ||
        //                    s.Specialty.ToLower().Contains(searchText) ||
        //                    s.Group.ToLower().Contains(searchText) ||
        //                    s.Address.City.ToLower().Contains(searchText) ||
        //                    s.Address.Street.ToLower().Contains(searchText) ||
        //                    s.Workplace.Company.ToLower().Contains(searchText) ||
        //                    s.Workplace.Position.ToLower().Contains(searchText))
        //        .ToList();

        //    if (foundStudents.Count == 0)
        //    {
        //        MessageBox.Show("⚠️ Ничего не найдено!", "Результат поиска", MessageBoxButtons.OK, MessageBoxIcon.Information);
        //        return;
        //    }

        //    foundIndex = 0;
        //    DisplayStudent(foundStudents[foundIndex]);
        //    MessageBox.Show($"✅ Найдено {foundStudents.Count} студентов.", "Результат поиска", MessageBoxButtons.OK, MessageBoxIcon.Information);
        //}

    private void findButton_Click(object sender, EventArgs e)
    {
        findTextBox.Focus();
        UpdateLastAction($"Выполнен поиск: {findTextBox.Text}");
        string searchText = findTextBox.Text.Trim().ToLower();

        if (string.IsNullOrEmpty(searchText))
        {
            MessageBox.Show("⚠️ Введите текст для поиска!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            return;
        }

        if (students == null || students.Count == 0)
        {
            MessageBox.Show("⚠️ Файл не загружен или список пуст!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            return;
        }

        Regex regex = new Regex(searchText, RegexOptions.IgnoreCase);

        foundStudents = students
            .Where(s => regex.IsMatch(s.FullName) ||
                        regex.IsMatch(s.Specialty) ||
                        regex.IsMatch(s.Group) ||
                        regex.IsMatch(s.Address.City) ||
                        regex.IsMatch(s.Address.Street) ||
                        regex.IsMatch(s.Workplace.Company) ||
                        regex.IsMatch(s.Workplace.Position))
            .ToList();

        if (foundStudents.Count == 0)
        {
            MessageBox.Show("⚠️ Ничего не найдено!", "Результат поиска", MessageBoxButtons.OK, MessageBoxIcon.Information);
            return;
        }

        foundIndex = 0;
        DisplayStudent(foundStudents[foundIndex]);
        MessageBox.Show($"✅ Найдено {foundStudents.Count} студентов.", "Результат поиска", MessageBoxButtons.OK, MessageBoxIcon.Information);
    }

    private void btnNextFound_Click(object sender, EventArgs e)
        {
            UpdateLastAction("Нажата кнопка вперед");
            if (foundStudents.Count == 0)
            {
                MessageBox.Show("⚠️ Нет найденных студентов!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (foundIndex < foundStudents.Count - 1)
            {
                foundIndex++;
                DisplayStudent(foundStudents[foundIndex]);
            }
            else
            {
                MessageBox.Show("⚠️ Достигнут конец списка найденных студентов!", "Переключение", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private void btnPrevFound_Click(object sender, EventArgs e)
        {
            UpdateLastAction("Нажата кнопка назад");
            if (foundStudents.Count == 0)
            {
                MessageBox.Show("⚠️ Нет найденных студентов!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (foundIndex > 0)
            {
                foundIndex--;
                DisplayStudent(foundStudents[foundIndex]);
            }
            else
            {
                MessageBox.Show("⚠️ Достигнуто начало списка найденных студентов!", "Переключение", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private void ResetSearch()
        {
            foundStudents.Clear();
            foundIndex = -1; 
        }

        private void findTextBox_TextChanged(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(findTextBox.Text))
            {
                ResetSearch();
            }
        }

        private void findButton2_Click(object sender, EventArgs e)
        {
            UpdateLastAction("Выполнен поиск");
            findButton_Click(sender, e);
        }

        private void hideShowMenu_Click(object sender, EventArgs e)
        {
            if (toolStrip1.Visible == true)
            {
                toolStrip1.Visible = false;
                UpdateLastAction("Меню скрыто");
            } else
            {
                toolStrip1.Visible = true;
                UpdateLastAction("Меню показано");
            }
                //toolStrip1.Visible = toolStrip1.Visible ? true : false; 
                hideShowMenu.Text = toolStrip1.Visible ? "Скрыть меню" : "Показать меню";
        }

        private void hide()
        {
            toolStrip1.Visible = false;
        }

        private void show()
        {
            toolStrip1.Visible = true;
        }

        private void aboutBtn_Click(object sender, EventArgs e)
        {
            aboutText.Text = "Выполнил Павлович Ян";
            UpdateLastAction("О программе");
        }

        private void btnSave2_Click(object sender, EventArgs e)
        {
            btnSave_Click(sender, e);
        }
    }
}
