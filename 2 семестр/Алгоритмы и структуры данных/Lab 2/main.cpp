#include <iostream>
using namespace std;

const int A = 100;
int mass[A] = {};
int counter = 0;

void addToMass(int X)
{
	mass[counter] = X;
	counter++;
}

void findX(int X, int right, int left)
{
	int choiceFind;

	cout << "Ваше число: " << X << ".";
	cout << "\n\n1. Мало.\n2. Много.\n3. Угадал.\n\nВаш выбор: ";
	cin >> choiceFind;
	system("cls");

	switch (choiceFind)
	{
	case 1:
		addToMass(X);
		left = X;
		X = (left + right) / 2;
		findX(X, right + 1, left);
		break;
	case 2:
		addToMass(X);
		right = X;
		X = (left + right) / 2;
		findX(X, right, left - 1);
		break;
	case 3:
		cout << "Ваше число отгадано: " << X << ".\n";
		cout << "Все шаги бинарного поиска: ";
		for (int i = 0; i < counter; i++)
		{
			cout << mass[i] << ", ";
		}
		cout << X << ".\n";
		break;
	default:
		cout << "Ошибка!";
		break;
	}
}

int maxSteps(int step)
{
	int steps = 0;
	while (step >= 1)
	{
		step /= 2;
		steps++;
	}

	return steps;
}

int main()
{
	setlocale(LC_CTYPE, "rus");
	int right, X, choice, left = 1, step;

	do
	{
		cout << "1. Угадайка.\n2. Выход.\n\nВаш выбор: ";
		cin >> choice;
		system("cls");

		if (choice == 1)
		{
			counter = 0;
			cout << "Введите число N: ";
			cin >> right;
			if (right == 0)
			{
				system("cls");
				cout << "Ошибка!\n\n";
				continue;
			}
			step = right;
			system("cls");
			X = (left + right) / 2;
			findX(X, right, left);
			cout << "Максимальное количество шагов: " << maxSteps(step) << endl << endl;
		}
		else
		{
			cout << "До свидания!\n\n";
		}

	} while (choice != 2);

	return 0;
}