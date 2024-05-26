#include <iostream>
#include <ctime>
using namespace std;


void divideCountries(int country, int** A, int* groups, int j, bool* visited)
{
    visited[country] = true;
    for (int n = 0; n < j; n++)
    {
        if (A[country][n] == 1 && groups[country] == groups[n])
        {
            groups[n] = !groups[country];

            if (!visited[n])
            {
                divideCountries(n, A, groups, j, visited);
            }  
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(0));
    int i, j;
    cout << "Количество строк: "; cin >> i;
    cout << "Количество столбцов: "; cin >> j;
    system("cls");

    int** A = new int* [i];
    cout << endl << "Матрица стран:" << endl;
    for (int k = 0; k < i; k++)
    {
        A[k] = new int[j];
        for (int n = 0; n < j; n++)
        {
            A[k][n] = rand() % 2;
            cout << A[k][n] << " ";
        }
        cout << endl;
    }

    int* groups = new int[i];
    for (int k = 0; k < i; k++)
    {
        groups[k] = 0;
    }

    bool* visited = new bool[i];
    for (int k = 0; k < i; k++)
    {
        visited[k] = false;
    }
       
    for (int k = 0; k < i; k++)
    {
        divideCountries(k, A, groups, j, visited);
    }

    cout << endl << "Группы стран:" << endl;
    for (int k = 0; k < i; k++)
    {
        cout << "Страна " << k + 1 << " в группе " << groups[k] << endl;
    }

    for (int k = 0; k < i; k++)
    {
        delete[] A[k];
        delete[] A;
        delete[] groups;
        delete[] visited;
    }

    return 0;
}
