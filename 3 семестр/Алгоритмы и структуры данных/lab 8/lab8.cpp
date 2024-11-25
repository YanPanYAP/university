#include <iostream>
#include <vector>
#include <locale>
#include <string>
using namespace std;

struct Item {
    wstring name;
    int weight;
    int value;
};

void addObject(vector<Item>& items) {
    Item newItem;
    wcout << L"Введите название предмета: ";
    wcin >> ws;
    getline(wcin, newItem.name);
    wcout << L"Введите вес предмета: ";
    cin >> newItem.weight;
    wcout << L"Введите стоимость предмета: ";
    cin >> newItem.value;
    items.push_back(newItem);
}

int resualt(int capacity, vector<Item>& items, vector<Item>& selectedItems) {
    int n = items.size();
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int w = 0; w <= capacity; ++w) {
            if (items[i - 1].weight <= w) {
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - items[i - 1].weight] + items[i - 1].value);
            }
            else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    int w = capacity;
    for (int i = n; i > 0 && w > 0; --i) {
        if (dp[i][w] != dp[i - 1][w]) {
            selectedItems.push_back(items[i - 1]);
            w -= items[i - 1].weight;
        }
    }

    return dp[n][capacity];
}

int main() {
    wcout.imbue(locale("ru_RU.UTF-8"));
    wcin.imbue(locale("ru_RU.UTF-8"));

    int capacity;
    vector<Item> items;

    wcout << L"Введите вместимость рюкзака: ";
    cin >> capacity;

    while (true) {
        wcout << L"\n1. Добавить предмет\n2. Закончить добавление\nВаш выбор: ";
        int choice;
        cin >> choice;
        if (choice == 1) {
            system("cls");
            addObject(items);
        }
        else if (choice == 2) {
            system("cls");
            break;
        }
        else {
            wcout << L"Ошибка: неверный ввод\n";
        }
    }

    vector<Item> selectedItems;
    int maxValue = resualt(capacity, items, selectedItems);

    wcout << L"Максимальная стоимость предметов в рюкзаке: " << maxValue << L"\n";
    wcout << L"\nВыбранные предметы:\n";
    for (const auto& item : selectedItems) {
        wcout << L"Название: " << item.name << L", Вес: " << item.weight << L", Стоимость: " << item.value << L"\n";
    }

    return 0;
}
