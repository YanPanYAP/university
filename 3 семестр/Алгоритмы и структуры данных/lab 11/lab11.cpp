#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

bool algoritm(int boxes[], int rounds) {
    int count = 0;
    for (int i = 0; i < 100; i++) {
        int find = boxes[i];
        if (find == i) {
            count++;
            continue;
        }
        for (int j = 1; j < rounds; j++) {
            find = boxes[find];
            if (find == i) {
                count++;
                break;
            }
        }
    }

    return count == 100;
}

bool noalgoritm(int boxes[], int rounds) {
    int count = 0;
    for (int i = 0; i < 100; i++) {
        vector<bool> open(100, false);
        for (int j = 0; j < rounds; j++) {
            int temp = rand() % 100;
            while (open[temp]) temp = rand() % 100;
            open[temp] = true;
            if (boxes[temp] == i) {
                count++;
                break;
            }
        }
    }

    return count == 100;
}

int* gen() {
    static int boxes[100];
    vector<bool> mes(100, false);
    for (int i = 0; i < 100; i++) {
        int temp = rand() % 100;
        while (mes[temp]) {
            temp = rand() % 100;
        }
        mes[temp] = true;
        boxes[i] = temp;
    }
    return boxes;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    int rounds;
    cout << "Введите количество раундов: ";
    cin >> rounds;

    int count1 = 0;
    int count2 = 0;

    for (int i = 0; i < rounds; i++) {
        int* boxes = gen();
        if (algoritm(boxes, 50)) count1++;
        if (noalgoritm(boxes, 50)) count2++;
    }

    cout << "Количество успехов при алгоритме: " << count1 << endl;
    cout << "Количество успехов при случайном выборе: " << count2 << endl;

    return 0;
}
