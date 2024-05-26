#include <iostream>
#include <fstream>
using namespace std;

struct ListNode
{
    char character;
    ListNode* next;
};

void insert(ListNode*&, char);
void printList(ListNode*);
void toFile(ListNode*&);
void fromFile(ListNode*&);
int del(ListNode*&, char);
int findChar(ListNode*&, char);
int IsEmpty(ListNode*);
void findAndPrintPrevious(ListNode*, char);
void menu();

int main()
{
    setlocale(LC_CTYPE, "Russian");
    ListNode* first = nullptr;
    int choice; char value;
    menu();
    cout << "��� �����: ";
    cin >> choice;
    while (choice != 0)
    {
        switch (choice)
        {
        case 1:
            cout << "������� ������: ";
            cin >> value;
            insert(first, value);
            printList(first);
            break;
        case 2:
            toFile(first);
            break;
        case 3:
            fromFile(first);
            break;
        case 4:
            if (!IsEmpty(first))
            {
                cout << "������� ��������� ������: ";
                cin >> value;
                if (del(first, value))
                {
                    cout << "������ ������ " << value << endl;
                    printList(first);
                }
                else
                    cout << "������ �� ������." << endl;
            }
            else
            cout << "������ ����." << endl;
            break;
        case 5:
            printList(first);
            break;
        case 6:
            if (!IsEmpty(first))
            {
                cout << "������� ������� ������: ";
                cin >> value;
                if (findChar(first, value))
                {
                    cout << "������� ������: " << value << endl;
                }
                else
                    cout << "������ �� ������." << endl;
            }
            else
            cout << "������ ����." << endl;
            break;
        case 7:
            if (!IsEmpty(first))
            {
                cout << "������� ������: ";
                cin >> value;
                findAndPrintPrevious(first, value);
            }
            else
                cout << "������ ����." << endl;
            break;
        default:
            cout << "������������ �����." << endl;
            menu(); break;
        }
        cout << "\n��� �����:  ";
        cin >> choice;
    }
    return 0;
}

void insert(ListNode*& p, char value)
{
    ListNode* newP = new ListNode;
    if (newP != NULL)
    {
        newP->character = value;
        newP->next = p;
        p = newP;
    }
    else
        cout << "�������� ���������� �� ���������." << endl;
}

void printList(ListNode* p)
{
    if (p == NULL)
        cout << "������ ����." << endl;
    else
    {
        cout << "������:" << endl;
        while (p != NULL)
        {
            cout << "-->" << p->character;
            p = p->next;
        }
        cout << "-->NULL" << endl;
    }
}

void toFile(ListNode*& p)
{
    ListNode* temp = p;
    ListNode buf;
    ofstream frm("mList.dat");
    if (frm.fail())
    {
        cout << "\n ������ �������� �����.";
        exit(1);
    }
    while (temp)
    {
        buf = *temp;
        frm.write((char*)&buf, sizeof(ListNode));
        temp = temp->next;
    }
    frm.close();
    cout << "������ ������� � ���� mList.dat.\n";
}

void fromFile(ListNode*& p)
{
    ListNode buf, * first = nullptr;
    ifstream frm("mList.dat");
    if (frm.fail())
    {
        cout << "\n ������ �������� �����.";
        exit(1);
    }
    frm.read((char*)&buf, sizeof(ListNode));
    while (!frm.eof())
    {
        insert(first, buf.character);
        cout << "-->" << buf.character;
        frm.read((char*)&buf, sizeof(ListNode));
    }
    cout << "-->NULL" << endl;
    frm.close();
    p = first;
    cout << "������ ������ �� ����� mList.dat.\n";
}

int del(ListNode*& p, char value)
{
    ListNode* previous, * current, * temp;
    if (value == p->character)
    {
        temp = p;
        p = p->next;
        delete temp;
        return value;
    }
    else
    {
        previous = p;
        current = p->next;
        while (current != NULL && current->character != value)
        {
            previous = current;
            current = current->next;
        }
        if (current != NULL)
        {
            temp = current;
            previous->next = current->next;
            delete(temp);
            return value;
        }
    }
    return 0;
}

int findChar(ListNode*& p, char value)
{
    ListNode* previous, * current;
    if (value == p->character)
    {
        return value;
    }
    else
    {
        previous = p;
        current = p->next;
        while (current != NULL && current->character != value)
        {
            previous = current;
            current = current->next;
        }
        if (current != NULL)
        {
            return value;
        }
    }
    return 0;
}

int IsEmpty(ListNode* p)
{
    return p == NULL;
}

void findAndPrintPrevious(ListNode* p, char value)
{
    ListNode* previous = nullptr;
    ListNode* current = p;
    while (current != NULL && current->character != value)
    {
        previous = current;
        current = current->next;
    }
    if (current != NULL && previous != NULL)
    {
        cout << "������� ������: " << current->character << endl;
        cout << "���������� ������: " << previous->character << endl;
    }
    else
    {
        cout << "������ �� ������ ��� �� ��������� � ������." << endl;
    }
}

void menu()
{
    cout << "1 - ���� �������\n";
    cout << "2 - ������ ������ � ����\n";
    cout << "3 - ������ ������ �� �����\n";
    cout << "4 - ����� � ������� ������\n";
    cout << "5 - ������� ������\n";
    cout << "6 - ����� ������\n";
    cout << "7 - ����� � ������� ���������� ������\n";
    cout << "0 - �����\n\n";
}