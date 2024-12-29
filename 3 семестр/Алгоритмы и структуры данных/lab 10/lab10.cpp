#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <iomanip>
#include <random>
#include <locale>

using namespace std;

random_device rd;
mt19937 gen(rd());

// Генерация случайного графа
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

// Вывод матрицы расстояний
void displayGraph(const vector<vector<int>>& graph) {
    cout << "Матрица расстояний:" << endl;
    for (const auto& row : graph) {
        for (int val : row) {
            cout << setw(5) << val << " ";
        }
        cout << endl;
    }
}

// Вычисление длины маршрута
int calculateRouteDistance(const vector<int>& route, const vector<vector<int>>& graph) {
    int distance = 0;
    for (size_t i = 0; i < route.size() - 1; ++i) {
        distance += graph[route[i]][route[i + 1]];
    }
    distance += graph[route.back()][route[0]]; // Возвращение в начальный город
    return distance;
}

// Основной муравьиный алгоритм
void antColonyOptimization(const vector<vector<int>>& graph, int numCities, double alpha, double beta, double evaporationRate, int iterations, double initialPheromone) {
    // Инициализация феромонов
    vector<vector<double>> pheromones(numCities, vector<double>(numCities, initialPheromone));
    vector<int> bestRoute;
    int bestDistance = numeric_limits<int>::max();

    for (int iter = 1; iter <= iterations; ++iter) {
        vector<vector<int>> allRoutes; // Маршруты всех муравьев
        vector<int> distances;        // Расстояния маршрутов

        // Каждый муравей строит маршрут
        for (int ant = 0; ant < numCities; ++ant) {
            vector<int> route = { ant };
            vector<bool> visited(numCities, false);
            visited[ant] = true;

            while (route.size() < numCities) {
                int currentCity = route.back();
                vector<double> probabilities(numCities, 0.0);
                double sum = 0.0;

                // Вычисляем вероятность выбора каждого следующего города
                for (int nextCity = 0; nextCity < numCities; ++nextCity) {
                    if (!visited[nextCity]) {
                        double pheromone = pheromones[currentCity][nextCity];
                        double distance = 1.0 / graph[currentCity][nextCity];
                        probabilities[nextCity] = pow(pheromone, alpha) * pow(distance, beta);
                        sum += probabilities[nextCity];
                    }
                }

                // Выбор следующего города на основе вероятности
                uniform_real_distribution<> dist(0.0, sum);
                double randomValue = dist(gen);
                int nextCity = -1;
                for (int city = 0; city < numCities; ++city) {
                    if (!visited[city]) {
                        randomValue -= probabilities[city];
                        if (randomValue <= 0.0) {
                            nextCity = city;
                            break;
                        }
                    }
                }

                // Добавляем следующий город в маршрут
                route.push_back(nextCity);
                visited[nextCity] = true;
            }

            // Добавляем маршрут муравья и его расстояние
            allRoutes.push_back(route);
            distances.push_back(calculateRouteDistance(route, graph));
        }

        // Обновление лучшего маршрута
        for (int i = 0; i < distances.size(); ++i) {
            if (distances[i] < bestDistance) {
                bestDistance = distances[i];
                bestRoute = allRoutes[i];
            }
        }

        // Испарение феромонов
        for (int i = 0; i < numCities; ++i) {
            for (int j = 0; j < numCities; ++j) {
                pheromones[i][j] *= (1.0 - evaporationRate);
            }
        }

        // Обновление феромонов
        for (int i = 0; i < allRoutes.size(); ++i) {
            for (int j = 0; j < allRoutes[i].size() - 1; ++j) {
                int from = allRoutes[i][j];
                int to = allRoutes[i][j + 1];
                pheromones[from][to] += 1.0 / distances[i];
                pheromones[to][from] += 1.0 / distances[i];
            }
        }

        // Вывод информации об итерации
        cout << "Итерация " << iter << ":\n";
        for (int i = 0; i < allRoutes.size(); ++i) {
            cout << "  Маршрут муравья " << i + 1 << ": ";
            for (int city : allRoutes[i]) cout << city << " ";
            cout << "| Длина: " << distances[i] << endl;
        }
        cout << "Лучший маршрут: ";
        for (int city : bestRoute) cout << city << " ";
        cout << "| Длина: " << bestDistance << "\n" << endl;
    }

    // Вывод окончательного результата
    cout << "Самый лучший маршрут: ";
    for (int city : bestRoute) cout << city << " ";
    cout << "| Длина: " << bestDistance << endl;
}

int main() {
    wcout.imbue(locale("ru_RU.UTF-8"));
    wcin.imbue(locale("ru_RU.UTF-8"));
    int numCities, iterations;
    double alpha, beta, evaporationRate, initialPheromone;

    cout << "Введите количество городов: ";
    cin >> numCities;

    auto graph = generateGraph(numCities);
    displayGraph(graph);

    cout << "Введите начальное значение феромонов: ";
    cin >> initialPheromone;
    cout << "Введите альфа: ";
    cin >> alpha;
    cout << "Введите бета: ";
    cin >> beta;
    cout << "Введите коэффициент испарения (0.0 - 1.0): ";
    cin >> evaporationRate;
    cout << "Введите количество итераций: ";
    cin >> iterations;

    antColonyOptimization(graph, numCities, alpha, beta, evaporationRate, iterations, initialPheromone);

    return 0;
}
