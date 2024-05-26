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
    void writeToFile(const string& filename);
    void readFromFile(const string& filename);
    void print();
    void find(T value);
    void push_back(T data);
    T averagePositive();

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
        cout << "������ ������� ������� � ����." << endl;
    }
    else {
        cout << "���������� ������� ���� ��� ������." << endl;
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
void List<T>::readFromFile(const string& filename)
{
    clear();
    ifstream file(filename);
    if (file.is_open())
    {
        T value;
        while (file >> value)
        {
            push_back(value);
        }
        file.close();
        cout << "������ ������� �������� �� �����." << endl;
    }
    else
    {
        cout << "���������� ������� ���� ��� ������." << endl;
    }
}

template<typename T>
void List<T>::print()
{
    if (Size == 0)
    {
        cout << "������ ����." << endl;
        return;
    }
    Node<T>* current = head;
    cout << "������: ";
    while (current != nullptr)
    {
        cout << current->data << " ";
        current = current->pNext;
    }
    cout << endl;
}

template<typename T>
void List<T>::find(T value)
{
    Node<T>* current = head;
    int index = 0;
    bool found = false;
    while (current != nullptr)
    {
        if (current->data == value) 
        {
            cout << "������� '" << value << "' ������ �� ������� " << index << "." << endl;
            found = true;
        }
        current = current->pNext;
        index++;
    }
    if (!found)
    {
        cout << "������� '" << value << "' �� ������ � ������." << endl;
    }
}

template<typename T>
T List<T>::averagePositive() {
    T sum = 0;
    int count = 0;
    Node<T>* current = head;
    while (current != nullptr) {
        if (current->data > 0) {
            sum += current->data;
            count++;
        }
        current = current->pNext;
    }
    if (count > 0) {
        return sum / count;
    }
    else {
        cout << "��� ������������� ��������� � ������." << endl;
        return 0;
    }
}

void menu()
{
    cout << "1. ���� ��������\n";
    cout << "2. ������ ������ � ����\n";
    cout << "3. ������ ������ �� �����\n";
    cout << "4. ������� �������\n";
    cout << "5. ������� ������\n";
    cout << "6. ����� �������\n";
    cout << "7. ����� ������� �������� ������������� ���������\n";
    cout << "0. �����\n\n";
}

int main()
{
    setlocale(LC_CTYPE, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int choice; float value; string filename;
    List<float> lst;
    menu();
    cout << "��� �����: ";
    cin >> choice;

    while (choice != 0)
    {
        switch (choice)
        {
        case 1:
            cout << "������� �������: ";
            cin >> value;
            lst.push_front(value);
            break;
        case 2:
            cout << "������� ��� ����� ��� ������: ";
            cin >> filename;
            lst.writeToFile(filename);
            break;
        case 3:
            cout << "������� ��� ����� ��� ������: ";
            cin >> filename;
            lst.readFromFile(filename);
            break;
        case 4:
            cout << "������� ��������� �������: ";
            cin >> value;
            lst.removeAt(value);
            break;
        case 5:
            lst.print();
            break;
        case 6:
            cout << "������� ������� ��� ������: ";
            cin >> value;
            lst.find(value);
            break;
        case 7:
            cout << "������� �������� ������������� ���������: " << lst.averagePositive() << endl;
            break;
        default:
            cout << "������������ �����." << endl;
            menu(); break;
        }

        cout << "\n��� �����: ";
        cin >> choice;
    }

    return 0;
}
