#include <iostream>
#include <Windows.h>
using namespace std;

void quickSort(int allProducts[], int first, int last) {
	if (first >= last) {
		return;
	}

	int pivot = allProducts[(first + last) / 2];
	int left = first;
	int right = last;

	while (left <= right) {
		while (allProducts[left] < pivot) {
			left++;
		}
		while (allProducts[right] > pivot) {
			right--;
		}
		if (left <= right) {
			int temp = allProducts[left];
			allProducts[left] = allProducts[right];
			allProducts[right] = temp;
			left++;
			right--;
		}
	}

	if (first < right) {
		quickSort(allProducts, first, right);
	}
	if (left < last) {
		quickSort(allProducts, left, last);
	}
}

int main()
{
	setlocale(LC_CTYPE, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int N, n, sum = 0;

	cout << "������� ���-�����.\n" << "������� ����������� ����� N (<10000) ��� ��������� �������: ";
	cin >> N;
	int* massProducts = new int[N];

	for (int i = 0; i < N; i++)
	{
		massProducts[i] = 1 + rand() % 100;
	}

	int* allProducts; allProducts = massProducts;

	quickSort(allProducts, 0, N - 1);

	if (N % 2 == 0)
	{
		n = N / 2;
	}
	else
	{
		n = N / 2 + 1;
	}

	cout << "\n������� (������ ������ � �������):";
	N--;

	for (int i = 0; i < n; i++)
	{
		cout << " ";
		cout << allProducts[N];
		cout << " ";
		cout << allProducts[i];
		sum += allProducts[N];
		N--;
	}
	cout << "\n����� ����: " << sum;

	delete[] massProducts;

	return 0;
}