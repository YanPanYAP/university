#include <iostream>
#include <climits>

using namespace std;

const int MAX_VERTICES = 9;
const int INF = INT_MAX;

struct Edge {
    int dest, weight;
};

Edge adjList[MAX_VERTICES][MAX_VERTICES];
int edgeCount[MAX_VERTICES];

void initializeGraph() {
    for (int i = 0; i < MAX_VERTICES; i++) {
        edgeCount[i] = 0;
    }

    // ������� A (0) ��������� � ��������� B (1) � C (2)
    adjList[0][edgeCount[0]++] = { 1, 7 };
    adjList[0][edgeCount[0]++] = { 2, 10 };

    // ������� B (1) ��������� � ��������� A (0), F (5) � G (6)
    adjList[1][edgeCount[1]++] = { 0, 7 };
    adjList[1][edgeCount[1]++] = { 5, 9 };
    adjList[1][edgeCount[1]++] = { 6, 27 };

    // ������� C (2) ��������� � ��������� A (0), F (5) � E (4)
    adjList[2][edgeCount[2]++] = { 0, 10 };
    adjList[2][edgeCount[2]++] = { 5, 8 };
    adjList[2][edgeCount[2]++] = { 4, 31 };

    // ������� D (3) ��������� � ��������� E (4), H (7), I (8)
    adjList[3][edgeCount[3]++] = { 4, 32 };
    adjList[3][edgeCount[3]++] = { 7, 17 };
    adjList[3][edgeCount[3]++] = { 8, 21 };

    // ������� E (4) ��������� � ��������� D (3) � C (2)
    adjList[4][edgeCount[4]++] = { 3, 32 };
    adjList[4][edgeCount[4]++] = { 2, 31 };

    // ������� F (5) ��������� � ��������� B (1), C (2) � H (7)
    adjList[5][edgeCount[5]++] = { 1, 9 };
    adjList[5][edgeCount[5]++] = { 2, 8 };
    adjList[5][edgeCount[5]++] = { 7, 11 };

    // ������� G (6) ��������� � ��������� B (1) � I (8)
    adjList[6][edgeCount[6]++] = { 1, 27 };
    adjList[6][edgeCount[6]++] = { 8, 15 };

    // ������� H (7) ��������� � ��������� F (5), D (3) � I (8)
    adjList[7][edgeCount[7]++] = { 5, 11 };
    adjList[7][edgeCount[7]++] = { 3, 17 };
    adjList[7][edgeCount[7]++] = { 8, 15 };

    // ������� I (8) ��������� � ��������� G (6), D (3) � H (7)
    adjList[8][edgeCount[8]++] = { 6, 15 };
    adjList[8][edgeCount[8]++] = { 3, 21 };
    adjList[8][edgeCount[8]++] = { 7, 15 };
}

int findMinDistance(int dist[], bool visited[]) {
    int minIndex = -1;
    int minDist = INF;
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (!visited[i] && dist[i] < minDist) {
            minDist = dist[i];
            minIndex = i;
        }
    }
    return minIndex;
}

void dijkstra(int start) {
    int dist[MAX_VERTICES];
    bool visited[MAX_VERTICES];

    // ������������� ���������� � ������� ���������
    for (int i = 0; i < MAX_VERTICES; i++) {
        dist[i] = INF;
        visited[i] = false;
    }
    dist[start] = 0;

    for (int i = 0; i < MAX_VERTICES; i++) {
        int u = findMinDistance(dist, visited);
        if (u == -1) break;  // ��� ������� ����������
        visited[u] = true;

        // ������������ ������� �������
        for (int j = 0; j < edgeCount[u]; j++) {
            Edge edge = adjList[u][j];
            int v = edge.dest;
            int weight = edge.weight;

            // ���� ������ ���������� ����, ��������� ����������
            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }

    // ������� ���������� �� ��������� �������
    cout << "���������� �� ������� " << char(start + 'A') << ":\n";
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (dist[i] == INF) {
            cout << char(i + 'A') << ": �����������\n";
        }
        else {
            cout << char(i + 'A') << ": " << dist[i] << "\n";
        }
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    initializeGraph();

    while (true) {
        char startVertex;
        cout << "������� ��������� ������� (A-I): ";
        cin >> startVertex;

        if (startVertex < 'A' || startVertex > 'I') {
            cout << "������: ������� ������� �� A �� I.\n\n";
            continue;
        }

        int start = startVertex - 'A';
        dijkstra(start);

        int choice;
        cout << "\n����������? (1 - ��, 0 - ���): ";
        cin >> choice;
        if (choice == 0) {
            break;
        }
        system("cls");
    }

    return 0;
}
