#include <iostream>
using namespace std;

struct Tree {
	int key;          // ключ
	char text[5];    // текст - не более 4 букв
	Tree* Left;
	Tree* Right;
};

// Функции для работы с деревом
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

int c = 0; // количество слов
Tree* Root = NULL; // указатель на корень

void main() {
	setlocale(0, "Russian");
	int key, choice, level;
	char s[5], letter;

	while (true) {
		cout << "1 - создание дерева\n";
		cout << "2 - добавление элемента\n";
		cout << "3 - поиск по ключу\n";
		cout << "4 - удаление элемента\n";
		cout << "5 - вывод дерева\n";
		cout << "6 - подсчет количества слов\n";
		cout << "7 - очистка дерева\n";
		cout << "8 - подсчет суммы значений всех вершин дерева\n";
		cout << "9 - количество ветвей и вывод элементов на n-м уровне\n"; // Новый выбор
		cout << "0 - выход\n";
		cout << "Ваш выбор?\n";
		cin >> choice;

		switch (choice) {
		case 1: {
			Root = makeTree(Root);
			break;
		}
		case 2: {
			cout << "Введите ключ: ";
			cin >> key;
			cout << "Введите слово: ";
			cin >> s;
			insertElem(Root, key, s);
			break;
		}
		case 3: {
			cout << "Введите ключ: ";
			cin >> key;
			Tree* rc = search(Root, key);
			if (rc) {
				cout << "Найденное слово: ";
				puts(rc->text);
			}
			else {
				cout << "Нет такого элемента\n";
			}
			break;
		}
		case 4: {
			cout << "Введите удаляемый ключ: ";
			cin >> key;
			Root = delet(Root, key);
			break;
		}
		case 5: {
			if (Root != NULL) {
				cout << "Дерево повернуто на 90 град. влево\n";
				view(Root, 0);
			}
			else {
				cout << "Дерево пустое\n";
			}
			break;
		}
		case 6: {
			cout << "Введите букву: ";
			cin >> letter;
			int n = count(Root, letter);
			cout << "Количество слов, начинающихся с буквы " << letter << " равно " << n << endl;
			break;
		}
		case 7: {
			delAll(Root);
			Root = NULL;
			cout << "Дерево очищено\n";
			break;
		}
		case 8: {
			int totalSum = sum(Root);
			if (totalSum == 0) {
				cout << "Дерево пустое\n";
			}
			else {
				cout << "Сумма значений всех вершин дерева: " << totalSum << endl;
			}
			break;
		}
		case 9: {
			cout << "Введите уровень n: ";
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

// Функция для определения количества ветвей на n-м уровне и вывода их элементов
void branchesAtLevel(Tree* t, int targetLevel, int currentLevel) {
	if (t == NULL) {
		return;
	}

	// Если достигли целевого уровня, выводим элемент
	if (currentLevel == targetLevel) {
		cout << "Ключ: " << t->key << ", Текст: " << t->text << endl;
	}
	else {
		// Рекурсивно идем влево и вправо, увеличивая текущий уровень
		branchesAtLevel(t->Left, targetLevel, currentLevel + 1);
		branchesAtLevel(t->Right, targetLevel, currentLevel + 1);
	}
}

// Остальные функции остаются прежними, их реализация уже дана в исходном коде.



Tree* makeTree(Tree* Root)    //Создание дерева
{
	int key; char s[5];
	cout << "Конец ввода - отрицательное число\n\n";
	if (Root == NULL)	// если дерево не создано
	{
		cout << "Введите ключ корня: ";
		cin >> key;
		cout << "Введите слово корня: ";
		cin >> s;
		Root = list(key, s);	// установка указателя на корень
	}
	while (1)                //добавление элементов
	{
		cout << "\nВведите ключ: ";
		cin >> key;
		if (key < 0) break;       //признак выхода (ключ < 0)   
		cout << "Введите слово: ";
		cin >> s;
		insertElem(Root, key, s);
	}
	return Root;
}

Tree* list(int i, char* s)     //Создание нового элемента
{
	Tree* t = new Tree[sizeof(Tree)];
	t->key = i;
	for (i = 0; i < 5; i++)
		*((t->text) + i) = *(s + i);
	t->Left = t->Right = NULL;
	return t;
}

Tree* insertElem(Tree* t, int key, char* s)  //Добавление нового элемента
{
	Tree* Prev = NULL;	      // Prev - элемент перед текущим
	int find = 0;        // признак поиска
	while (t && !find)
	{
		Prev = t;
		if (key == t->key)
			find = 1;	   //ключи должны быть уникальны
		else
			if (key < t->key)
				t = t->Left;
			else
				t = t->Right;
	}
	if (!find)              //найдено место с адресом Prev
	{
		t = list(key, s);           //создается новый узел 
		if (key < Prev->key)     // и присоединяется либо 
			Prev->Left = t;     //переход на левую ветвь,
		else
			Prev->Right = t;    // либо на правую 
	}
	return t;
}

Tree* delet(Tree* Root, int key)  //Удаление элемента по ключу
{	// Del, Prev_Del - удаляемый элемент и его предыдущий;
	// R, Prev_R - элемент, на который заменяется удаленный, и его родитель; 
	Tree* Del, * Prev_Del, * R, * Prev_R;
	Del = Root;
	Prev_Del = NULL;
	while (Del != NULL && Del->key != key)//поиск элемента и его родителя 
	{
		Prev_Del = Del;
		if (Del->key > key)
			Del = Del->Left;
		else
			Del = Del->Right;
	}
	if (Del == NULL)              // элемент не найден
	{
		puts("\nНет такого ключа");
		return Root;
	}
	if (Del->Right == NULL)     // поиск элемента R для замены
		R = Del->Left;
	else
		if (Del->Left == NULL)
			R = Del->Right;
		else
		{
			Prev_R = Del;      //поиск самого правого элемента в левом поддереве
			R = Del->Left;
			while (R->Right != NULL)
			{
				Prev_R = R;
				R = R->Right;
			}
			if (Prev_R == Del) // найден элемент для замены R и его родителя Prev_R 
				R->Right = Del->Right;
			else
			{
				R->Right = Del->Right;
				Prev_R->Right = R->Left;
				R->Left = Prev_R;
			}
		}
	if (Del == Root) Root = R;	//удаление корня и замена его на R
	else
		// поддерево R присоединяется к родителю удаляемого узла
		if (Del->key < Prev_Del->key)
			Prev_Del->Left = R;  //на левую ветвь 
		else
			Prev_Del->Right = R;	//на правую ветвь
	int tmp = Del->key;
	cout << "\nУдален элемент с ключом " << tmp << endl;
	delete Del;
	return Root;
}

Tree* search(Tree* n, int key)  //Поиск элемента по ключу 
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
		cout << "Нет такого элемента\n";
	return rc;
}

int count(Tree* t, char letter) //Подсчет количества слов
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

int view(Tree* t, int level) //Вывод дерева 
{
	if (t)
	{
		view(t->Right, level + 1);	//вывод правого поддерева
		for (int i = 0; i < level; i++)
			cout << "   ";
		int tm = t->key;
		cout << tm << ' ';
		puts(t->text);
		view(t->Left, level + 1);	//вывод левого поддерева
		return 0;
	}
	return 1;
}

bool delAll(Tree* t) //Очистка дерева
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