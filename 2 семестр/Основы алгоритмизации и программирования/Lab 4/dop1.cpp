#include<iostream>
#include<string>
using namespace std;

struct student
{
	string fio;
	int num_ex;
	int* marks;
};

bool zachet(student a);
double prosent(student* a, int n);

void main()
{
	setlocale(LC_ALL, "rus");
	student a[10];
	cout << "������� ���������� ���������: "; int n; cin >> n;

	for (int i = 0; i < n; i++)
	{
		cout << "\n���: "; cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, a[i].fio);
		cout << "���������� ���������: "; cin >> a[i].num_ex;
		cout << "������: ";
		
		a[i].marks = new int[a[i].num_ex];
		for (int j = 0; j < a[i].num_ex; j++)
		{
			cin >> a[i].marks[j];
		}
	}

	int q;
	cout << "\n������� ����� ��������, �������� ����� ���������: "; cin >> q;

	if (zachet(a[q - 1]) == true)
	{
		cout << "������ ������� ������� ������ ������!" << endl;
	}
	else
	{
		cout << "������ ������� �� ���� ������!" << endl;
	}
	cout << "���������� ��������� ���������� � ������ ���������� ���������: " << prosent(a, n) << "%" << endl;
}

bool zachet(student a)
{
	int count = a.num_ex;
	for (int j = 0; j < count; j++)
	{
		if (a.marks[j] < 4)
		{
			return false;
		}
	}
	return true;
}

double prosent(student* a, int n)
{
	double good = 0;
	int all = 0;
	for (int i = 0; i < n; i++)
	{
		bool flag = true;
		int count = a[i].num_ex;
		for (int j = 0; j < count; j++)
		{
			if (a[i].marks[j] < 4)
			{
				flag = false;
				break;
			}
		}
		if (flag == true)
	{
			good++;
		}
	}

	double pros = (good / n) * 100;
	return pros;
}