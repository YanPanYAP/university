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
    cout << "Введите символ: ";
    cin >> symbol;
    if (isLetter(symbol)) {
        if (isupper(symbol)) {
            if (!lowercaseQueue.isFull())
                lowercaseQueue.enqueue(tolower(symbol));
            else
                cout << "Очередь строчных букв полна." << endl;
        }
        else {
            if (!uppercaseQueue.isFull())
                uppercaseQueue.enqueue(toupper(symbol));
            else
                cout << "Очередь прописных букв полна." << endl;
        }
    }
    else {
        cout << "Недопустимый символ: " << symbol << endl;
    }
}

void printQueues(Queue<char>& lowercaseQueue, Queue<char>& uppercaseQueue) {
    cout << "Очередь строчных букв: ";
    lowercaseQueue.print();
    cout << "Очередь прописных букв: ";
    uppercaseQueue.print();
}

void removeElement(Queue<char>& lowercaseQueue, Queue<char>& uppercaseQueue) {
    char symbol;
    cout << "Введите символ для удаления: ";
    cin >> symbol;
    bool removed = lowercaseQueue.remove(symbol) || uppercaseQueue.remove(symbol);
    if (removed) {
        cout << "Символ " << symbol << " удален из очереди." << endl;
    }
    else {
        cout << "Символ " << symbol << " не найден в очередях." << endl;
    }
}

int main() {
    setlocale(LC_CTYPE, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int size;
    cout << "Введите максимальный размер очередей: ";
    cin >> size;
    Queue<char> lowercaseQueue(size);
    Queue<char> uppercaseQueue(size);

    int choice;
    do {
        cout << "\nМеню:\n";
        cout << "1. Добавить символ в очередь\n";
        cout << "2. Вывести очереди строчных и прописных букв\n";
        cout << "3. Удалить символ из обеих очередей\n";
        cout << "0. Выход\n";
        cout << "Выберите действие: ";
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
            cout << "Выход из программы." << endl;
            break;
        default:
            cout << "Неверный выбор. Попробуйте еще раз." << endl;
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
        cerr << "Очередь полна." << endl;
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
        cerr << "Очередь пуста." << endl;
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
        cout << "Очередь пуста." << endl;
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
