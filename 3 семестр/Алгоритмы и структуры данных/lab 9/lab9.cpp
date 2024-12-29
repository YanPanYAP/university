#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>
#include <locale>
#include <set>
using namespace std;

random_device rd;
mt19937 gen(rd());

// матрица расстояний
vector<vector<int>> generateGraph(int numCities) {
    vector<vector<int>> graph(numCities, vector<int>(numCities, 0));
    uniform_int_distribution<> dist(1, 100);

    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            if (i != j) {
                graph[i][j] = dist(gen);
            }
        }
    }
    return graph;
}

// начальная популяция
vector<vector<int>> initializePopulation(int populationSize, int numCities) {
    vector<vector<int>> population;
    vector<int> basePath(numCities);
    for (int i = 0; i < numCities; ++i) {
        basePath[i] = i;
    }

    for (int i = 0; i < populationSize; ++i) {
        shuffle(basePath.begin(), basePath.end(), gen);
        population.push_back(basePath);
    }
    return population;
}

// оценка маршрута
int evaluatePath(const vector<int>& path, const vector<vector<int>>& graph) {
    int totalDistance = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        totalDistance += graph[path[i]][path[i + 1]];
    }
    totalDistance += graph[path.back()][path[0]];
    return totalDistance;
}

// маппинг скрещивания
pair<vector<int>, vector<int>> crossover(const vector<int>& parent1, const vector<int>& parent2) {
    int n = parent1.size();
    uniform_int_distribution<> dist(0, n - 1);
    int start = dist(gen), end = dist(gen);
    if (start > end) swap(start, end);

    vector<int> offspring1(n, -1), offspring2(n, -1);

    for (int i = start; i <= end; ++i) {
        offspring1[i] = parent1[i];
        offspring2[i] = parent2[i];
    }

    auto fillOffspring = [&](const vector<int>& parent, vector<int>& offspring) {
        int index = 0;
        for (int i = 0; i < n; ++i) {
            if (offspring[i] == -1) {
                while (find(offspring.begin(), offspring.end(), parent[index]) != offspring.end()) {
                    index++;
                }
                offspring[i] = parent[index];
            }
        }
        };

    fillOffspring(parent2, offspring1);
    fillOffspring(parent1, offspring2);

    return { offspring1, offspring2 };
}

// мутация
void mutate(vector<int>& path, float mutationRate) {
    uniform_int_distribution<> dist(0, 100);
    if (dist(gen) < mutationRate * 100) {
        uniform_int_distribution<> idxDist(0, path.size() - 1);
        int i = idxDist(gen), j = idxDist(gen);
        if (i != j) {
            swap(path[i], path[j]);
        }
    }
}

void geneticAlgorithm(int numCities, int populationSize, int crossoverCount, float mutationRate, int generations, vector<vector<int>>& graph) {
    auto population = initializePopulation(populationSize, numCities);
    set<int> uniqueRoutes;
    int bestOverallDistance = numeric_limits<int>::max();
    vector<int> bestOverallRoute;

    for (int generation = 1; generation <= generations; ++generation) {
        vector<pair<int, vector<int>>> fitnessPopulation;
        for (const auto& path : population) {
            fitnessPopulation.emplace_back(evaluatePath(path, graph), path);
        }

        sort(fitnessPopulation.begin(), fitnessPopulation.end());

        cout << "\nПоколение: " << generation << endl;
        cout << "Лучшие маршруты в этом поколении:" << endl;

        for (int i = 0; i < fitnessPopulation.size(); ++i) {
            cout << "Маршрут: ";
            for (int city : fitnessPopulation[i].second) {
                cout << city << " ";
            }
            cout << " | Длина маршрута: " << fitnessPopulation[i].first << endl;
        }

        int bestRoute = fitnessPopulation[0].first;
        if (bestRoute < bestOverallDistance) {
            bestOverallDistance = bestRoute;
            bestOverallRoute = fitnessPopulation[0].second;
        }

        if (uniqueRoutes.count(bestRoute) == 0) {
            uniqueRoutes.insert(bestRoute);
            cout << "Кратчайший маршрут в этом поколении: ";
            for (int city : fitnessPopulation[0].second) cout << city << " ";
            cout << "\nДлина маршрута: " << bestRoute << endl;
        }

        vector<vector<int>> newPopulation;
        for (int i = 0; i < populationSize / 2; ++i) {
            newPopulation.push_back(fitnessPopulation[i].second);
        }

        while (newPopulation.size() < populationSize) {
            uniform_int_distribution<> dist(0, newPopulation.size() - 1);
            int p1 = dist(gen), p2 = dist(gen);
            auto offspring = crossover(newPopulation[p1], newPopulation[p2]);
            newPopulation.push_back(offspring.first);
            newPopulation.push_back(offspring.second);
        }

        if (newPopulation.size() > populationSize) {
            newPopulation.resize(populationSize);
        }

        for (auto& path : newPopulation) {
            mutate(path, mutationRate);
        }

        population = move(newPopulation);
    }

    cout << "\nСамый кратчайший маршрут за всё время: ";
    for (int city : bestOverallRoute) {
        cout << city << " ";
    }
    cout << "\nДлина маршрута: " << bestOverallDistance << endl;
}

void displayGraph(const vector<vector<int>>& graph) {
    cout << "Матрица расстояний:" << endl;
    cout << "\t";
    for (int i = 0; i < graph.size(); ++i) {
        cout << i << "\t";
    }
    cout << endl;

    for (int i = 0; i < graph.size(); ++i) {
        cout << i << "\t";
        for (int dist : graph[i]) {
            cout << dist << "\t";
        }
        cout << endl;
    }
}

void displayMenu() {
    cout << "Меню:" << endl;
    cout << "1. Добавить город" << endl;
    cout << "2. Удалить город" << endl;
    cout << "3. Добавить путь" << endl;
    cout << "4. Удалить путь" << endl;
    cout << "5. Запустить генетический алгоритм" << endl;
    cout << "Выберите действие: ";
}

void addCity(vector<vector<int>>& graph, int& numCities) {
    numCities++;
    graph.resize(numCities);
    for (int i = 0; i < numCities; ++i) {
        graph[i].resize(numCities, 0);
    }

    uniform_int_distribution<> dist(1, 100);
    for (int i = 0; i < numCities - 1; ++i) {
        graph[i][numCities - 1] = dist(gen);
        graph[numCities - 1][i] = dist(gen);
    }

    graph[numCities - 1][numCities - 1] = 0;
    system("cls");
    displayGraph(graph);
}

void removeCity(vector<vector<int>>& graph, int& numCities) {
    if (numCities > 0) {
        numCities--;
        graph.pop_back();
        for (auto& row : graph) {
            row.pop_back();
        }
        system("cls");
        displayGraph(graph);
    }
    else {
        cout << "Нет городов для удаления." << endl;
    }
}

void addPath(vector<vector<int>>& graph) {
    int city1, city2, distance;
    cout << "Введите номер первого города: ";
    cin >> city1;
    cout << "Введите номер второго города: ";
    cin >> city2;
    cout << "Введите расстояние: ";
    cin >> distance;

    if (city1 < graph.size() && city2 < graph.size()) {
        graph[city1][city2] = distance;
        system("cls");
        displayGraph(graph);
    }
    else {
        cout << "Некорректные номера городов." << endl;
    }
}

void removePath(vector<vector<int>>& graph) {
    int city1, city2;
    cout << "Введите номер первого города: ";
    cin >> city1;
    cout << "Введите номер второго города: ";
    cin >> city2;

    if (city1 < graph.size() && city2 < graph.size()) {
        graph[city1][city2] = 0;
        system("cls");
        displayGraph(graph);
    }
    else {
        cout << "Некорректные номера городов." << endl;
    }
}


int main() {
    wcout.imbue(locale("ru_RU.UTF-8"));
    wcin.imbue(locale("ru_RU.UTF-8"));
    int numCities, populationSize, crossoverCount, generations;
    float mutationRate;

    cout << "Введите количество городов: ";
    cin >> numCities;

    cout << "Введите размер популяции: ";
    cin >> populationSize;

    cout << "Введите количество скрещиваний: ";
    cin >> crossoverCount;

    cout << "Введите показатель мутации (0.0 - 1.0): ";
    cin >> mutationRate;

    cout << "Введите количество поколений: ";
    cin >> generations;

    vector<vector<int>> graph = generateGraph(numCities);
    displayGraph(graph);

    while (true) {
        displayMenu();
        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            addCity(graph, numCities);
            break;
        case 2:
            removeCity(graph, numCities);
            break;
        case 3:
            addPath(graph);
            break;
        case 4:
            removePath(graph);
            break;
        case 5:
            geneticAlgorithm(numCities, populationSize, crossoverCount, mutationRate, generations, graph);
            return 0;
        default:
            cout << "Неверный выбор. Пожалуйста, попробуйте снова." << endl;
        }
    }
}
