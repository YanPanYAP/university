#include "stdafx.h"

string getMonth(int month)
{
	string months[] = { "€нварь", "февраль", "март", "апрель", "май", "июнь", "июль", "август", "сент€брь", "окт€брь", "но€брь", "декабрь" };
	return months[month - 1];
}