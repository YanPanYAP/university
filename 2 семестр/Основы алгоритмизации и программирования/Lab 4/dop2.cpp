#include <iostream>
#include <string>
#include <algorithm>
#include <Windows.h>
using namespace std;

struct TRAIN
{
    string destination;
    int number;
    string departure_time;
};

bool compareByDestination(const TRAIN& a, const TRAIN& b)
{
    return a.destination < b.destination;
}

void sortTrains(TRAIN trains[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (compareByDestination(trains[j], trains[j + 1]))
            {
                swap(trains[j], trains[j + 1]);
            }
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const int SIZE = 8;
    TRAIN trains[SIZE];

    for (int i = 0; i < SIZE; i++)
    {
        cout << "������� ����� ����������, ����� ������ � ����� ����������� ��� ������ " << i + 1 << ": ";
        cin >> trains[i].destination >> trains[i].number >> trains[i].departure_time;
    }

    sortTrains(trains, SIZE);
    string input_time;
    cout << "������� �����: ";
    cin >> input_time;

    bool found = false;
    for (int i = 0; i < SIZE; i++)
    {
        if (trains[i].departure_time > input_time)
        {
            found = true;
            cout << "����� ����� " << trains[i].number << " ������������ � " << trains[i].departure_time << " � ����������� " << trains[i].destination << ".\n";
        }
    }

    if (!found)
    {
        cout << "�������, �������������� ����� ���������� �������, �� �������.\n";
    }

    return 0;
}