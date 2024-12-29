#include <iostream>
#include <string>
#include <stack>
using namespace std;

// Функция для проверки приоритета операторов
int getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Функция для преобразования выражения в обратную польскую запись
void PolishNotation(string str) {
    stack<char> st;
    string result;

    for (char c : str) {
        if (isalnum(c)) {
            // Если символ — операнд, добавляем его к результату
            result += c;
        }
        else if (c == '(') {
            // Если символ — открывающая скобка, помещаем её в стек
            st.push(c);
        }
        else if (c == ')') {
            // Если символ — закрывающая скобка, извлекаем из стека до открывающей скобки
            while (!st.empty() && st.top() != '(') {
                result += st.top();
                st.pop();
            }
            if (!st.empty() && st.top() == '(') {
                st.pop();
            }
        }
        else {
            // Если символ — оператор, обрабатываем приоритет
            while (!st.empty() && getPrecedence(st.top()) >= getPrecedence(c)) {
                result += st.top();
                st.pop();
            }
            st.push(c);
        }
    }

    // Извлекаем оставшиеся элементы из стека
    while (!st.empty()) {
        result += st.top();
        st.pop();
    }

    cout << result << endl;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    string str;
    cout << "Введите выражение: ";
    cin >> str;
    cout << "\nОбратная польская запись: ";
    PolishNotation(str);
    return 0;
}
