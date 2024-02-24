#include "stdafx.h"

bool isCorrect(int day, int month, int year, string date) {
	return (((year % 4 == 0 && month == 2 && day < 30) || (month == 2 && day < 29) || (month == 4 || month == 6 || month == 9 || month == 11 && day < 31) || (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12 && day < 32)) && month < 13 && 0 < year && year <= 9999 && 0 < day && day < 32 && date.length() == 8);
}