#include <iostream>
#include <Windows.h>
using namespace std;

void quickSort(int allPeople[], int first, int last) {
	if (first >= last) {
		return;
	}

	int pivot = allPeople[(first + last) / 2];
	int left = first;
	int right = last;

	while (left <= right) {
		while (allPeople[left] < pivot) {
			left++;
		}
		while (allPeople[right] > pivot) {
			right--;
		}
		if (left <= right) {
			int temp = allPeople[left];
			allPeople[left] = allPeople[right];
			allPeople[right] = temp;
			left++;
			right--;
		}
	}

	if (first < right) {
		quickSort(allPeople, first, right);
	}
	if (left < last) {
		quickSort(allPeople, left, last);
	}
}

int main()
{
	setlocale(LC_CTYPE, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int N, sum = 1, guard = 1;

	cout << "Определяем призеров.\n" << "Введите натуральное число N (<10000) для генерации количества: ";
	cin >> N;
	int* massPeople = new int[N];

	for (int i = 0; i < N; i++)
	{
		massPeople[i] = 1 + rand() % 100;
	}

	int* allPeople; allPeople = massPeople;

	quickSort(allPeople, 0, N - 1);

	for (int i = 0; i < N; i++)
	{
		cout << massPeople[i] << " ";
	}

	N--;
	for (N; N >= 0; N--)
	{
		if (allPeople[N - 1] == allPeople[N])
		{
			sum++;
		}
		else
		{
			if (guard < 3)
			{
				sum++;
				guard++;
			}
			else
			{
				break;
			}
			
		}
	}
	cout << "\nКоличество призеров: " << sum;

	delete[] massPeople;

	return 0;
}