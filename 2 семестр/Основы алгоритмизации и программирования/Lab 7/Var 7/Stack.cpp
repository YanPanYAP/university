#include "Stack.h"

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
        cout << "���� ����." << endl;
        return;
    }
    Node<T>* temp = top;
    top = top->next;
    delete temp;
}

template<typename T>
T Stack<T>::peek() {
    if (isEmpty()) {
        cerr << "���� ����." << endl;
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
        cout << "���� �������� � ����." << endl;
    }
    else {
        cout << "�� ���������." << endl;
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
        cout << "���� �������� �� �����." << endl;
    }
    else {
        cout << "���� �� ������." << endl;
    }
}

template<typename T>
void Stack<T>::removeMultiplesOfThree() {
    if (isEmpty()) {
        cerr << "���� ����." << endl;
        return;
    }

    if (typeid(T) != typeid(int) && typeid(T) != typeid(float)) {
        cerr << "�������� �� ���������." << endl;
        return;
    }

    Node<T>* current = top;
    Node<T>* previous = nullptr;

    while (current != nullptr) {
        if (current->data - static_cast<int>(current->data / 3) * 3 != 0) {
            previous = current;
            current = current->next;
        }
        else {
            if (previous == nullptr) {
                pop();
                current = top;
            }
            else {
                previous->next = current->next;
                delete current;
                current = previous->next;
            }
        }
    }
}



template<typename T>
void Stack<T>::print() {
    if (isEmpty()) {
        cout << "���� ����." << endl;
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
