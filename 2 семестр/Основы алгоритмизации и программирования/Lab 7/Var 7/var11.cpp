#include "Stack.h"

int main() {
    setlocale(LC_CTYPE, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Stack<int> stack;
    int choice;
    int value;
    string filename;

    while (true) {
        cout << "1. Добавить элемент\n";
        cout << "2. Улалить делющиеся на три\n";
        cout << "3. Сохранить в файл\n";
        cout << "4. Загрузить из файла\n";
        cout << "5. Вывести стэк\n";
        cout << "6. Очистить стэк\n";
        cout << "0. Выход\n";
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Введите значение: ";
            cin >> value;
            stack.push(value);
            break;
        case 2:
            stack.removeMultiplesOfThree();
            cout << "Элементы удалены." << endl;
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
            cout << "Стэк: ";
            stack.print();
            break;
        case 6:
            stack.clear();
            cout << "Стек очищен." << endl;
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
