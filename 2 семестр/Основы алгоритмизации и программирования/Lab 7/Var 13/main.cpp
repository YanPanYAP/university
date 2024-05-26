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
        cout << "1. Добавить элемент\n";
        cout << "2. Очистить стек\n";
        cout << "3. Сохранить стек в файл\n";
        cout << "4. Загрузить стек из файла\n";
        cout << "5. Подсчитать количество повторяющихся элементов\n";
        cout << "6. Вывести стек\n";
        cout << "0. Выход\n";
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Введите значение: ";
            cin >> value;
            stack.push(value);
            break;
        case 2:
            stack.clear();
            cout << "Стек очищен." << endl;
            break;
            break;
        case 3:
            cout << "Введите имя файла: ";
            cin >> filename;
            stack.saveToFile(filename);
            break;
        case 4:
            cout << "Введите имя файла: ";
            cin >> filename;
            stack.readFromFile(filename);
            break;
        case 5:
            cout << "Количество повторяющихся элементов: " << stack.countDuplicates() << endl;
            break;
        case 6:
            cout << "Стек: ";
            stack.print();
            break;
        case 0:
            return 0;
        default:
            cout << "Неправильный выбор." << endl;
        }
        cout << endl;
    }

    return 0;
}
