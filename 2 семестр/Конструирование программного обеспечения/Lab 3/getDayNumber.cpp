#include "stdafx.h"

int getDayNumber(int day, int month, int year) {
	int a = 28, b = 30, c = 31;

	if (isLeapYear(year)) {
		a++;
	}

	switch (month) {
	case 1: return (day); break;
	case 2: return (c + day); break;
	case 3: return (c + a + day); break;
	case 4: return (2 * c + a + day); break;
	case 5: return (2 * c + a + b + day); break;
	case 6: return (3 * c + a + b + day); break;
	case 7: return (3 * c + a + 2 * b + day); break;
	case 8: return (4 * c + a + 2 * b + day); break;
	case 9: return (5 * c + a + 2 * b + day); break;
	case 10: return (5 * c + a + 3 * b + day); break;
	case 11: return (6 * c + a + 3 * b + day); break;
	case 12: return (6 * c + a + 4 * b + day); break;
	}
}