#include "stdafx.h"

int main() {
	setlocale(LC_ALL, "rus");
	cout << "Введите дату в формате ДДММГГГГ: ";

	string date;
	cin >> date;

	int day = stoi(date.substr(0, 2));
	int month = stoi(date.substr(2, 2));
	int year = stoi(date.substr(4));

	if (isCorrect(day, month, year, date)) {
		if (isLeapYear(year)) {
			cout << endl << year << " год является високосным.";
		}
		else {
			cout << endl << year << " год не является високосным.";
		}

		cout << endl << getDaysToBirthday(day, month, year) << " день/дней/дня до ближайшего дня моего рождения.";
		cout << endl << "Порядковый номер дня в году: " << getDayNumber(day, month, year) << "." << endl;
		cout << "В дате введен месяц " << getMonth(month) << "." << endl;
		if (getDayNumber(day, month, year) == 256) {
			cout << "Ваша дата попала на день программиста!" << endl;
		}
		cout << "Порядковый номер года в дате: " << year << "." << endl;
		if (month == 6 && day == 1) {
			cout << "Ваша дата попала на первый день лета!" << endl;
		}
	}
	else {
		cout << "Введены некорректные данные!";
	}

	return 0;
}