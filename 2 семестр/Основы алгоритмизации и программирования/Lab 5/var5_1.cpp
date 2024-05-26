#include <iostream>
#include <iomanip>
#include <string>
#include <Windows.h>
using namespace std;

enum BookOrigin {
    ПОКУПКА,
    КРАЖА,
    ПОДАРОК
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
    cout << "Введите автора: ";
    cin.ignore();
    getline(cin, newBook.author);
    cout << "Введите название: ";
    getline(cin, newBook.title);
    cout << "Введите издательство: ";
    getline(cin, newBook.publisher);
    cout << "Введите раздел: ";
    getline(cin, newBook.section);
    cout << "Введите происхождение (0 для покупки, 1 для кражи, 2 для подарка): ";
    int originChoice;
    cin >> originChoice;
    newBook.origin = static_cast<BookOrigin>(originChoice);
    cout << "Доступна ли книга? (1 для да, 0 для нет): ";
    cin >> newBook.available;
    cout << "Введите дату приобретения (ДД ММ ГГГГ): ";
    cin >> newBook.acquisitionDate.day >> newBook.acquisitionDate.month >> newBook.acquisitionDate.year;

    library[bookCount++] = newBook;
    system("cls");
}

void showBooks() {
    system("cls");
    cout << "Библиотека:" << endl;
    cout << "Автор\tНазвание\tИздательство\tРаздел\tПроисхождение\tДоступна\tДата приобретения" << endl;
    for (int i = 0; i < bookCount; ++i) {
        cout << setw(5) << left << library[i].author << '\t'
            << setw(8) << left << library[i].title << '\t'
            << setw(12) << left << library[i].publisher << '\t'
            << setw(6) << left << library[i].section << '\t'
            << setw(13) << left << library[i].origin << '\t'
            << setw(8) << left << (library[i].available ? "Да" : "Нет") << '\t'
            << library[i].acquisitionDate.day << "." << library[i].acquisitionDate.month << "." << library[i].acquisitionDate.year << '\t'
            << endl;
    }
    cout << endl;
}

void deleteBook(int index) {
    if (index < 0 || index >= bookCount) {
        cout << "Неверный индекс книги." << endl;
        return;
    }
    for (int i = index; i < bookCount - 1; ++i) {
        library[i] = library[i + 1];
    }
    --bookCount;
}

void searchBooksByYear(int year) {
    cout << "Книги приобретенные в " << year << " году:" << endl;
    cout << "Автор\tНазвание" << endl;
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
        cout << "Меню:" << endl;
        cout << "1. Добавить книгу" << endl;
        cout << "2. Показать все книги" << endl;
        cout << "3. Удалить книгу" << endl;
        cout << "4. Поиск книг по году приобретения" << endl;
        cout << "5. Выход" << endl;
        cout << "Ваш выбор: ";
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
            cout << "Введите индекс книги для удаления: ";
            cin >> index;
            deleteBook(index);
            break;
        case 4:
            int year;
            cout << "Введите год для поиска: ";
            cin >> year;
            searchBooksByYear(year);
            break;
        case 5:
            cout << "Завершение программы." << endl;
            break;
        default:
            cout << "Неверный выбор. Пожалуйста, попробуйте снова." << endl;
            break;
        }
    } while (choice != 5);

    return 0;
}
