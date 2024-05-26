#include <iostream>
#include <Windows.h>
using namespace std;

template<typename T>
struct Node {
    T data;
    Node<T>* next;
    Node(T val) : data(val), next(nullptr) {}
};

template<typename T>
class Queue {
private:
    Node<T>* front;
    Node<T>* rear;
    int maxSize;
    int currentSize;

public:
    Queue(int size);
    ~Queue();
    void enqueue(T val);
    T dequeue();
    bool isFull();
    bool isEmpty();
    void clear();
    void print();
    bool remove(T val);
};

bool isLetter(char symbol) {
    return (symbol >= 'a' && symbol <= 'z') ||
        (symbol >= 'A' && symbol <= 'Z');
}


void addElement(Queue<char>& lowercaseQueue, Queue<char>& uppercaseQueue) {
    char symbol;
    cout << "������� ������: ";
    cin >> symbol;
    if (isLetter(symbol)) {
        if (isupper(symbol)) {
            if (!lowercaseQueue.isFull())
                lowercaseQueue.enqueue(tolower(symbol));
            else
                cout << "������� �������� ���� �����." << endl;
        }
        else {
            if (!uppercaseQueue.isFull())
                uppercaseQueue.enqueue(toupper(symbol));
            else
                cout << "������� ��������� ���� �����." << endl;
        }
    }
    else {
        cout << "������������ ������: " << symbol << endl;
    }
}

void printQueues(Queue<char>& lowercaseQueue, Queue<char>& uppercaseQueue) {
    cout << "������� �������� ����: ";
    lowercaseQueue.print();
    cout << "������� ��������� ����: ";
    uppercaseQueue.print();
}

void removeElement(Queue<char>& lowercaseQueue, Queue<char>& uppercaseQueue) {
    char symbol;
    cout << "������� ������ ��� ��������: ";
    cin >> symbol;
    bool removed = lowercaseQueue.remove(symbol) || uppercaseQueue.remove(symbol);
    if (removed) {
        cout << "������ " << symbol << " ������ �� �������." << endl;
    }
    else {
        cout << "������ " << symbol << " �� ������ � ��������." << endl;
    }
}

int main() {
    setlocale(LC_CTYPE, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int size;
    cout << "������� ������������ ������ ��������: ";
    cin >> size;
    Queue<char> lowercaseQueue(size);
    Queue<char> uppercaseQueue(size);

    int choice;
    do {
        cout << "\n����:\n";
        cout << "1. �������� ������ � �������\n";
        cout << "2. ������� ������� �������� � ��������� ����\n";
        cout << "3. ������� ������ �� ����� ��������\n";
        cout << "0. �����\n";
        cout << "�������� ��������: ";
        cin >> choice;
        system("cls");

        switch (choice) {
        case 1:
            addElement(lowercaseQueue, uppercaseQueue);
            break;
        case 2:
            printQueues(lowercaseQueue, uppercaseQueue);
            break;
        case 3:
            removeElement(lowercaseQueue, uppercaseQueue);
            break;
        case 0:
            cout << "����� �� ���������." << endl;
            break;
        default:
            cout << "�������� �����. ���������� ��� ���." << endl;
        }
    } while (choice != 0);

    return 0;
}

template<typename T>
Queue<T>::Queue(int size) : front(nullptr), rear(nullptr), maxSize(size), currentSize(0) {}

template<typename T>
Queue<T>::~Queue() {
    clear();
}

template<typename T>
void Queue<T>::enqueue(T val) {
    if (isFull()) {
        cerr << "������� �����." << endl;
        exit(EXIT_FAILURE);
    }
    Node<T>* newNode = new Node<T>(val);
    if (isEmpty()) {
        front = rear = newNode;
    }
    else {
        rear->next = newNode;
        rear = newNode;
    }
    currentSize++;
}

template<typename T>
T Queue<T>::dequeue() {
    if (isEmpty()) {
        cerr << "������� �����." << endl;
        exit(EXIT_FAILURE);
    }
    T data = front->data;
    Node<T>* temp = front;
    front = front->next;
    delete temp;
    currentSize--;
    return data;
}

template<typename T>
bool Queue<T>::isFull() {
    return currentSize == maxSize;
}

template<typename T>
bool Queue<T>::isEmpty() {
    return currentSize == 0;
}

template<typename T>
void Queue<T>::clear() {
    while (!isEmpty()) {
        dequeue();
    }
}

template<typename T>
void Queue<T>::print() {
    if (isEmpty()) {
        cout << "������� �����." << endl;
        return;
    }
    Node<T>* current = front;
    while (current != nullptr) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}

template<typename T>
bool Queue<T>::remove(T val) {
    if (isEmpty()) {
        return false;
    }

    if (front->data == val) {
        Node<T>* temp = front;
        front = front->next;
        delete temp;
        currentSize--;
        return true;
    }

    Node<T>* prev = front;
    Node<T>* current = front->next;
    while (current != nullptr) {
        if (current->data == val) {
            prev->next = current->next;
            delete current;
            if (rear == current) {
                rear = prev;
            }
            currentSize--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}
