#pragma once
#include <string>
#include <iostream>
using namespace std;

bool isCorrect(int day, int month, int year, string date);
bool isLeapYear(int year);
int getDaysToBirthday(int day, int month, int year);
int getDayNumber(int day, int month, int year);
string getMonth(int month);