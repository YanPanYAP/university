#include "stdafx.h"
#include <tchar.h>
#include "Auxil.h"                            // вспомогательные функции 
#include <iostream>
#include <ctime>
#include <locale>
using namespace  std;

#define  CYCLE  1000000                       // количество циклов  
#define N 100

clock_t SS[N];

unsigned long long getFibonacci(int n) {
	if (n <= 1) return n;
	return getFibonacci(n - 1) + getFibonacci(n - 2);
}

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru_RU.UTF-8");

	double  av1 = 0, av2 = 0;
	clock_t  t1 = 0, t2 = 0;

	auxil::start();                          // старт генерации 
	t1 = clock();                            // фиксация времени 
	for (int i = 0; i < CYCLE; i++)
	{
		av1 += (double)auxil::iget(-100, 100); // сумма случайных чисел 
		av2 += auxil::dget(-100, 100);         // сумма случайных чисел 
	}
	t2 = clock();                            // фиксация времени 


	std::cout << std::endl << "количество циклов:         " << CYCLE;
	std::cout << std::endl << "среднее значение (int):    " << av1 / CYCLE;
	std::cout << std::endl << "среднее значение (double): " << av2 / CYCLE;
	std::cout << std::endl << "продолжительность (у.е):   " << (t2 - t1);
	std::cout << std::endl << "                  (сек):   "
		<< ((double)(t2 - t1)) / ((double)CLOCKS_PER_SEC);
	std::cout << std::endl << std::endl;

	auxil::start();

	for (int n = 10; n <= 45; n++)
	{
		SS[n] = clock();
		unsigned long long F = getFibonacci(n);
		SS[n] = -(SS[n] - clock());
		cout << n << "-ое число Фибоначчи = " << F << " за время: " << SS[n] << endl;
	}

	system("pause");
	return 0;
}