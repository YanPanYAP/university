#include "stdafx.h"

int main() {
	setlocale(LC_ALL, "rus");
	cout << "������� ���� � ������� ��������: ";
	
	string date;
    cin >> date;

    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(2, 2));
    int year = stoi(date.substr(4));

	if (isCorrect(day, month, year, date)) {
		if (isLeapYear(year)) {
			cout << endl << year << " ��� �������� ����������.";
		}
		else {
			cout << endl << year << " ��� �� �������� ����������.";
		}

		cout << endl << getDaysToBirthday(day, month, year) << " ����/����/��� �� ���������� ��� ����� ��������.";
		cout << endl << "���������� ����� ��� � ����: " << getDayNumber(day, month, year) << endl;
	}
	else {
		cout << "������� ������������ ������!";
	}

	return 0;
}