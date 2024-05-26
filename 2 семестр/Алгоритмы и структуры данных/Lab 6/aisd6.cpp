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
        cout << "Уровень " << i << ":\n";
        do {
            if (ptr->str[0] == '\0') {
                cout << "Пусто\n";
                break;
            }
            else {
                cout << "ФИО: " << ptr->str << ", номер: " << ptr->num << endl;
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
    cout << "Элемента нет\n";
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
            cout << "Ключ: " << ptr->num << " ФИО: " << ptr->str << endl;
            found = true;
        }
        ptr = ptr->next;
    }
    if (!found) {
        cout << "Элемента нет\n";
    }
}


void main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int size = 7;
    cout << "Введите размер хеш-таблицы: ";
    cin >> size;
    arr* mas = new arr[size];
    int choise;
    do {
        system("cls");
        cout << "1. Вывод хеш-таблицы\n2. Добавление элемента\n3. Поиск элемента\n4. Удаление\n0. Выход\n\nВаш выбор: ";
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
            cout << "Введите ФИО: ";
            cin.ignore();
            getline(cin, str);
            cout << "Введите номер телефона: ";
            cin >> num;

            if (num > 0 && num < INT_MAX) {
                input(mas, size, str, num);
            }
            else {
                cout << "Некорректный номер\n";
            }
            break;
        }
        case 3: {
            cout << "Введите ключ: ";
            int num;
            cin >> num;
            cin.ignore();
            auto start = chrono::high_resolution_clock::now();
            search(mas, size, num);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<float> duration = end - start;
            cout << "Расчетное время: " << duration.count() << " секунд" << endl;
            break;
        }
        case 4: {
            cout << "Введите ключ: ";
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
