#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <windows.h>
using namespace std;

// Узел дерева Хаффмана
struct Node {
    char symbol;
    int freq;
    Node* left;
    Node* right;
};

// Функция для создания нового узла
Node* createNode(char symbol, int freq, Node* left = nullptr, Node* right = nullptr) {
    Node* node = new Node();
    node->symbol = symbol;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;  // возвращаем указатель на новый узел
}

// Функция для сравнения двух узлов (для приоритетной очереди)
struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;  // Возвращаем результат сравнения по частоте
    }
};

// Построение кодов Хаффмана
void buildCodes(Node* root, string code, map<char, string>& huffmanCodes) {
    if (root == nullptr) {
        return;
    }

    if (!root->left && !root->right) {
        huffmanCodes[root->symbol] = code;
    }

    buildCodes(root->left, code + "0", huffmanCodes);
    buildCodes(root->right, code + "1", huffmanCodes);
}

// Декодирование
string decode(Node* root, string encodedStr) {
    string decoded = "";
    Node* current = root;

    for (int i = 0; i < encodedStr.length(); i++) {
        if (encodedStr[i] == '0') {
            current = current->left;
        }
        else {
            current = current->right;
        }

        // Проверка на NULL
        if (current == nullptr) {
            cout << "Ошибка декодирования: некорректный код.\n";
            return "";
        }

        if (!current->left && !current->right) {
            decoded += current->symbol;
            current = root;
        }
    }

    return decoded;
}

// Освобождение памяти
void freeTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string text;

    cout << "Введите текст: ";
    getline(cin, text);

    // Подсчёт частоты символов
    int chastota[256] = { 0 };
    char symbols[256];
    int n = 0;

    for (int i = 0; i < text.length(); i++) {
        if (chastota[(unsigned char)text[i]] == 0) {
            symbols[n++] = text[i];  // Запоминаем символ
        }
        chastota[(unsigned char)text[i]]++;  // Увеличиваем частоту текущего символа
    }

    // Приоритетная очередь для построения дерева Хаффмана
    priority_queue<Node*, vector<Node*>, Compare> pq;

    // Создаём листья для каждого символа с ненулевой частотой
    for (int i = 0; i < 256; i++) {
        if (chastota[i] > 0) {
            pq.push(createNode(i, chastota[i]));
        }
    }

    // Построение дерева Хаффмана
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        int sum = left->freq + right->freq;
        pq.push(createNode('\0', sum, left, right));
    }

    // Корень дерева Хаффмана
    Node* root = pq.top();

    // Таблица кодов Хаффмана
    map<char, string> huffmanCodes;
    buildCodes(root, "", huffmanCodes);

    // Вывод частоты символов в порядке убывания
    cout << "\nТаблица встречаемости символов:\n";
    for (int i = 0; i < n; i++) {
        if (chastota[(unsigned char)symbols[i]] > 0) {
            cout << symbols[i] << ": " << chastota[(unsigned char)symbols[i]] << "\n";
        }
    }

    // Вывод таблицы соответствия символов кодам
    cout << "\nТаблица кодов Хаффмана:\n";
    for (map<char, string>::iterator it = huffmanCodes.begin(); it != huffmanCodes.end(); it++) {
        cout << it->first << ": " << it->second << "\n";  // Символ и его код
    }

    // Вывод закодированной последовательности
    string encodedStr = "";
    cout << "\nЗакодированная последовательность:\n";
    for (int i = 0; i < text.length(); i++) {
        encodedStr += huffmanCodes[text[i]];
        cout << huffmanCodes[text[i]];
    }
    cout << "\n";

    string decodedStr = decode(root, encodedStr);
    cout << "\nДекодированная строка:\n" << decodedStr << "\n";

    freeTree(root);

    return 0;
}
