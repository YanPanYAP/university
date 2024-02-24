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

	cout << "������� ����� ����� � ������������������ ���������: ";
	cin >> N;

	if (N == 0)
	{
		cout << "\n���� ����� ���������: " << first << endl;
	}
	else if (N == 1)
	{
		cout << "\n���� ����� ���������: " << second << endl;
	}
	else if (N < 0)
	{
		cout << "\n������� �������� ������!\n";
	}
	else
	{
		cout << "\n���� ����� ���������, ����������� � ������� �����: " << findNumberCycle(N, F0, F1, answerCycle) << endl;
		clock_t time_req;
		time_req = clock();
		cout << "\n���� ����� ���������, ����������� � ������� ��������: " << findNumberRecurson(N, F0, F1, i, answerRecurson);
		time_req = clock() - time_req;
		int minutes = time_req / CLOCKS_PER_SEC / 60;
		int seconds = time_req / CLOCKS_PER_SEC % 60;
		cout << "\n���������� ����� � ������� �������� ��������� " << setw(2) << setfill('0') << minutes << " ����� " << setw(2) << setfill('0') << seconds << " ������" << endl;
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
	cout << "\n���������� ����� � ������� ����� ��������� " << setw(2) << setfill('0') << minutes << " ����� " << setw(2) << setfill('0') << seconds << " ������";

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
