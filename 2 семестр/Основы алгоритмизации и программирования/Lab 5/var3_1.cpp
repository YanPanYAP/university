#include <iostream>
#include <iomanip>
#include <windows.h>
#include <cstring>
using namespace std;

const int MAX_SIZE = 5;
int current_size = 0;

enum Subjects {
    МАТЕМАТИКА,
    ФИЗИКА,
    ЛИТЕРАТУРА,
    АНГЛИЙСКИЙ,
    ИСТОРИЯ,
    ХИМИЯ,
    NUM_SUBJECTS
};

struct Klass {
    unsigned num : 4;
    char letter;
};

struct Pupil {
    char surname[50];
    char name[50];
    char middlename[50];
    Klass klass;
    int marks[NUM_SUBJECTS];
    float average;
} pupils[MAX_SIZE];

struct Pupil empty_pupil;

void input();
void displayOneStructure(Pupil onePupil);
void output();
void del();
void find();

int main() {
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    short choose;
    do {
        cout << "Меню:" << endl;
        cout << "1 - Ввести новую запись" << endl;
        cout << "2 - Вывести информацию" << endl;
        cout << "3 - Удалить запись" << endl;
        cout << "4 - Поиск" << endl;
        cout << "5 - Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choose;

        system("cls");
        switch (choose)
        {
        case 1:
            input();
            break;
        case 2:
            output();
            break;
        case 3:
            del();
            break;
        case 4:
            find();
            break;
        case 5:
            cout << "Выход";
            break;
        default:
            cout << "Некорректные данные";
        }
    } while (choose != 5);
    return 0;
}

void input() {
    cout << "Ввод информации" << endl;
    cout << "Номер записи " << current_size + 1 << endl;

    if (current_size < MAX_SIZE) {
        cout << "Фамилия: ";
        cin >> pupils[current_size].surname;

        cout << "Имя: ";
        cin >> pupils[current_size].name;

        cout << "Отчество: ";
        cin >> pupils[current_size].middlename;

        cout << "Класс (номер): ";
        int klass_num;
        cin >> klass_num;
        if (klass_num < 1 or klass_num > 11) {
            cout << "Неправильный ввод" << endl;
            return;
        }
        pupils[current_size].klass.num = klass_num;

        cout << "Класс (буква): ";
        cin >> pupils[current_size].klass.letter;

        cout << "Оценки (матем, физика, литература, английский, история, химия):" << endl;
        for (int i = 0; i < NUM_SUBJECTS; i++) {
            cout << "Оценка по предмету " << i + 1 << ": ";
            cin >> pupils[current_size].marks[i];
        }

        float sum = 0;
        for (int i = 0; i < NUM_SUBJECTS; i++) {
            sum += pupils[current_size].marks[i];
        }
        pupils[current_size].average = sum / NUM_SUBJECTS;

        current_size++;
    }
    else {
        cout << "Никакие другие записи не могут быть введены" << endl;
    }

    system("cls");
}

void displayOneStructure(Pupil onePupil) {
    cout << "Фамилия: " << onePupil.surname << endl;
    cout << "Имя: " << onePupil.name << endl;
    cout << "Отчество: " << onePupil.middlename << endl;
    cout << "Класс (номер): " << onePupil.klass.num << endl;
    cout << "Класс (буква): " << onePupil.klass.letter << endl;
    cout << "Оценки: " << endl;
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        cout << "Предмет " << i + 1 << ": " << onePupil.marks[i] << endl;
    }
    cout << "Средний балл: " << fixed << setprecision(2) << onePupil.average << endl << endl;
}

void output() {
    cout << "Вывод информации" << endl;
    if (current_size == 0) {
        cout << "Данных об учениках нет." << endl;
        return;
    }
    for (int i = 0; i < current_size; i++) {
        displayOneStructure(pupils[i]);
    }
}

void del() {
    cout << "Удаление записи" << endl;
    int rec_number;
    cout << "Введите номер записи для удаления: ";
    cin >> rec_number;

    if (rec_number < 1 || rec_number > current_size) {
        cout << "Некорректный номер записи" << endl << endl;
        return;
    }

    pupils[rec_number - 1] = empty_pupil;

    cout << "Запись с номером " << rec_number << " удалена" << endl;
}

void find() {
    cout << "Поиск записи" << endl;
    char surname[50];
    cout << "Введите фамилию для поиска: ";
    cin >> surname;
    cout << endl;
    bool found = false;
    for (int i = 0; i < current_size; i++) {
        if (strcmp(pupils[i].surname, surname) == 0) {
            found = true;
            displayOneStructure(pupils[i]);
        }
    }
    if (!found) {
        cout << "Нет данных об ученике с фамилией " << surname << endl;
    }
}
