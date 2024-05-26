#pragma once
#include <iostream>
#include <locale>
#include <float.h>

using namespace std;

namespace Varparm {
	int ivarparm(int, ...);
	int svarparm(short, ...);
	double fvarparm(float, ...);
	double dvarparm(double, ...);
};