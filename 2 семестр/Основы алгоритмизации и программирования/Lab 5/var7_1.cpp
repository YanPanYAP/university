#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <string>
using namespace std;

enum Percent {
    Percent_5 = 5,
    Percent_7 = 7,
    Percent_11 = 11,
    Percent_20 = 20,
    Percent_25 = 25,
    Percent_30 = 30
};

struct Item {
    string name;
    float price;
    int quantity;
    Percent markup;
};

const int MAX_ITEMS = 100;
Item stock[MAX_ITEMS];
int itemCount = 0;

void addItem()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Item newItem;

    cout << "������������ ������: ";
    cin >> newItem.name;
    cout << "����: ";
    cin >> newItem.price;
    cout << "����������: ";
    cin >> newItem.quantity;
    cout << "������� �������� �������� (5, 7, 11, 20, 25, 30): ";
    int markup;
    cin >> markup;
    newItem.markup = static_cast<Percent>(markup);

    stock[itemCount++] = newItem;
    system("cls");
}

void showStock()
{
    cout << "�\t" << "������������ ������\t" << "����\t" << "����������\t" << "������� ��������\t" << endl;

    for (int i = 0; i < itemCount; i++)
    {
        cout << i + 1 << '\t'
            << setw(20) << left << stock[i].name << '\t'
            << setw(6) << left << stock[i].price << '\t'
            << setw(10) << left << stock[i].quantity << '\t'
            << stock[i].markup << "%" << '\t'
            << endl;
    }
    cout << endl;
}

void deleteItem(int itemNumber)
{
    if (itemNumber >= 1 && itemNumber <= itemCount)
    {
        for (int i = itemNumber - 1; i < itemCount - 1; i++)
        {
            stock[i] = stock[i + 1];
        }
        itemCount--;
        cout << "����� ������� ������." << endl << endl;
    }
    else
    {
        cout << "������������ ����� ������ ��� �� �����������." << endl << endl;
    }
}

void searchItemByPrice(float price)
{
    bool found = false;

    cout << "���������� ������ �� ���� " << price << ":" << endl;
    cout << "�\t" << "������������ ������\t" << "����\t" << "����������\t" << "������� ��������\t" << endl;

    for (int i = 0; i < itemCount; i++)
    {
        if (stock[i].price == price)
        {
            cout << i + 1 << '\t'
                << setw(20) << left << stock[i].name << '\t'
                << setw(6) << left << stock[i].price << '\t'
                << setw(10) << left << stock[i].quantity << '\t'
                << stock[i].markup << "%" << '\t'
                << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << "������ � ����� " << price << " �� �������." << endl << endl;
    }
    else
    {
        cout << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "rus");
    int choice;
    float price;
    do
    {
        cout << "1. �������� �����.\n";
        cout << "2. �������� ������.\n";
        cout << "3. ������� �� ������.\n";
        cout << "4. ����� ����� �� ����.\n";
        cout << "5. �����.\n";
        cout << "��� �����: ";
        cin >> choice;
        cout << endl;
        system("cls");

        switch (choice)
        {
        case 1:
            addItem();
            break;
        case 2:
            showStock();
            break;
        case 3:
            int itemNumber;
            cout << "������� ����� ������ ��� ��������: ";
            cin >> itemNumber;
            deleteItem(itemNumber);
            break;
        case 4:
            cout << "������� ���� ��� ������: ";
            cin >> price;
            searchItemByPrice(price);
            break;
        case 5:
            cout << "��������� ���!";
            break;
        default:
            cout << "������������ ����!\n";
        }
    } while (choice != 5);

    return 0;
}
