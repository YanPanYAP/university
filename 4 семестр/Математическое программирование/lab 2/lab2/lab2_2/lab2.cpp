#include <iostream>
using namespace std;

int main() {
    char r[][2] = { "A", "B", "C", "D" };
    int n = sizeof(r) / sizeof(r[0]);

    cout << endl << " --- Генератор размещений ---" << endl << "Исходное множество: " << "{ ";
    for (int i = 0; i < n; i++) {
        cout << r[i] << (i < n - 1 ? ", " : " ");
    }
    cout << "}" << endl;

    cout << "Все размещения из " << n << " по 2:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                cout << r[i] << " " << r[j] << endl;
            }
        }
    }

    return 0;
}