#include <iostream>
#include <stack>
using namespace std;

void print_rods(stack<int> rods[], int k) {
    for (int i = 0; i < k; ++i) {
        cout << "�������� " << i + 1 << ": ";

        if (rods[i].empty()) {
            cout << "";
        }
        else {
            stack<int> temp = rods[i];
            stack<int> disks;
            while (!temp.empty()) {
                disks.push(temp.top());
                temp.pop();
            }
            while (!disks.empty()) {
                cout << disks.top() << " ";
                disks.pop();
            }
        }

        cout << endl;
    }
    cout << endl;
}

bool move_disk(stack<int> rods[], int from, int to) {
    if (rods[from].empty()) {
        cout << "������: �� ������� " << from + 1 << " ��� ������.\n";
        return false;
    }
    if (!rods[to].empty() && rods[to].top() < rods[from].top()) {
        cout << "������: ������ ����������� ������� ���� �� �������.\n";
        return false;
    }

    rods[to].push(rods[from].top());
    rods[from].pop();
    return true;
}

int main() {
    setlocale(LC_CTYPE, "rus");
    int N, k;

    cout << "������� ���������� ������ (N): ";
    cin >> N;
    cout << "������� ���������� �������� (k): ";
    cin >> k; cout << endl;

    stack<int> rods[10];
    for (int i = N; i >= 1; --i) {
        rods[0].push(i);
    }

    while (true) {
        print_rods(rods, k);

        int from, to;
        cout << "�������� ��������, � �������� ������ ����������� ����: ";
        cin >> from;

        if (from < 1 || from > k) {
            system("cls");
            cout << "������: ������������ ����� �������.\n\n";
            continue;
        }
        from--;

        cout << "�������� ��������, �� ������� ������ ����������� ����: ";
        cin >> to;

        if (to < 1 || to > k) {
            system("cls");
            cout << "������: ������������ ����� �������.\n\n";
            continue;
        }
        to--;

        system("cls");
        if (!move_disk(rods, from, to)) {
            cout << "����������� ����������. ���������� �����.\n\n";
            continue;
        }
    }

    return 0;
}
