#include <iostream>
#include <climits>

using namespace std;

void printMatrix(int matrix[6][6], int size, const string& name) {
    cout << "������� " << name << ":\n";
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (matrix[i][j] == INT_MAX)
                cout << "INF" << "\t";
            else
                cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

void printPath(int S[6][6], int start, int end) {
    if (start == end) {
        cout << start + 1 << " ";
        return;
    }

    if (S[start][end] == end + 1) {
        cout << start + 1 << " -> " << end + 1 << " ";
        return;
    }

    printPath(S, start, S[start][end] - 1);
    cout << end + 1 << " ";
}

int main() {
    setlocale(LC_ALL, "rus");

    const int size = 6;

    int D[size][size] = {
        { 0, 28, 21, 59, 12, 27 },
        { 7, 0, 24, INT_MAX, 21, 9 },
        { 9, 32, 0, 13, 11, INT_MAX },
        { 8, INT_MAX, 5, 0, 16, INT_MAX },
        { 14, 13, 15, 10, 0, 22 },
        { 15, 18, INT_MAX, INT_MAX, 6, 0 },
    };

    int S[size][size];

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) {
                S[i][j] = 0;
            }
            else {
                S[i][j] = j + 1;
            }
        }
    }

    printMatrix(D, size, "D (��������� ������� ���������� ����� ����� ���������)");
    printMatrix(S, size, "S (��������� ������� ������������������ ������)");

    int Sum;
    for (int k = 0; k < size; k++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (D[i][k] != INT_MAX && D[k][j] != INT_MAX) {
                    Sum = D[i][k] + D[k][j];
                    if (Sum < D[i][j]) {
                        D[i][j] = Sum;
                        S[i][j] = k + 1;
                    }
                }
            }
        }
    }

    printMatrix(D, size, "D (�������� ������� ���������� ����� ����� ���������)");
    printMatrix(S, size, "S (�������� ������� ������������������ ������)");

    int startVertex, endVertex;
    cout << "������� ��������� ������� (1-" << size << "): ";
    cin >> startVertex;

    if (cin.fail() || startVertex < 1 || startVertex > size) {
        cout << "������ �����: �������� �������� ��������� �������." << endl;
        return 1;
    }

    cout << "������� �������� ������� (1-" << size << "): ";
    cin >> endVertex;

    if (cin.fail() || endVertex < 1 || endVertex > size) {
        cout << "������ �����: �������� �������� �������� �������." << endl;
        return 1;
    }

    startVertex--;
    endVertex--;

    if (D[startVertex][endVertex] == INT_MAX) {
        cout << "����� ��������� " << startVertex + 1 << " � " << endVertex + 1 << " ���� ���." << endl;
    }
    else {
        cout << "���������� ���������� �� ������� " << startVertex + 1 << " �� ������� " << endVertex + 1 << ": " << D[startVertex][endVertex] << endl;
        cout << "���������� ����: ";
        printPath(S, startVertex, endVertex);
        cout << endl;
    }

    return 0;
}
