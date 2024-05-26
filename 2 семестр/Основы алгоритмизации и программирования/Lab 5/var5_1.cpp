#include <iostream>
#include <iomanip>
#include <string>
#include <Windows.h>
using namespace std;

enum BookOrigin {
    �������,
    �����,
    �������
};

struct Date {
    unsigned int day;
    unsigned int month;
    unsigned int year;
};

struct Book {
    string author;
    string title;
    string publisher;
    string section;
    BookOrigin origin;
    bool available;
    Date acquisitionDate;
};

const int MAX_BOOKS = 100;
Book library[MAX_BOOKS];
int bookCount = 0;

void addBook() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Book newBook;
    cout << "������� ������: ";
    cin.ignore();
    getline(cin, newBook.author);
    cout << "������� ��������: ";
    getline(cin, newBook.title);
    cout << "������� ������������: ";
    getline(cin, newBook.publisher);
    cout << "������� ������: ";
    getline(cin, newBook.section);
    cout << "������� ������������� (0 ��� �������, 1 ��� �����, 2 ��� �������): ";
    int originChoice;
    cin >> originChoice;
    newBook.origin = static_cast<BookOrigin>(originChoice);
    cout << "�������� �� �����? (1 ��� ��, 0 ��� ���): ";
    cin >> newBook.available;
    cout << "������� ���� ������������ (�� �� ����): ";
    cin >> newBook.acquisitionDate.day >> newBook.acquisitionDate.month >> newBook.acquisitionDate.year;

    library[bookCount++] = newBook;
    system("cls");
}

void showBooks() {
    system("cls");
    cout << "����������:" << endl;
    cout << "�����\t��������\t������������\t������\t�������������\t��������\t���� ������������" << endl;
    for (int i = 0; i < bookCount; ++i) {
        cout << setw(5) << left << library[i].author << '\t'
            << setw(8) << left << library[i].title << '\t'
            << setw(12) << left << library[i].publisher << '\t'
            << setw(6) << left << library[i].section << '\t'
            << setw(13) << left << library[i].origin << '\t'
            << setw(8) << left << (library[i].available ? "��" : "���") << '\t'
            << library[i].acquisitionDate.day << "." << library[i].acquisitionDate.month << "." << library[i].acquisitionDate.year << '\t'
            << endl;
    }
    cout << endl;
}

void deleteBook(int index) {
    if (index < 0 || index >= bookCount) {
        cout << "�������� ������ �����." << endl;
        return;
    }
    for (int i = index; i < bookCount - 1; ++i) {
        library[i] = library[i + 1];
    }
    --bookCount;
}

void searchBooksByYear(int year) {
    cout << "����� ������������� � " << year << " ����:" << endl;
    cout << "�����\t��������" << endl;
    for (int i = 0; i < bookCount; ++i) {
        if (library[i].acquisitionDate.year == year) {
            cout << library[i].author << "\t" << library[i].title << endl;
        }
    }
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "rus");
    int choice;
    do {
        cout << "����:" << endl;
        cout << "1. �������� �����" << endl;
        cout << "2. �������� ��� �����" << endl;
        cout << "3. ������� �����" << endl;
        cout << "4. ����� ���� �� ���� ������������" << endl;
        cout << "5. �����" << endl;
        cout << "��� �����: ";
        cin >> choice;

        system("cls");
        switch (choice) {
        case 1:
            addBook();
            break;
        case 2:
            showBooks();
            break;
        case 3:
            int index;
            cout << "������� ������ ����� ��� ��������: ";
            cin >> index;
            deleteBook(index);
            break;
        case 4:
            int year;
            cout << "������� ��� ��� ������: ";
            cin >> year;
            searchBooksByYear(year);
            break;
        case 5:
            cout << "���������� ���������." << endl;
            break;
        default:
            cout << "�������� �����. ����������, ���������� �����." << endl;
            break;
        }
    } while (choice != 5);

    return 0;
}
