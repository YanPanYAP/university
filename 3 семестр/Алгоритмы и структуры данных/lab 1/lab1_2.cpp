#include <iostream>
#include <ctime>
using namespace std;

const int MAX_RODS = 10;
int move_count = 0;

void print_rods(int rods[][MAX_RODS], int rod_sizes[], int k) {
    for (int i = 0; i < k; ++i) {
        cout << "Стержень " << i + 1 << ": ";
        if (rod_sizes[i] == 0) {
            cout << "";
        }
        else {
            for (int j = 0; j < rod_sizes[i]; ++j) {
                cout << rods[i][j] << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void move(int disk, int from, int to, int rods[][MAX_RODS], int rod_sizes[]) {
    move_count++;
    rods[to][rod_sizes[to]] = disk;
    rod_sizes[to]++;
    rod_sizes[from]--;
    cout << "Переместить диск " << disk << " с " << from + 1 << " на " << to + 1 << " стержень.\n";
}

void hanoi(int n, int from, int to, int aux, int rods[][MAX_RODS], int rod_sizes[]) {
    if (n == 1) {
        move(1, from, to, rods, rod_sizes);
        return;
    }
    hanoi(n - 1, from, aux, to, rods, rod_sizes);
    move(n, from, to, rods, rod_sizes);
    hanoi(n - 1, aux, to, from, rods, rod_sizes);
}

int main() {
    setlocale(LC_CTYPE, "rus");
    while (true) {
        int N, k, start, target;

        cout << "Введите количество дисков (N): ";
        cin >> N;
        if (cin.fail() || N <= 0) {
            cout << "Ошибка: количество дисков должно быть положительным числом.\n\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Введите количество стержней (k): ";
        cin >> k;
        if (cin.fail() || k < 3) {
            cout << "Ошибка: количество стержней должно быть 3 или больше.\n\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Введите номер начального стержня (от 1 до " << k << "): ";
        cin >> start;
        if (cin.fail() || start < 1 || start > k) {
            cout << "Ошибка: некорректный номер начального стержня.\n\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Введите номер стержня, на который хотите переместить башню (от 1 до " << k << "): ";
        cin >> target;
        if (cin.fail() || target < 1 || target > k || target == start) {
            cout << "Ошибка: некорректный номер целевого стержня или он совпадает с начальным.\n\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        start--;
        target--;

        int aux = -1;
        for (int i = 0; i < k; ++i) {
            if (i != start && i != target) {
                aux = i;
                break;
            }
        }

        int rods[MAX_RODS][MAX_RODS] = { 0 };
        int rod_sizes[MAX_RODS] = { 0 };
        for (int i = N; i >= 1; --i) {
            rods[start][rod_sizes[start]++] = i;
        }

        cout << "\nНачальное состояние башни:\n";
        print_rods(rods, rod_sizes, k);

        clock_t start_time = clock();

        hanoi(N, start, target, aux, rods, rod_sizes);

        clock_t end_time = clock();

        cout << "\nКонечное состояние башни:\n";
        print_rods(rods, rod_sizes, k);

        cout << "Всего ходов: " << move_count << endl;

        double time_spent = double(end_time - start_time) / CLOCKS_PER_SEC;
        cout << "Время выполнения: " << time_spent << " секунд" << endl << endl;
    }
    return 0;
}
