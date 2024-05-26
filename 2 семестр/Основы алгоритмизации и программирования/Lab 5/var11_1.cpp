#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <string>
using namespace std;

struct Date {
	unsigned int day : 5;
	unsigned int month : 4;
	unsigned int year : 12;
};

struct Item {
	string clientName;
	string itemName;
	float estimatedValue;
	float loanAmount;
	Date returnDate;
	unsigned int storagePeriod : 8;
};

const int MAX_ITEMS = 100;
Item items[MAX_ITEMS];
int itemCount = 0;

void addItem()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int b;
	string date;
	Item newItem;

	cout << "Фамилия клиента: ";
	cin >> newItem.clientName;
	cout << "Наименование товара: ";
	cin >> newItem.itemName;
	cout << "Оценочная стоимость: ";
	cin >> newItem.estimatedValue;
	cout << "Сумма, выданная под залог: ";
	cin >> newItem.loanAmount;
	cout << "Дата сдачи в формате ДДММГГГГ: ";
	cin >> date;
	newItem.returnDate.day = stoi(date.substr(0, 2));
	newItem.returnDate.month = stoi(date.substr(2, 2));
	newItem.returnDate.year = stoi(date.substr(4));
	cout << "Срок хранения: ";
	cin >> b; cout << endl;
	newItem.storagePeriod = b;

	items[itemCount++] = newItem;
	system("cls");
}

void showData()
{
	cout << "№\t" << "Фамилия клиента\t" << "Наименование товара\t" << "Оценочная стоимость\t" << "Сумма, выданная под залог\t" << "Дата сдачи\t" << "Срок хранения\t" << endl;
	
	for (int i = 0; i < itemCount; i++)
	{
		cout << i + 1 << '\t'
			<< setw(15) << left << items[i].clientName << '\t'
			<< setw(19) << left << items[i].itemName << '\t'
			<< setw(19) << left << items[i].estimatedValue << '\t'
			<< setw(25) << left << items[i].loanAmount << '\t'
			<< items[i].returnDate.day << "." << items[i].returnDate.month << "." << items[i].returnDate.year << '\t'
			<< setw(13) << left << items[i].storagePeriod
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
			items[i] = items[i + 1];
		}
		itemCount--;
		cout << "Товар успешно удален." << endl << endl;
	}
	else
	{
		cout << "Некорректный номер товара или он отсутствует." << endl << endl;
	}
}

void searchItemByName(const string& name)
{
	bool found = false;

	cout << "Результаты поиска по фамилии клиента \"" << name << "\":" << endl;
	cout << "№\t" << "Наименование товара\t" << "Оценочная стоимость\t" << "Сумма, выданная под залог\t" << "Дата сдачи\t" << "Срок хранения\t" << endl;
	for (int i = 0; i < itemCount; i++)
	{
		if (items[i].clientName == name)
		{
			cout << i + 1 << '\t'
				<< setw(19) << left << items[i].itemName << '\t'
				<< setw(19) << left << items[i].estimatedValue << '\t'
				<< setw(25) << left << items[i].loanAmount << '\t'
				<< items[i].returnDate.day << "." << items[i].returnDate.month << "." << items[i].returnDate.year << '\t'
				<< setw(13) << left << items[i].storagePeriod
				<< endl;
			found = true;
		}
	}
	cout << endl;

	if (!found)
	{
		system("cls");
		cout << "Товары с указанной фамилией клиента не найдены." << endl << endl;
	}
}


int main()
{
	setlocale(LC_ALL, "rus");
	int choice;
	string name;
	do
	{
		cout << "1. Добавить товар.\n";
		cout << "2. Показать товары.\n";
		cout << "3. Удалить по номеру.\n";
		cout << "4. Найти товар по фамилии.\n";
		cout << "5. Выход.\n";
		cout << "Ваш выбор: ";
		cin >> choice; cout << endl;
		system("cls");

		switch (choice)
		{
		case 1:
			addItem();
			break;
		case 2:
			showData();
			break;
		case 3:
			int itemNumber;
			cout << "Введите номер товара для удаления: ";
			cin >> itemNumber;
			deleteItem(itemNumber);
			break;
		case 4:
			cout << "Введите фамилию клиента для поиска: ";
			cin >> name;
			searchItemByName(name);
			break;
		case 5:
			cout << "Приходите еще!";
			break;
		default:
			cout << "Некорректный ввод!\n";
		}
	} while (choice != 5);

	return 0;
}