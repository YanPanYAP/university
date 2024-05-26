#include <iostream>
using namespace std;

struct Tree {
	int key;          // ����
	char text[5];    // ����� - �� ����� 4 ����
	Tree* Left;
	Tree* Right;
};

// ������� ��� ������ � �������
Tree* makeTree(Tree* Root);
Tree* list(int i, char* s);
Tree* insertElem(Tree* Root, int key, char* s);
Tree* search(Tree* n, int key);
Tree* delet(Tree* Root, int key);
int view(Tree* t, int level);
int count(Tree* t, char letter);
bool delAll(Tree* t);
int sum(Tree* t);
void branchesAtLevel(Tree* t, int level, int currentLevel);

int c = 0; // ���������� ����
Tree* Root = NULL; // ��������� �� ������

void main() {
	setlocale(0, "Russian");
	int key, choice, level;
	char s[5], letter;

	while (true) {
		cout << "1 - �������� ������\n";
		cout << "2 - ���������� ��������\n";
		cout << "3 - ����� �� �����\n";
		cout << "4 - �������� ��������\n";
		cout << "5 - ����� ������\n";
		cout << "6 - ������� ���������� ����\n";
		cout << "7 - ������� ������\n";
		cout << "8 - ������� ����� �������� ���� ������ ������\n";
		cout << "9 - ���������� ������ � ����� ��������� �� n-� ������\n"; // ����� �����
		cout << "0 - �����\n";
		cout << "��� �����?\n";
		cin >> choice;

		switch (choice) {
		case 1: {
			Root = makeTree(Root);
			break;
		}
		case 2: {
			cout << "������� ����: ";
			cin >> key;
			cout << "������� �����: ";
			cin >> s;
			insertElem(Root, key, s);
			break;
		}
		case 3: {
			cout << "������� ����: ";
			cin >> key;
			Tree* rc = search(Root, key);
			if (rc) {
				cout << "��������� �����: ";
				puts(rc->text);
			}
			else {
				cout << "��� ������ ��������\n";
			}
			break;
		}
		case 4: {
			cout << "������� ��������� ����: ";
			cin >> key;
			Root = delet(Root, key);
			break;
		}
		case 5: {
			if (Root != NULL) {
				cout << "������ ��������� �� 90 ����. �����\n";
				view(Root, 0);
			}
			else {
				cout << "������ ������\n";
			}
			break;
		}
		case 6: {
			cout << "������� �����: ";
			cin >> letter;
			int n = count(Root, letter);
			cout << "���������� ����, ������������ � ����� " << letter << " ����� " << n << endl;
			break;
		}
		case 7: {
			delAll(Root);
			Root = NULL;
			cout << "������ �������\n";
			break;
		}
		case 8: {
			int totalSum = sum(Root);
			if (totalSum == 0) {
				cout << "������ ������\n";
			}
			else {
				cout << "����� �������� ���� ������ ������: " << totalSum << endl;
			}
			break;
		}
		case 9: {
			cout << "������� ������� n: ";
			cin >> level;
			branchesAtLevel(Root, level, 0);
			break;
		}
		case 0: {
			return;
		}
		}
	}
}

// ������� ��� ����������� ���������� ������ �� n-� ������ � ������ �� ���������
void branchesAtLevel(Tree* t, int targetLevel, int currentLevel) {
	if (t == NULL) {
		return;
	}

	// ���� �������� �������� ������, ������� �������
	if (currentLevel == targetLevel) {
		cout << "����: " << t->key << ", �����: " << t->text << endl;
	}
	else {
		// ���������� ���� ����� � ������, ���������� ������� �������
		branchesAtLevel(t->Left, targetLevel, currentLevel + 1);
		branchesAtLevel(t->Right, targetLevel, currentLevel + 1);
	}
}

// ��������� ������� �������� ��������, �� ���������� ��� ���� � �������� ����.



Tree* makeTree(Tree* Root)    //�������� ������
{
	int key; char s[5];
	cout << "����� ����� - ������������� �����\n\n";
	if (Root == NULL)	// ���� ������ �� �������
	{
		cout << "������� ���� �����: ";
		cin >> key;
		cout << "������� ����� �����: ";
		cin >> s;
		Root = list(key, s);	// ��������� ��������� �� ������
	}
	while (1)                //���������� ���������
	{
		cout << "\n������� ����: ";
		cin >> key;
		if (key < 0) break;       //������� ������ (���� < 0)   
		cout << "������� �����: ";
		cin >> s;
		insertElem(Root, key, s);
	}
	return Root;
}

Tree* list(int i, char* s)     //�������� ������ ��������
{
	Tree* t = new Tree[sizeof(Tree)];
	t->key = i;
	for (i = 0; i < 5; i++)
		*((t->text) + i) = *(s + i);
	t->Left = t->Right = NULL;
	return t;
}

Tree* insertElem(Tree* t, int key, char* s)  //���������� ������ ��������
{
	Tree* Prev = NULL;	      // Prev - ������� ����� �������
	int find = 0;        // ������� ������
	while (t && !find)
	{
		Prev = t;
		if (key == t->key)
			find = 1;	   //����� ������ ���� ���������
		else
			if (key < t->key)
				t = t->Left;
			else
				t = t->Right;
	}
	if (!find)              //������� ����� � ������� Prev
	{
		t = list(key, s);           //��������� ����� ���� 
		if (key < Prev->key)     // � �������������� ���� 
			Prev->Left = t;     //������� �� ����� �����,
		else
			Prev->Right = t;    // ���� �� ������ 
	}
	return t;
}

Tree* delet(Tree* Root, int key)  //�������� �������� �� �����
{	// Del, Prev_Del - ��������� ������� � ��� ����������;
	// R, Prev_R - �������, �� ������� ���������� ���������, � ��� ��������; 
	Tree* Del, * Prev_Del, * R, * Prev_R;
	Del = Root;
	Prev_Del = NULL;
	while (Del != NULL && Del->key != key)//����� �������� � ��� �������� 
	{
		Prev_Del = Del;
		if (Del->key > key)
			Del = Del->Left;
		else
			Del = Del->Right;
	}
	if (Del == NULL)              // ������� �� ������
	{
		puts("\n��� ������ �����");
		return Root;
	}
	if (Del->Right == NULL)     // ����� �������� R ��� ������
		R = Del->Left;
	else
		if (Del->Left == NULL)
			R = Del->Right;
		else
		{
			Prev_R = Del;      //����� ������ ������� �������� � ����� ���������
			R = Del->Left;
			while (R->Right != NULL)
			{
				Prev_R = R;
				R = R->Right;
			}
			if (Prev_R == Del) // ������ ������� ��� ������ R � ��� �������� Prev_R 
				R->Right = Del->Right;
			else
			{
				R->Right = Del->Right;
				Prev_R->Right = R->Left;
				R->Left = Prev_R;
			}
		}
	if (Del == Root) Root = R;	//�������� ����� � ������ ��� �� R
	else
		// ��������� R �������������� � �������� ���������� ����
		if (Del->key < Prev_Del->key)
			Prev_Del->Left = R;  //�� ����� ����� 
		else
			Prev_Del->Right = R;	//�� ������ �����
	int tmp = Del->key;
	cout << "\n������ ������� � ������ " << tmp << endl;
	delete Del;
	return Root;
}

Tree* search(Tree* n, int key)  //����� �������� �� ����� 
{
	Tree* rc = n;
	if (rc != NULL)
	{
		if (key < (key, n->key))
			rc = search(n->Left, key);
		else
			if (key > (key, n->key))
				rc = search(n->Right, key);
	}
	else
		cout << "��� ������ ��������\n";
	return rc;
}

int count(Tree* t, char letter) //������� ���������� ����
{
	if (t)
	{
		count(t->Right, letter);
		if (*(t->text) == letter)
			c++;
		count(t->Left, letter);
	}
	return c;
}

int view(Tree* t, int level) //����� ������ 
{
	if (t)
	{
		view(t->Right, level + 1);	//����� ������� ���������
		for (int i = 0; i < level; i++)
			cout << "   ";
		int tm = t->key;
		cout << tm << ' ';
		puts(t->text);
		view(t->Left, level + 1);	//����� ������ ���������
		return 0;
	}
	return 1;
}

bool delAll(Tree* t) //������� ������
{
	if (t != NULL)
	{
		delAll(t->Left);
		delAll(t->Right);
		delete t;
		return true;
	}
	return false;
}

int sum(Tree* p1) {
	if (p1) {

		int s = p1->key;
		if (p1->Left) {
			s += sum(p1->Left);
		}
		if (p1->Right) {
			s += sum(p1->Right);
		}

		return s;
	}
	else {
		return 0;
	}
}