#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <iomanip>
using namespace std;

enum BookOrigin {
    PURCHASE,
    THEFT,
    GIFT
};

enum BookSection {
    SPECIAL_LITERATURE,
    HOBBY,
    HOUSEHOLD,
    FICTION
};

union BookData {
    BookOrigin origin;
    BookSection section;
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
    BookData data;
    bool available;
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
    cout << "������� ������������� ����� (0 ��� �������, 1 ��� �����, 2 ��� �������): ";
    int originChoice;
    cin >> originChoice;
    newBook.data.origin = static_cast<BookOrigin>(originChoice);
    cout << "�������� �� �����? (1 ��� ��, 0 ��� ���): ";
    cin >> newBook.available;

    library[bookCount++] = newBook;
    system("cls");
}

void saveLibraryToFile(const string& filename) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        for (int i = 0; i < bookCount; ++i) {
            file << "�����: " << library[i].author << endl;
            file << "��������: " << library[i].title << endl;
            file << "������������: " << library[i].publisher << endl;
            file << "�������������: " << library[i].data.origin << endl;
            file << "�����������: " << (library[i].available ? "��" : "���") << endl;
            file << endl;
        }
        file.close();
        cout << "���������� ������� ��������� � ����: " << filename << endl;
    }
    else {
        cout << "���������� ������� ���� ��� ������." << endl;
    }
}

void showBooks() {
    cout << "����������:" << endl;
    cout << "�����\t��������\t������������\t�������������\t��������" << endl;
    for (int i = 0; i < bookCount; ++i) {
        cout << setw(5) << left << library[i].author << '\t'
        << setw(8) << left << library[i].title << '\t'
        << setw(12) << left << library[i].publisher << '\t'
        << setw(13) << left << library[i].data.origin << '\t'
        << setw(8) << (library[i].available ? "��" : "���") << endl;
    }
    cout << endl;
}

void searchBooksByAuthor(const string& author) {
    cout << "����� ���� �� ������: " << author << endl;
    cout << "�����\t��������\t������������\t�������������\t��������" << endl;
    for (int i = 0; i < bookCount; ++i) {
        if (library[i].author == author) {
            cout << setw(5) << left << library[i].author << '\t'
            << setw(8) << left << library[i].title << '\t'
            << setw(12) << left << library[i].publisher << '\t'
            << setw(13) << left << library[i].data.origin << '\t'
            << setw(8) << (library[i].available ? "��" : "���") << endl;
        }
    }
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "rus");
    int choice;
    string filename, author;

    do {
        cout << "����:" << endl;
        cout << "1. �������� �����" << endl;
        cout << "2. ��������� ���������� � ����" << endl;
        cout << "3. �������� ��� �����" << endl;
        cout << "4. ����� ���� �� ������" << endl;
        cout << "5. �����" << endl;
        cout << "��� �����: ";
        cin >> choice;

        system("cls");
        switch (choice) {
        case 1:
            addBook();
            break;
        case 2:
            cout << "������� ��� ����� ��� ����������: ";
            cin >> filename;
            saveLibraryToFile(filename);
            break;
        case 3:
            showBooks();
            break;
        case 4:
            cout << "������� ������ ��� ������: ";
            cin.ignore();
            getline(cin, author);
            searchBooksByAuthor(author);
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
