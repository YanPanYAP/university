#include <string>
#include <iostream>
#include <chrono>
#include <Windows.h>

using namespace std;
#define INT_MAX 32767 
#define NUM 13

struct arr {
    string str = "\0";
    int num = 0;
    arr* next = NULL;
};

int hash_f(string str) {
    int sum = 0;
    for (int i = 0; i < str.length(); i++)
    {
        sum += str[i];
    }

    return sum % NUM;
}

void output(arr* tab, int size) {
    for (int i = 0; i < size; i++) {
        arr* ptr = &tab[i];
        cout << "������� " << i << ":\n";
        do {
            if (ptr->str[0] == '\0') {
                cout << "�����\n";
                break;
            }
            else {
                cout << "���: " << ptr->str << ", �����: " << ptr->num << endl;
            }
        } while (ptr = ptr->next);
        cout << endl;
    }
}

void dlt(arr* tab, int size, int num) {
    int k = hash_f(to_string(num));
    while (k > size - 1) k -= size;
    arr* ptr = &tab[k];
    if (ptr->num == num) {
        if (ptr->next) {
            arr* temp = ptr->next;
            ptr->num = temp->num;
            ptr->str = temp->str;
            ptr->next = temp->next;
            delete temp;
        }
        else {
            ptr->num = 0;
            ptr->str = '\0';
        }
        return;
    }
    if (ptr->num) {
        arr* ptr_prev = nullptr;
        while (ptr->next && ptr->num != num) {
            ptr_prev = ptr;
            ptr = ptr->next;
        }
        if (ptr->num == num) {
            if (ptr_prev)
                ptr_prev->next = ptr->next;
            delete ptr;
            return;
        }
    }
    cout << "�������� ���\n";
    return;
}

void input(arr* tab, int size, string str, int num) {
    int k = hash_f(to_string(num));
    while (k > size - 1) k -= size;
    arr* ptr = &tab[k];
    if (!ptr->num) {
        ptr->next = NULL;
        ptr->str = str;
        ptr->num = num;
    }
    else {
        while (ptr->next) ptr = ptr->next;
        arr* p = new arr;
        p->next = NULL;
        p->str = str;
        p->num = num;
        ptr->next = p;
    }
}

void search(arr* tab, int size, int num) {
    int k = hash_f(to_string(num));
    while (k > size - 1) k -= size;
    arr* ptr = &tab[k];
    bool found = false;
    while (ptr) {
        if (ptr->num == num) {
            cout << "����: " << ptr->num << " ���: " << ptr->str << endl;
            found = true;
        }
        ptr = ptr->next;
    }
    if (!found) {
        cout << "�������� ���\n";
    }
}


void main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int size = 7;
    cout << "������� ������ ���-�������: ";
    cin >> size;
    arr* mas = new arr[size];
    int choise;
    do {
        system("cls");
        cout << "1. ����� ���-�������\n2. ���������� ��������\n3. ����� ��������\n4. ��������\n0. �����\n\n��� �����: ";
        cin >> choise;
        system("cls");
        switch (choise) {
        case 1: {
            output(mas, size);
            break;
        }
        case 2: {
            string str;
            int num;
            cout << "������� ���: ";
            cin.ignore();
            getline(cin, str);
            cout << "������� ����� ��������: ";
            cin >> num;

            if (num > 0 && num < INT_MAX) {
                input(mas, size, str, num);
            }
            else {
                cout << "������������ �����\n";
            }
            break;
        }
        case 3: {
            cout << "������� ����: ";
            int num;
            cin >> num;
            cin.ignore();
            auto start = chrono::high_resolution_clock::now();
            search(mas, size, num);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<float> duration = end - start;
            cout << "��������� �����: " << duration.count() << " ������" << endl;
            break;
        }
        case 4: {
            cout << "������� ����: ";
            int num;
            cin >> num;
            dlt(mas, size, num);
            break;
        }
        default:
            break;
        }
        system("pause");
    } while (choise);
}
