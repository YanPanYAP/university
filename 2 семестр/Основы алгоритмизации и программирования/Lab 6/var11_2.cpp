#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;

template<typename T>
class List
{
public:
    List();
    ~List();

    void clear();
    void push_front(T data);
    void removeAt(int index);
    void pop_front();
    void findAndPrintPrevious(T value);
    void writeToFile(const string& filename);
    void readFromFile(const string& filename);
    void print();
    void find(T value);
    void push_back(T data);

private:
    template<typename T>
    class Node
    {
    public:
        Node* pNext;
        T data;

        Node(T data = T(), Node* pNext = nullptr)
        {
            this->data = data;
            this->pNext = pNext;
        }
    };
    int Size;
    Node<T>* head;
};


template<typename T>
List<T>::List()
{
    Size = 0;
    head = nullptr;
}


template<typename T>
List<T>::~List()
{
    clear();
}

template<typename T>
void List<T>::push_front(T data)
{
    head = new Node<T>(data, head);
    Size++;
}

template<typename T>
void List<T>::removeAt(int index)
{
    if (index == 0)
    {
        pop_front();
    }
    else
    {
        Node<T>* previous = this->head;
        for (int i = 0; i < index - 1; i++)
        {
            previous = previous->pNext;
        }

        Node<T>* toDelete = previous->pNext;

        previous->pNext = toDelete->pNext;

        delete toDelete;

        Size--;
    }
}

template<typename T>
void List<T>::pop_front()
{
    Node<T>* temp = head;

    head = head->pNext;

    delete temp;

    Size--;
}

template<typename T>
void List<T>::clear()
{
    while (Size)
    {
        pop_front();
    }
}

template<typename T>
void List<T>::writeToFile(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        Node<T>* current = head;
        while (current != nullptr) {
            file << current->data << " ";
            current = current->pNext;
        }
        file.close();
        cout << "Список успешно записан в файл." << endl;
    }
    else {
        cout << "Невозможно открыть файл для записи." << endl;
    }
}

template<typename T>
void List<T>::push_back(T data)
{
    if (head == nullptr)
    {
        head = new Node<T>(data);
    }
    else
    {
        Node<T>* current = head;
        while (current->pNext != nullptr)
        {
            current = current->pNext;
        }
        current->pNext = new Node<T>(data);
    }
    Size++;
}


template<typename T>
void List<T>::readFromFile(const string& filename) {
    clear();
    ifstream file(filename);
    if (file.is_open()) {
        T value;
        while (file >> value) {
            push_back(value);
        }
        file.close();
        cout << "Список успешно прочитан из файла." << endl;
    }
    else {
        cout << "Невозможно открыть файл для чтения." << endl;
    }
}

template<typename T>
void List<T>::print() {
    if (Size == 0) {
        cout << "Список пуст." << endl;
        return;
    }
    Node<T>* current = head;
    cout << "Список: ";
    while (current != nullptr) {
        cout << current->data << " ";
        current = current->pNext;
    }
    cout << endl;
}

template<typename T>
void List<T>::find(T value) {
    Node<T>* current = head;
    int index = 0;
    bool found = false;
    while (current != nullptr) {
        if (current->data == value) {
            cout << "Символ '" << value << "' найден на позиции " << index << "." << endl;
            found = true;
        }
        current = current->pNext;
        index++;
    }
    if (!found) {
        cout << "Символ '" << value << "' не найден в списке." << endl;
    }
}

template<typename T>
void List<T>::findAndPrintPrevious(T value) {
    if (Size < 2) {
        cout << "Недостаточно элементов в списке для поиска." << endl;
        return;
    }
    Node<T>* current = head->pNext;
    Node<T>* prev = head;
    while (current != nullptr) {
        if (current->data == value) {
            cout << "Найден символ '" << value << "' и предыдущий символ '" << prev->data << "'." << endl;
            return;
        }
        prev = current;
        current = current->pNext;
    }
    cout << "Символ '" << value << "' не найден в списке." << endl;
}

void menu()
{
    cout << "1. Ввод символа\n";
    cout << "2. Запись списка в файл\n";
    cout << "3. Чтение списка из файла\n";
    cout << "4. Найти и удалить символ\n";
    cout << "5. Вывести список\n";
    cout << "6. Найти символ\n";
    cout << "7. Найти и вывести предыдущий символ\n";
    cout << "0. Выход\n\n";
}

int main()
{
	setlocale(LC_CTYPE, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

    int choice; char value; string filename;
    List<char> lst;
    menu();
    cout << "Ваш выбор: ";
    cin >> choice;

    while (choice != 0)
    {
        switch (choice)
        {
        case 1:
            cout << "Введите символ: ";
            cin >> value;
            lst.push_front(value);
            break;
        case 2:
            cout << "Введите имя файла для записи: ";
            cin >> filename;
            lst.writeToFile(filename);
            break;
        case 3:
            cout << "Введите имя файла для чтения: ";
            cin >> filename;
            lst.readFromFile(filename);
            break;
        case 4:
            cout << "Введите удаляемый символ: ";
            cin >> value;
            lst.removeAt(value);
            break;
        case 5:
            lst.print();
            break;
        case 6:
            cout << "Введите символ для поиска: ";
            cin >> value;
            lst.find(value);
            break;
        case 7:
            cout << "Введите символ для поиска: ";
            cin >> value;
            lst.findAndPrintPrevious(value);
            break;
        default:
            cout << "Неправильный выбор." << endl;
            menu(); break;
        }

        cout << "\nВаш выбор: ";
        cin >> choice;
    }

    return 0;
}