#include <iostream>
#include <iomanip>
#include <ctime>
using namespace std;

long long findNumberCycle(int N, long long F0, long long F1, long long answerCycle);
long long findNumberRecurson(int N, long long F0, long long F1, int i, long long answerRecurson);

int main()
{
	setlocale(LC_ALL, "rus");
	const int first = 0, second = 1;
	int N, i = 2;
	long long F0 = first, F1 = second, answerRecurson = 0, answerCycle = 0;

	cout << "Введите номер числа в последовательности Фибоначчи: ";
	cin >> N;

	if (N == 0)
	{
		cout << "\nВаше число Фибоначчи: " << first << endl;
	}
	else if (N == 1)
	{
		cout << "\nВаше число Фибоначчи: " << second << endl;
	}
	else if (N < 0)
	{
		cout << "\nВведены неверные данные!\n";
	}
	else
	{
		cout << "\nВаше число Фибоначчи, вычисленное с помощью цикла: " << findNumberCycle(N, F0, F1, answerCycle) << endl;
		clock_t time_req;
		time_req = clock();
		cout << "\nВаше число Фибоначчи, вычисленное с помощью рекурсии: " << findNumberRecurson(N, F0, F1, i, answerRecurson);
		time_req = clock() - time_req;
		int minutes = time_req / CLOCKS_PER_SEC / 60;
		int seconds = time_req / CLOCKS_PER_SEC % 60;
		cout << "\nНахождение числа с помощью рекурсии составило " << setw(2) << setfill('0') << minutes << " минут " << setw(2) << setfill('0') << seconds << " секунд" << endl;
	}

	return 0;
}

long long findNumberCycle(int N, long long F0, long long F1, long long answerCycle)
{
	clock_t time_req;
	time_req = clock();

	for (int i = 2; i <= N; i++)
	{
		answerCycle = F1 + F0;
		F0 = F1;
		F1 = answerCycle;
	}

	time_req = clock() - time_req;
	int minutes = time_req / CLOCKS_PER_SEC / 60;
	int seconds = time_req / CLOCKS_PER_SEC % 60;
	cout << "\nНахождение числа с помощью цикла составило " << setw(2) << setfill('0') << minutes << " минут " << setw(2) << setfill('0') << seconds << " секунд";

	return answerCycle;
}

long long findNumberRecurson(int N, long long F0, long long F1, int i, long long answerRecurson)
{
	answerRecurson = F1 + F0;
	if (i < N)
	{
		i++;
		F0 = F1;
		F1 = answerRecurson;
		findNumberRecurson(N, F0, F1, i, answerRecurson);
	}
	else
	{
		return answerRecurson;
	}
}
