#include <iostream>
#include <iomanip>
#include <windows.h>
#include <cstring>
using namespace std;

const int MAX_SIZE = 5;
int current_size = 0;

enum Subjects {
    ����������,
    ������,
    ����������,
    ����������,
    �������,
    �����,
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
        cout << "����:" << endl;
        cout << "1 - ������ ����� ������" << endl;
        cout << "2 - ������� ����������" << endl;
        cout << "3 - ������� ������" << endl;
        cout << "4 - �����" << endl;
        cout << "5 - �����" << endl;
        cout << "��� �����: ";
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
            cout << "�����";
            break;
        default:
            cout << "������������ ������";
        }
    } while (choose != 5);
    return 0;
}

void input() {
    cout << "���� ����������" << endl;
    cout << "����� ������ " << current_size + 1 << endl;

    if (current_size < MAX_SIZE) {
        cout << "�������: ";
        cin >> pupils[current_size].surname;

        cout << "���: ";
        cin >> pupils[current_size].name;

        cout << "��������: ";
        cin >> pupils[current_size].middlename;

        cout << "����� (�����): ";
        int klass_num;
        cin >> klass_num;
        if (klass_num < 1 or klass_num > 11) {
            cout << "������������ ����" << endl;
            return;
        }
        pupils[current_size].klass.num = klass_num;

        cout << "����� (�����): ";
        cin >> pupils[current_size].klass.letter;

        cout << "������ (�����, ������, ����������, ����������, �������, �����):" << endl;
        for (int i = 0; i < NUM_SUBJECTS; i++) {
            cout << "������ �� �������� " << i + 1 << ": ";
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
        cout << "������� ������ ������ �� ����� ���� �������" << endl;
    }

    system("cls");
}

void displayOneStructure(Pupil onePupil) {
    cout << "�������: " << onePupil.surname << endl;
    cout << "���: " << onePupil.name << endl;
    cout << "��������: " << onePupil.middlename << endl;
    cout << "����� (�����): " << onePupil.klass.num << endl;
    cout << "����� (�����): " << onePupil.klass.letter << endl;
    cout << "������: " << endl;
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        cout << "������� " << i + 1 << ": " << onePupil.marks[i] << endl;
    }
    cout << "������� ����: " << fixed << setprecision(2) << onePupil.average << endl << endl;
}

void output() {
    cout << "����� ����������" << endl;
    if (current_size == 0) {
        cout << "������ �� �������� ���." << endl;
        return;
    }
    for (int i = 0; i < current_size; i++) {
        displayOneStructure(pupils[i]);
    }
}

void del() {
    cout << "�������� ������" << endl;
    int rec_number;
    cout << "������� ����� ������ ��� ��������: ";
    cin >> rec_number;

    if (rec_number < 1 || rec_number > current_size) {
        cout << "������������ ����� ������" << endl << endl;
        return;
    }

    pupils[rec_number - 1] = empty_pupil;

    cout << "������ � ������� " << rec_number << " �������" << endl;
}

void find() {
    cout << "����� ������" << endl;
    char surname[50];
    cout << "������� ������� ��� ������: ";
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
        cout << "��� ������ �� ������� � �������� " << surname << endl;
    }
}
