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
    cout << "������������ ������: ";
    cin >> stock[itemCount].name;
    cout << "����: ";
    cin >> stock[itemCount].price;
    cout << "����������: ";
    cin >> stock[itemCount].quantity;
    cout << "������� �������� �������� (5, 10, 15, 20, 35, 30): ";
    cin >> stock[itemCount].markup.percent;
    itemCount++;
    system("cls");
}

void showStock() {
    cout << "�\t" << "������������ ������\t" << "����\t" << "����������\t" << "������� ��������\t" << endl;
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
        cout << "������ ������� �������� � ����." << endl;
    }
    else {
        cout << "������ �������� ����� ��� ������." << endl;
    }
}

void searchItemByName(const string& name) {
    bool found = false;
    cout << "���������� ������ �� ������������ \"" << name << "\":" << endl;
    cout << "�\t" << "������������ ������\t" << "����\t" << "����������\t" << "������� ��������\t" << endl;
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
        cout << "������ � ������������� \"" << name << "\" �� �������." << endl;
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int choice;
    string filename, name;
    do {
        cout << "1. �������� �����.\n";
        cout << "2. �������� ������.\n";
        cout << "3. �������� ������ � ����.\n";
        cout << "4. ����� ����� �� ������������.\n";
        cout << "5. �����.\n";
        cout << "��� �����: ";
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
            cout << "������� ��� ����� ��� ������: ";
            cin >> filename;
            writeToFile(filename);
            break;
        case 4:
            cout << "������� ������������ ������ ��� ������: ";
            cin >> name;
            searchItemByName(name);
            break;
        case 5:
            cout << "�� ��������!" << endl;
            break;
        default:
            cout << "������������ ����!\n";
        }
    } while (choice != 5);

    return 0;
}
