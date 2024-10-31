#include <iostream>
#include <queue>
#include <stack>
#include <ctime>
#include <list>
using namespace std;

const int MAX_VERTICES = 10;
const int MAX_EDGES = 11;

struct Edge {
    int src, dest;
};

int adjMatrix[MAX_VERTICES][MAX_VERTICES];
list<int> adjList[MAX_VERTICES];
Edge edgeList[MAX_EDGES];

void initializeGraph() {
    edgeList[0] = { 0, 1 };
    edgeList[1] = { 0, 4 };
    edgeList[2] = { 1, 6 };
    edgeList[3] = { 1, 7 };
    edgeList[4] = { 2, 7 };
    edgeList[5] = { 3, 5 };
    edgeList[6] = { 3, 8 };
    edgeList[7] = { 4, 5 };
    edgeList[8] = { 6, 7 };
    edgeList[9] = { 5, 8 };
    edgeList[10] = { 8, 9 };

    // Инициализация матрицы смежности
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            adjMatrix[i][j] = 0;
        }
    }

    // Заполняем матрицу и список смежности
    for (int i = 0; i < MAX_EDGES; i++) {
        int u = edgeList[i].src;
        int v = edgeList[i].dest;

        adjMatrix[u][v] = 1;
        adjMatrix[v][u] = 1;

        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }
}

// Вывод матрицы смежности
void printAdjMatrix() {
    cout << "Матрица смежности графа:\n";
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            cout << adjMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

// Вывод списка смежности
void printAdjList() {
    cout << "\nСписок смежности графа:\n";
    for (int i = 0; i < MAX_VERTICES; i++) {
        cout << i + 1 << ": ";
        for (int neighbor : adjList[i]) {
            cout << neighbor + 1 << " ";
        }
        cout << endl;
    }
}

// Вывод списка рёбер
void printEdgeList() {
    cout << "\nСписок рёбер графа:\n";
    for (int i = 0; i < MAX_EDGES; i++) {
        cout << edgeList[i].src + 1 << " - " << edgeList[i].dest + 1 << endl;
    }
}

// Обход в глубину (DFS)
void DFS(int start) {
    bool visited[MAX_VERTICES] = { false };
    stack<int> s;
    s.push(start);

    cout << "\nПорядок прохождения вершин (DFS): ";

    while (!s.empty()) {
        int v = s.top();
        s.pop();

        if (!visited[v]) {
            cout << v + 1 << " ";
            visited[v] = true;
        }

        for (int i = MAX_VERTICES - 1; i >= 0; i--) {
            if (adjMatrix[v][i] == 1 && !visited[i]) {
                s.push(i);
            }
        }
    }
    cout << endl;
}

// Обход в ширину (BFS)
void BFS(int start) {
    bool visited[MAX_VERTICES] = { false };
    queue<int> q;
    q.push(start);
    visited[start] = true;

    cout << "\nПорядок прохождения вершин (BFS): ";

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        cout << v + 1 << " ";

        for (int i = 0; i < MAX_VERTICES; i++) {
            if (adjMatrix[v][i] == 1 && !visited[i]) {
                q.push(i);
                visited[i] = true;
            }
        }
    }
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "rus");
    initializeGraph();   

    while (true) {
        printAdjMatrix();
        printAdjList();
        printEdgeList();

        int startVertex;
        cout << "\nВведите вершину для начала обхода (1-10): ";
        cin >> startVertex;

        // Проверка корректного диапазона ввода
        if (startVertex < 1 || startVertex > 10) {
            cout << "Ошибка: Введите число от 1 до 10.\n";
            continue;
        }

        startVertex--;

        clock_t start = clock();
        DFS(startVertex);
        clock_t end = clock();
        double dfsTime = double(end - start) / CLOCKS_PER_SEC;
        cout << "Время выполнения DFS: " << dfsTime << " секунд\n";

        start = clock();
        BFS(startVertex);
        end = clock();
        double bfsTime = double(end - start) / CLOCKS_PER_SEC;
        cout << "Время выполнения BFS: " << bfsTime << " секунд\n";
        int count = MAX_VERTICES * MAX_VERTICES;
        cout << "\nМатрица смежности: " << count << ".\nСписок ребер: " << MAX_EDGES << ".\nСписок смежности: " << MAX_VERTICES+ MAX_EDGES << ".\n";

        int choice;
        cout << "\nПродолжить? (1 - да, 0 - нет): ";
        cin >> choice;
        if (choice == 0) {
            break;
        }

        system("cls");
    }

    return 0;
}



/*
Матрица смежности: Требует O(V²) памяти.
Список рёбер: Требует O(E) памяти.
Список смежности: Требует O(V + E) памяти.
*/