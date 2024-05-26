#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
using namespace std;

struct item {
    char fname[30];
    char sname[30];
    char thname[30];
    int year;
    int course;
    int group;
    int marks[5];
    float middle_mark = 0;
    item* next;
    item* prev;
};

item* plist = nullptr;

void add() {
    char fname[30], sname[30], thname[30];
    cout << "������� ��� ��������: ";
    cin >> fname >> sname >> thname;
    int year;
    cout << "������� ��� ��������: ";
    cin >> year;
    int group, course, marks[5];
    cout << "������� ���� � ����� ������: ";
    cin >> course >> group;
    for (int i = 0; i < 5; i++) {
        cout << "������� ������ �� �������� " << i + 1 << ": ";
        cin >> marks[i];
        if (marks[i] < 0 || marks[i] > 10) {
            cout << "������ ������ ���� �� 0 �� 10. ��������� ����: ";
            cin >> marks[i];
        }
    }
    item* p = new item;
    strcpy(p->fname, fname);
    strcpy(p->sname, sname);
    strcpy(p->thname, thname);
    p->year = year;
    p->course = course;
    p->group = group;
    memcpy(p->marks, marks, sizeof(marks));
    for (int i = 0; i < 5; i++) {
        p->middle_mark += marks[i];
    }
    p->middle_mark /= 5;
    p->next = plist;
    p->prev = nullptr;
    if (plist != nullptr) {
        plist->prev = p;
    }
    plist = p;
    cout << "������� ��� ������� �������� � ������ ������." << endl;
}

void Sort() {
    item* current = plist;
    while (current != nullptr) {
        item* min_course = current;
        item* temp = current->next;
        while (temp != nullptr) {
            if (temp->course < min_course->course) {
                min_course = temp;
            }
            else if (temp->course == min_course->course) {
                if (strcmp(temp->fname, min_course->fname) < 0) {
                    min_course = temp;
                }
            }
            temp = temp->next;
        }
        swap(current->fname, min_course->fname);
        swap(current->sname, min_course->sname);
        swap(current->thname, min_course->thname);
        swap(current->year, min_course->year);
        swap(current->course, min_course->course);
        swap(current->group, min_course->group);
        swap(current->marks, min_course->marks);
        swap(current->middle_mark, min_course->middle_mark);
        current = current->next;
    }
    cout << "���������� ��������� �������!" << endl;
}

void write() {
    item* current = plist;
    while (current != nullptr) {
        cout << "���: " << current->fname << " " << current->sname << " " << current->thname << endl
            << "��� ��������: " << current->year << endl << "����: " << current->course << endl << "������: " << current->group << endl
            << "������: " << current->marks[0] << " " << current->marks[1] << " " << current->marks[2] << " " << current->marks[3] << " " << current->marks[4] << endl
            << "������� ����: " << current->middle_mark << endl << endl;
        current = current->next;
    }
}

void better_student() {
    item* current = plist;
    item* best_student = current;
    float max_mark = current->middle_mark;
    while (current != nullptr) {
        if (current->middle_mark > max_mark) {
            max_mark = current->middle_mark;
            best_student = current;
        }
        current = current->next;
    }
    if (best_student != nullptr) {
        cout << "������ ������� �� ������������: " << best_student->fname << " " << best_student->sname << " " << best_student->thname << " (" << max_mark << ")" << endl;
    }
    else {
        cout << "������ ������� �� ������������ �� ������." << endl;
    }

}

void max_min_year() {
    item* current = plist;
    item* oldest_student = current;
    item* youngest_student = current;
    int max_year = current->year;
    int min_year = current->year;
    while (current != nullptr) {
        if (current->year > max_year) {
            max_year = current->year;
            oldest_student = current;
        }
        if (current->year < min_year) {
            min_year = current->year;
            youngest_student = current;
        }
        current = current->next;
    }
    if (oldest_student != nullptr) {
        cout << "����� ������� �������: " << oldest_student->fname << " " << oldest_student->sname << " " << oldest_student->thname << " (" << max_year << ")" << endl;
    }
    else {
        cout << "����� ������� ������� �� ������." << endl;
    }

    if (youngest_student != nullptr) {
        cout << "����� ������� �������: " << youngest_student->fname << " " << youngest_student->sname << " " << youngest_student->thname << " (" << min_year << ")" << endl;
    }
    else {
        cout << "����� ������� ������� �� ������." << endl;
    }

}

void middle_groups() {
    float sum_marks[10][5] = { 0 };
    int count_students[10] = { 0 }; 

    item* current = plist;
    while (current != nullptr) {
        for (int i = 0; i < 5; ++i) {
            sum_marks[current->group - 1][i] += current->marks[i];
        }
        count_students[current->group - 1]++;
        current = current->next;
    }

    cout << "������� ���� ������ ������ �� ������� ��������:" << endl;
    for (int i = 0; i < 10; ++i) {
        if (count_students[i] != 0) {
            cout << "������ " << i + 1 << ":" << endl;
            for (int j = 0; j < 5; ++j) {
                cout << "������� " << j + 1 << ": " << sum_marks[i][j] / count_students[i] << endl;
            }
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_CTYPE, "Rus");
    int c;
    do {
        cout << "����:" << endl;
        cout << "1 - �������� ��������" << endl;
        cout << "2 - ������� ���� ���������" << endl;
        cout << "3 - ���������� � ������� ������" << endl;
        cout << "4 - ������ ������� �� ������������" << endl;
        cout << "5 - ����� ������� � ������� �������" << endl;
        cout << "6 - ������� ���� �� �������" << endl;
        cout << "7 - �����" << endl;
        cout << endl << "��� �����: ";
        cin >> c;
        system("cls");

        switch (c) {
        case 1:
            add();
            break;
        case 2:
            write();
            break;
        case 3:
            Sort();
            break;
        case 4:
            better_student();
            break;
        case 5:
            max_min_year();
            break;
        case 6:
            middle_groups();
            break;
        case 7:
            cout << "�� ��������!";
            break;
        default:
            cout << "������������ ������, ��������� ����������." << endl;
            break;
        }

        cout << endl;
    } while (c != 7);

    return 0;
}
