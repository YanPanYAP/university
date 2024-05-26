#include <iostream>
#include <iomanip>
#include <ctime>
#include <stdlib.h>
using namespace std;

void bubble(int massB[], int N)
{
	clock_t startTime = clock();

	for (int i = 0; i < N - 1; i++) {
		for (int j = 0; j <= N - i - 2; j++) {
			if (massB[j] > massB[j + 1]) {
				swap(massB[j], massB[j + 1]);
			}
		}
	}

	clock_t endTime = clock();
	double elapsedTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;
	int minutes = static_cast<int>(elapsedTime) / 60;
	int seconds = static_cast<int>(elapsedTime) % 60;


	cout << "\nПолучившийся массив B:";
	for (int i = 0; i < N; i++)
	{
		cout << " ";
		cout << massB[i];
	}
	cout << ".\n";

	cout << "Сортировка пузырьковым методом заняла " << minutes << " минут " << seconds << " секунд.\n";
}

void shaker(int massC[], int N)
{
	clock_t time_req;
	time_req = clock();

	int left, right, i;
	left = 0;
	right = N - 1;
	while (left <= right) {
		for (i = right; i >= left; i--) {
			if (massC[i - 1] > massC[i]) {
				swap(massC[i - 1], massC[i]);
			}
		}
		left++;
		for (i = left; i <= right; i++) {
			if (massC[i - 1] > massC[i]) {
				swap(massC[i - 1], massC[i]);
			}
		}
		right--;
	}

	time_req = clock() - time_req;
	int minutes = time_req / CLOCKS_PER_SEC / 60;
	int seconds = time_req / CLOCKS_PER_SEC % 60;

	cout << "\nПолучившийся массив C:";
	for (int i = 0; i < N; i++)
	{
		cout << " ";
		cout << massC[i];
	}
	cout << ".\n";

	cout << "Сортировка методом Шелла заняла " << setw(2) << setfill('0') << minutes << " минут " << setw(2) << setfill('0') << seconds << " секунд.\n";
}

void choose(int massD[], int N)
{
	clock_t time_req;
	time_req = clock();

	int min, buf;

	for (int i = 0; i < N; i++) {
		min = i;

		for (int j = i + 1; j < N; j++)
			min = (massD[j] < massD[min]) ? j : min;

		if (i != min) {
			buf = massD[i];
			massD[i] = massD[min];
			massD[min] = buf;
		}
	}

	time_req = clock() - time_req;
	int minutes = time_req / CLOCKS_PER_SEC / 60;
	int seconds = time_req / CLOCKS_PER_SEC % 60;

	cout << "\nПолучившийся массив D:";
	for (int i = 0; i < N; i++)
	{
		cout << " ";
		cout << massD[i];
	}
	cout << ".\n";

	cout << "Сортировка выборкой заняла " << setw(2) << setfill('0') << minutes << " минут " << setw(2) << setfill('0') << seconds << " секунд.\n";

}

void quickSort(int massE[], int first, int last) {
	if (first >= last) {
		return;
	}

	int pivot = massE[(first + last) / 2];
	int left = first;
	int right = last;

	while (left <= right) {
		while (massE[left] < pivot) {
			left++;
		}
		while (massE[right] > pivot) {
			right--;
		}
		if (left <= right) {
			int temp = massE[left];
			massE[left] = massE[right];
			massE[right] = temp;
			left++;
			right--;
		}
	}

	if (first < right) {
		quickSort(massE, first, right);
	}
	if (left < last) {
		quickSort(massE, left, last);
	}
}


void fast(int massE[], int N)
{
	clock_t time_req;
	time_req = clock();

	quickSort(massE, 0, N - 1);

	time_req = clock() - time_req;

	cout << "\nПолучившийся массив E:";
	for (int i = 0; i < N; i++)
	{
		cout << " ";
		cout << massE[i];
	}
	cout << ".\n";

	int minutes = time_req / CLOCKS_PER_SEC / 60;
	int seconds = time_req / CLOCKS_PER_SEC % 60;
	cout << "Быстрая сортировка заняла " << setw(2) << setfill('0') << minutes << " минут " << setw(2) << setfill('0') << seconds << " секунд.\n";
}

int main()
{
	setlocale(LC_CTYPE, "rus");
	int N, choice;

	do
	{
		cout << "1. Сортировка.\n" << "2. Выход.\n\n" << "Ваш выбор: ";
		cin >> choice;
		system("cls");

		if (choice == 1)
		{
			cout << "Сортировка массивов числовых данных.\n\n"
				<< "Введите N (количеество чисел): ";
			cin >> N;
			int* massA = new int[N];

			cout << "Получившийся массив:";
			for (int i = 0; i < N; i++)
			{
				cout << " ";
				massA[i] = 0 + rand() % 1000;
				cout << massA[i];
			}
			cout << ".\n";

			int* massB; massB = massA;
			int* massC; massC = massA;
			int* massD; massD = massA;
			int* massE; massE = massA;

			bubble(massB, N);
			shaker(massC, N);
			choose(massD, N);
			fast(massE, N);

			cout << endl << endl;
			
			delete[] massA;
		}
		else
		{
			cout << "До свидания!\n\n";
		}

	} while (choice != 2);

	return 0;
}