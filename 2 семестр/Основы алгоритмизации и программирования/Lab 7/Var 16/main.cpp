#include "StackFunctions.h"
#include <Windows.h>

int main() {
    setlocale(LC_CTYPE, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Stack<char> stack;
    int choice;
    char value;
    string filename;

    while (true) {
        cout << "1. �������� �������\n";
        cout << "2. ������� ������ ������������� �������\n";
        cout << "3. ��������� � ����\n";
        cout << "4. ��������� �� �����\n";
        cout << "5. ������� ����\n";
        cout << "6. �������� ����\n";
        cout << "0. �����\n";
        cout << "��� �����: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "������� ��������: ";
            cin >> value;
            stack.push(value);
            break;
        case 2:
            stack.removeFirstDuplicate();
            cout << "������ ������������� ������� ������." << endl;
            break;
        case 3:
            cout << "������� ��� �����: ";
            cin >> filename;
            stack.saveToFile(filename);
            break;
        case 4:
            cout << "������� ��� �����: ";
            cin >> filename;
            stack.readFromFile(filename);
            break;
        case 5:
            cout << "����: ";
            stack.print();
            break;
        case 6:
            stack.clear();
            cout << "���� ������." << endl;
            break;
        case 0:
            return 0;
        default:
            cout << "������������ �����." << endl;
        }
        cout << endl;
    }

    return 0;
}
