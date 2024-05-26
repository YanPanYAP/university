#include<iostream>
using namespace std;

struct Item
{
	int data;
	Item* next;
};
Item* head, * tail;

bool isNull(void) 
{
	return (head == NULL);
}

void deletFirst() 
{
	if (isNull())
		cout << "������� �����" << endl;
	else
	{
		Item* p = head;
		head = head->next;
		delete p;
	}
}
void getFromHead()
{
	if (isNull())
		cout << "������� �����" << endl;
	else
		cout << "������ = " << head->data << endl;
}

void insertToQueue(int x)
{
	Item* p = new Item;
	p->data = x;
	p->next = NULL;
	Item* v = new Item;
	Item* p1 = new Item;
	Item* p2 = new Item;
	int i = 0;      
	if (isNull())
		head = tail = p;
	else
	{
		p2 = head; p1 = head;
		while (p1 != NULL) 
		{
			if (i == 1)
			{
				if (x <= p1->data) 
				{
					v->data = x;
					v->next = p1;
					p2->next = v;
					return;
				}
				p2 = p2->next;
			}
			else
			{
				if (x <= p1->data)
				{
					v->data = x;
					v->next = p1;
					head = v;
					return;
				}
			}
			p1 = p1->next;
			i = 1;
		}
		if (p1 == NULL)
		{
			tail->next = p;
			tail = p;
		}
	}
}

void printQueue()    
{
	Item* p = new Item;
	if (isNull())
		cout << "������� �����" << endl;
	else
	{
		cout << "������� = ";
		p = head;
		while (!isNull())
		{
			if (p != NULL)
			{
				cout << p->data << " "; cout << "->";
				p = p->next;
			}
			else
			{
				cout << "NULL" << endl;
				return;
			}
		}
	}
}

void clrQueue()     
{
	while (!isNull()) deletFirst();
}

void insertMinPriority(int x) {
	Item* p = new Item;
	p->data = x;
	p->next = nullptr;
	if (isNull()) {
		head = tail = p;
	}
	else {
		tail->next = p;
		tail = p;
	}
}

void removeFirst() {
	if (isNull()) {
		cout << "������� �����" << endl;
	}
	else {
		Item* p = head;
		head = head->next;
		cout << "������ �������: " << p->data << endl;
		delete p;
	}
}

void insertMaxPriority(int x) {
	Item* p = new Item;
	p->data = x;
	p->next = nullptr;
	if (isNull()) {
		head = tail = p;
	}
	else {
		Item* current = head;
		Item* prev = nullptr;
		while (current && x >= current->data) {
			prev = current;
			current = current->next;
		}
		if (!prev) {
			p->next = head;
			head = p;
		}
		else {
			prev->next = p;
			p->next = current;
		}
	}
}

void removeLast() {
	if (isNull()) {
		cout << "������� �����" << endl;
	}
	else {
		if (head == tail) {
			cout << "������ �������: " << head->data << endl;
			delete head;
			head = tail = nullptr;
		}
		else {
			Item* current = head;
			while (current->next != tail) {
				current = current->next;
			}
			cout << "������ �������: " << tail->data << endl;
			delete tail;
			tail = current;
			tail->next = nullptr;
		}
	}
}

int main()
{
	setlocale(LC_CTYPE, "Russian");
	int i = 1, choice = 1, z; head = NULL;  tail = NULL;
	while (choice != 0)
	{
		cout << "\n1 - �������� �������" << endl;
		cout << "2 - �������� ������� � ������" << endl;
		cout << "3 - ������� ������� � ������" << endl;
		cout << "4 - ������� ��������" << endl;
		cout << "5 - �������� �������" << endl;
		cout << "6 - ������� 1" << endl;
		cout << "7 - ������� 2" << endl;
		cout << "8 - ������� 3" << endl;
		cout << "9 - ������� 5" << endl;
		cout << "0 - �����" << endl;
		cout << "�������� ��������  ";  cin >> choice;
		system("cls");

		switch (choice)
		{
		case 1: cout << "������� �������:  "; cin >> z;
			insertToQueue(z); printQueue();  break;
		case 2: getFromHead(); break;
		case 3: deletFirst();  break;
		case 4: printQueue();  break;
		case 5: clrQueue();    break;
		case 6:
			cout << "������� ��������: ";
			cin >> z;
			insertMinPriority(z);
			break;
		case 7:
			removeFirst();
			break;
		case 8:
			cout << "������� ��������: ";
			cin >> z;
			insertMaxPriority(z);
			break;
		case 9:
			removeLast();
			break;
		}
	}
	return 0;
}