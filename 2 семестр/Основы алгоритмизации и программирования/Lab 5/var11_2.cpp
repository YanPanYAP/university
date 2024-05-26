#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <string>
#include <fstream>
using namespace std;

enum BodyType
{
	Седан = 1, Универсал, Купе, Хэтчбэк, Кроссовер, Внедорожник, Пикап, Минивен, Родстер, Кабриолет, Лимузин
}type;

struct Date {
	unsigned int day;
	unsigned int month;
	unsigned int year;
};

struct Car
{
	string carBrand;
	string color;
	unsigned int number;
	Date releaseDate;
	BodyType carType;
	Date technicalInspection;
	string owner;
};

const int MAX_ITEMS = 100;
Car items[MAX_ITEMS];
int itemCount = 0;

void addItem()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int choiceType;
	string date;
	Car newItem;

	cout << "Марка автомобиля: ";
	cin >> newItem.carBrand;
	cout << "Цвет: ";
	cin >> newItem.color;
	cout << "Заводской номер: ";
	cin >> newItem.number;
	cout << "Дата выпуска в формате ДДММГГГГ: ";
	cin >> date;
	newItem.releaseDate.day = stoi(date.substr(0, 2));
	newItem.releaseDate.month = stoi(date.substr(2, 2));
	newItem.releaseDate.year = stoi(date.substr(4));
	cout << "Тип кузова: Седан - 1, Универсал - 2, Купе - 3, Хэтчбэк - 4, Кроссовер - 5, Внедорожник - 6, Пикап - 7, Минивен - 8, Родстер - 9, Кабриолет - 10, Лимузин - 11.\nВаш выбор: ";
	cin >> choiceType;
	newItem.carType = static_cast<BodyType>(choiceType);
	cout << "Дата последнего техосмотра в формате ДДММГГГГ: ";
	cin >> date;
	newItem.technicalInspection.day = stoi(date.substr(0, 2));
	newItem.technicalInspection.month = stoi(date.substr(2, 2));
	newItem.technicalInspection.year = stoi(date.substr(4));
	cout << "Фамилия владельца: ";
	cin >> newItem.owner;

	ofstream outFile("car_data.txt", ios::app);
	outFile << "Марка автомобиля: " << newItem.carBrand << endl;
	outFile << "Цвет: " << newItem.color << endl;
	outFile << "Заводской номер: " << newItem.number << endl;
	outFile << "Дата выпуска: " << newItem.releaseDate.day << "." << newItem.releaseDate.month << "." << newItem.releaseDate.year << endl;
	outFile << "Тип кузова: " << newItem.carType << endl;
	outFile << "Дата последнего техосмотра: " << newItem.technicalInspection.day << "." << newItem.technicalInspection.month << "." << newItem.technicalInspection.year << endl;
	outFile << "Фамилия владельца: " << newItem.owner << endl;
	outFile.close();

	items[itemCount++] = newItem;
	system("cls");
}

void showData()
{
	cout << "№\t" << "Марка автомобиля\t" << "Цвет\t" << "Заводской номер\t" << "Дата выпуска\t" << "Тип кузова\t" << "Дата последнего техосмотра\t" << "Фамилия владельца\t" << endl;

	for (int i = 0; i < itemCount; i++)
	{
		cout << i + 1 << '\t'
			<< setw(16) << left << items[i].carBrand << '\t'
			<< setw(4) << left << items[i].color << '\t'
			<< setw(15) << left << items[i].number << '\t'
			<< items[i].releaseDate.day << "." << items[i].releaseDate.month << "." << items[i].releaseDate.year << '\t'
			<< setw(10) << left << items[i].carType << '\t'
			<< items[i].technicalInspection.day << "." << items[i].technicalInspection.month << "." << items[i].technicalInspection.year << setw(16) << '\t'
			<< setw(17) << left << items[i].owner
			<< endl;
	}
	cout << endl;
}

void searchItemByName(const string& name)
{
	bool found = false;

	cout << "Результаты поиска по фамилии клиента \"" << name << "\":" << endl;
	cout << "№\t" << "Марка автомобиля\t" << "Цвет\t" << "Заводской номер\t" << "Дата выпуска\t" << "Тип кузова\t" << "Дата последнего техосмотра\t" << endl;

	for (int i = 0; i < itemCount; i++)
	{
		if (items[i].owner == name)
		{
			cout << i + 1 << '\t'
				<< setw(16) << left << items[i].carBrand << '\t'
				<< setw(4) << left << items[i].color << '\t'
				<< setw(15) << left << items[i].number << '\t'
				<< items[i].releaseDate.day << "." << items[i].releaseDate.month << "." << items[i].releaseDate.year << '\t'
				<< setw(10) << left << items[i].carType << '\t'
				<< items[i].technicalInspection.day << "." << items[i].technicalInspection.month << "." << items[i].technicalInspection.year << setw(16) << '\t'
				<< endl;
			found = true;
		}
	}
	cout << endl;

	if (!found)
	{
		system("cls");
		cout << "Автомобили с указанной фамилией владельца не найдены." << endl << endl;
	}
}


int main()
{
	setlocale(LC_ALL, "rus");
	int choice;
	string name;
	do
	{
		cout << "1. Добавить автомобиль.\n";
		cout << "2. Справочник автомобилей.\n";
		cout << "3. Найти автомобиль по фамилии владельца.\n";
		cout << "4. Выход.\n";
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
			cout << "Введите фамилию клиента для поиска: ";
			cin >> name;
			searchItemByName(name);
			break;
		case 4:
			cout << "Приходите еще!";
			break;
		default:
			cout << "Некорректный ввод!\n";
		}
	} while (choice != 4);

	return 0;
}