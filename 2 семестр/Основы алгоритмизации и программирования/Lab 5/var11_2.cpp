#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <string>
#include <fstream>
using namespace std;

enum BodyType
{
	����� = 1, ���������, ����, �������, ���������, �����������, �����, �������, �������, ���������, �������
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

	cout << "����� ����������: ";
	cin >> newItem.carBrand;
	cout << "����: ";
	cin >> newItem.color;
	cout << "��������� �����: ";
	cin >> newItem.number;
	cout << "���� ������� � ������� ��������: ";
	cin >> date;
	newItem.releaseDate.day = stoi(date.substr(0, 2));
	newItem.releaseDate.month = stoi(date.substr(2, 2));
	newItem.releaseDate.year = stoi(date.substr(4));
	cout << "��� ������: ����� - 1, ��������� - 2, ���� - 3, ������� - 4, ��������� - 5, ����������� - 6, ����� - 7, ������� - 8, ������� - 9, ��������� - 10, ������� - 11.\n��� �����: ";
	cin >> choiceType;
	newItem.carType = static_cast<BodyType>(choiceType);
	cout << "���� ���������� ���������� � ������� ��������: ";
	cin >> date;
	newItem.technicalInspection.day = stoi(date.substr(0, 2));
	newItem.technicalInspection.month = stoi(date.substr(2, 2));
	newItem.technicalInspection.year = stoi(date.substr(4));
	cout << "������� ���������: ";
	cin >> newItem.owner;

	ofstream outFile("car_data.txt", ios::app);
	outFile << "����� ����������: " << newItem.carBrand << endl;
	outFile << "����: " << newItem.color << endl;
	outFile << "��������� �����: " << newItem.number << endl;
	outFile << "���� �������: " << newItem.releaseDate.day << "." << newItem.releaseDate.month << "." << newItem.releaseDate.year << endl;
	outFile << "��� ������: " << newItem.carType << endl;
	outFile << "���� ���������� ����������: " << newItem.technicalInspection.day << "." << newItem.technicalInspection.month << "." << newItem.technicalInspection.year << endl;
	outFile << "������� ���������: " << newItem.owner << endl;
	outFile.close();

	items[itemCount++] = newItem;
	system("cls");
}

void showData()
{
	cout << "�\t" << "����� ����������\t" << "����\t" << "��������� �����\t" << "���� �������\t" << "��� ������\t" << "���� ���������� ����������\t" << "������� ���������\t" << endl;

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

	cout << "���������� ������ �� ������� ������� \"" << name << "\":" << endl;
	cout << "�\t" << "����� ����������\t" << "����\t" << "��������� �����\t" << "���� �������\t" << "��� ������\t" << "���� ���������� ����������\t" << endl;

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
		cout << "���������� � ��������� �������� ��������� �� �������." << endl << endl;
	}
}


int main()
{
	setlocale(LC_ALL, "rus");
	int choice;
	string name;
	do
	{
		cout << "1. �������� ����������.\n";
		cout << "2. ���������� �����������.\n";
		cout << "3. ����� ���������� �� ������� ���������.\n";
		cout << "4. �����.\n";
		cout << "��� �����: ";
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
			cout << "������� ������� ������� ��� ������: ";
			cin >> name;
			searchItemByName(name);
			break;
		case 4:
			cout << "��������� ���!";
			break;
		default:
			cout << "������������ ����!\n";
		}
	} while (choice != 4);

	return 0;
}