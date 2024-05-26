#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <Windows.h>
using namespace std;

union Markup {
    int percent;
};

struct Item {
    string name;
    float price;
    int quantity;
    Markup markup;
};

const int MAX_ITEMS = 100;
Item stock[MAX_ITEMS];
int itemCount = 0;

void addItem() {
    cout << "Наименование товара: ";
    cin >> stock[itemCount].name;
    cout << "Цена: ";
    cin >> stock[itemCount].price;
    cout << "Количество: ";
    cin >> stock[itemCount].quantity;
    cout << "Процент торговой надбавки (5, 10, 15, 20, 35, 30): ";
    cin >> stock[itemCount].markup.percent;
    itemCount++;
    system("cls");
}

void showStock() {
    cout << "№\t" << "Наименование товара\t" << "Цена\t" << "Количество\t" << "Процент надбавки\t" << endl;
    for (int i = 0; i < itemCount; i++) {
        cout << i + 1 << "\t" << setw(20) << left << stock[i].name << "\t" << setw(6) << left << stock[i].price << "\t" << setw(10) << left << stock[i].quantity << "\t";
        if (stock[i].markup.percent == 5) cout << "5%";
        else if (stock[i].markup.percent == 10) cout << "10%";
        else if (stock[i].markup.percent == 15) cout << "15%";
        else if (stock[i].markup.percent == 20) cout << "20%";
        else if (stock[i].markup.percent == 35) cout << "35%";
        else if (stock[i].markup.percent == 30) cout << "30%";
        cout << endl;
    }
}

void writeToFile(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < itemCount; i++) {
            file << stock[i].name << " " << stock[i].price << " " << stock[i].quantity << " " << stock[i].markup.percent << endl;
        }
        file.close();
        cout << "Данные успешно записаны в файл." << endl;
    }
    else {
        cout << "Ошибка открытия файла для записи." << endl;
    }
}

void searchItemByName(const string& name) {
    bool found = false;
    cout << "Результаты поиска по наименованию \"" << name << "\":" << endl;
    cout << "№\t" << "Наименование товара\t" << "Цена\t" << "Количество\t" << "Процент надбавки\t" << endl;
    for (int i = 0; i < itemCount; i++) {
        if (stock[i].name == name) {
            cout << i + 1 << "\t" << setw(20) << left << stock[i].name << "\t" << setw(6) << left << stock[i].price << "\t" << setw(10) << left << stock[i].quantity << "\t";
            if (stock[i].markup.percent == 5) cout << "5%";
            else if (stock[i].markup.percent == 10) cout << "10%";
            else if (stock[i].markup.percent == 15) cout << "15%";
            else if (stock[i].markup.percent == 20) cout << "20%";
            else if (stock[i].markup.percent == 35) cout << "35%";
            else if (stock[i].markup.percent == 30) cout << "30%";
            cout << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Товары с наименованием \"" << name << "\" не найдены." << endl;
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int choice;
    string filename, name;
    do {
        cout << "1. Добавить товар.\n";
        cout << "2. Показать товары.\n";
        cout << "3. Записать данные в файл.\n";
        cout << "4. Найти товар по наименованию.\n";
        cout << "5. Выход.\n";
        cout << "Ваш выбор: ";
        cin >> choice;
        system("cls");

        switch (choice) {
        case 1:
            addItem();
            break;
        case 2:
            showStock();
            break;
        case 3:
            cout << "Введите имя файла для записи: ";
            cin >> filename;
            writeToFile(filename);
            break;
        case 4:
            cout << "Введите наименование товара для поиска: ";
            cin >> name;
            searchItemByName(name);
            break;
        case 5:
            cout << "До свидания!" << endl;
            break;
        default:
            cout << "Некорректный ввод!\n";
        }
    } while (choice != 5);

    return 0;
}
