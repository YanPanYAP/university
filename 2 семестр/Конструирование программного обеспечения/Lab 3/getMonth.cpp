#include "stdafx.h"

string getMonth(int month)
{
	string months[] = { "������", "�������", "����", "������", "���", "����", "����", "������", "��������", "�������", "������", "�������" };
	return months[month - 1];
}