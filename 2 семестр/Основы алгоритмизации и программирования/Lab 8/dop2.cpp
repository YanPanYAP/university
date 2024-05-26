#include <iostream>
using namespace std;

struct Queue {
	float num;
	Queue* next;
};

void into(Queue** begin, Queue** end, float p)
{
	Queue* t = new Queue;
	t->num = p;
	t->next = NULL;
	if (*begin == NULL) {
		*begin = *end = t;
	}
	else
	{

		(*end)->next = t;
		*end = t;
	}
}

void from(Queue* begin)
{
	Queue* t = begin;
	if (t == NULL)
	{
		cout << "������� �����\n";
	}
	else {
		while (t != NULL)
		{
			cout << t->num << " ";
			t = t->next;
		}
	}
	cout << endl;
}

void function(Queue*& begin, Queue*& end)
{
	if (begin == nullptr)
	{
		cout << "������� �����\n";
		return;
	}
	Queue* Max = begin;
	Queue* PrevMax = NULL;
	Queue* t = begin->next;
	float max = begin->num;
	while (t)
	{
		if (t->num > max)
		{
			max = t->num;
			Max = t;
		}
		t = t->next;
	}
	if (Max != begin)
	{
		t = begin;
		while (t->next != Max)
		{
			t = t->next;
		}
		t->next = Max->next;
		Max->next = begin;
		begin = Max;
		if (t->next == NULL)
		{
			end = t;
		}
	}
}

int main()
{
	setlocale(LC_ALL, "RU");

	Queue* begin = NULL, * end, * t;
	t = new Queue;
	int size, counter = 1, choice;
	float p;
	cout << "������� ������������ ������ �������: ";
	cin >> size;

	do
	{
		cout << "\n����: " << endl;
		cout << "1.���������� ��������� � �������" << endl;
		cout << "2.����� ������� �� �����" << endl;
		cout << "3.����� �������" << endl;
		cout << "0.���������� ������" << endl;
		cout << "��� �����: ";
		cin >> choice;
		system("cls");

		switch (choice) {
			case 1:
			{
				if (counter <= size)
				{
					cout << "������� ����� ��� ���������� � �������: ";
					cin >> p;
					into(&begin, &end, p);
					counter++;

				}
				else
				{
					cout << "����� � ������� ���.\n";
				}
				break;
			}
			case 2:
			{
				cout << "�������: ";
				from(begin);
				break;
			}
			case 3:
			{
				cout << "����� ��������.\n";
				function(begin, end);
				break;
			}
			default:
				cout << "�� ��������.\n";
		}

	} while (choice != 0);

	return 0;
}