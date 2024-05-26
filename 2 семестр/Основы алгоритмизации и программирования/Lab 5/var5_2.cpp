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
    cout << "Введите автора: ";
    cin.ignore();
    getline(cin, newBook.author);
    cout << "Введите название: ";
    getline(cin, newBook.title);
    cout << "Введите издательство: ";
    getline(cin, newBook.publisher);
    cout << "Введите происхождение книги (0 для покупки, 1 для кражи, 2 для подарка): ";
    int originChoice;
    cin >> originChoice;
    newBook.data.origin = static_cast<BookOrigin>(originChoice);
    cout << "Доступна ли книга? (1 для да, 0 для нет): ";
    cin >> newBook.available;

    library[bookCount++] = newBook;
    system("cls");
}

void saveLibraryToFile(const string& filename) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        for (int i = 0; i < bookCount; ++i) {
            file << "Автор: " << library[i].author << endl;
            file << "Название: " << library[i].title << endl;
            file << "Издательство: " << library[i].publisher << endl;
            file << "Происхождение: " << library[i].data.origin << endl;
            file << "Доступность: " << (library[i].available ? "Да" : "Нет") << endl;
            file << endl;
        }
        file.close();
        cout << "Библиотека успешно сохранена в файл: " << filename << endl;
    }
    else {
        cout << "Невозможно открыть файл для записи." << endl;
    }
}

void showBooks() {
    cout << "Библиотека:" << endl;
    cout << "Автор\tНазвание\tИздательство\tПроисхождение\tДоступна" << endl;
    for (int i = 0; i < bookCount; ++i) {
        cout << setw(5) << left << library[i].author << '\t'
        << setw(8) << left << library[i].title << '\t'
        << setw(12) << left << library[i].publisher << '\t'
        << setw(13) << left << library[i].data.origin << '\t'
        << setw(8) << (library[i].available ? "Да" : "Нет") << endl;
    }
    cout << endl;
}

void searchBooksByAuthor(const string& author) {
    cout << "Поиск книг по автору: " << author << endl;
    cout << "Автор\tНазвание\tИздательство\tПроисхождение\tДоступна" << endl;
    for (int i = 0; i < bookCount; ++i) {
        if (library[i].author == author) {
            cout << setw(5) << left << library[i].author << '\t'
            << setw(8) << left << library[i].title << '\t'
            << setw(12) << left << library[i].publisher << '\t'
            << setw(13) << left << library[i].data.origin << '\t'
            << setw(8) << (library[i].available ? "Да" : "Нет") << endl;
        }
    }
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "rus");
    int choice;
    string filename, author;

    do {
        cout << "Меню:" << endl;
        cout << "1. Добавить книгу" << endl;
        cout << "2. Сохранить библиотеку в файл" << endl;
        cout << "3. Показать все книги" << endl;
        cout << "4. Поиск книг по автору" << endl;
        cout << "5. Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        system("cls");
        switch (choice) {
        case 1:
            addBook();
            break;
        case 2:
            cout << "Введите имя файла для сохранения: ";
            cin >> filename;
            saveLibraryToFile(filename);
            break;
        case 3:
            showBooks();
            break;
        case 4:
            cout << "Введите автора для поиска: ";
            cin.ignore();
            getline(cin, author);
            searchBooksByAuthor(author);
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
