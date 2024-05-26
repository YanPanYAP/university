#include "StackFunctions.h"

template<typename T>
Stack<T>::Stack() : top(nullptr) {}

template<typename T>
Stack<T>::~Stack() {
    clear();
}

template<typename T>
void Stack<T>::push(T val) {
    Node<T>* newNode = new Node<T>(val);
    newNode->next = top;
    top = newNode;
}

template<typename T>
void Stack<T>::pop() {
    if (isEmpty()) {
        cout << "Стэк пуст." << endl;
        return;
    }
    Node<T>* temp = top;
    top = top->next;
    delete temp;
}

template<typename T>
T Stack<T>::peek() {
    if (isEmpty()) {
        cerr << "Стэк пуст." << endl;
        exit(EXIT_FAILURE);
    }
    return top->data;
}

template<typename T>
bool Stack<T>::isEmpty() {
    return top == nullptr;
}

template<typename T>
void Stack<T>::clear() {
    while (!isEmpty()) {
        pop();
    }
}

template<typename T>
void Stack<T>::saveToFile(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        Node<T>* current = top;
        while (current != nullptr) {
            file << current->data << " ";
            current = current->next;
        }
        file.close();
        cout << "Стэк сохранен в файл." << endl;
    }
    else {
        cout << "Не сохранено." << endl;
    }
}

template<typename T>
void Stack<T>::readFromFile(const string& filename) {
    clear();
    ifstream file(filename);
    if (file.is_open()) {
        T value;
        while (file >> value) {
            push(value);
        }
        file.close();
        cout << "Стэк добавлен из файла." << endl;
    }
    else {
        cout << "Файл не открыт." << endl;
    }
}

template<typename T>
void Stack<T>::removeFirstDuplicate() {
    if (isEmpty()) {
        cerr << "Стэк пуст." << endl;
        return;
    }

    Node<T>* current = top;
    Node<T>* prev = nullptr;
    bool foundDuplicate = false;

    while (current != nullptr && !foundDuplicate) {
        Node<T>* runner = top;
        while (runner != current) {
            if (runner->data == current->data) {
                foundDuplicate = true;
                break;
            }
            runner = runner->next;
        }
        if (foundDuplicate) {
            if (prev == nullptr) {
                pop();
            }
            else {
                prev->next = current->next;
                delete current;
            }
        }
        else {
            prev = current;
            current = current->next;
        }
    }
}

template<typename T>
void Stack<T>::print() {
    if (isEmpty()) {
        cout << "Стэк пуст." << endl;
        return;
    }
    Node<T>* current = top;
    while (current != nullptr) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}

template class Stack<int>;
template class Stack<float>;
template class Stack<char>;