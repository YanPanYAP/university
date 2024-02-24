#include "stdafx.h"

int getDaysToBirthday(int day, int month, int year) {
	int birthdayDate = 16;
	int birthdayMonth = 2;
	int a = 28, b = 30, c = 31;
	
	if (isLeapYear(year)) {
		a++;
	}

	int d = a - birthdayDate;

	switch (month) {
	case 1: return (c - day + birthdayDate); break;
	case 2: return (abs(birthdayDate-day)); break;
	case 3: return (d + day); break;
	case 4: return (d + c + day); break;
	case 5: return (d + c + b + day); break;
	case 6: return (d + 2 * c + b + day); break;
	case 7: return (d + 2 * c + 2 * b + day); break;
	case 8: 
		if (day <= 16) {
			int e = d + 3 * c + 2 * b + day;
		}
		else {
			int e = birthdayDate + 3 * c + 2* b + day - b;
		} break;
	case 9: return (birthdayDate + 3 * c + b + day - b); break;
	case 10: return (birthdayDate + 2 * c + b + day - c); break;
	case 11: return (birthdayDate + 2 * c + b + day - b); break;
	case 12: return (birthdayDate + c + day - c); break;
	}
}