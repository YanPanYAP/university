// Main      
#include "stdafx.h"
using namespace std;

// Определите макросы для включения/выключения заданий
#define TASK_SUBSET_GENERATOR 1
#define TASK_KNAPSACK 0
#define TASK_COMBINATION_GENERATOR 0
#define SHIP_LOAD 0
#define PERMUTATION 0
#define SALESMAN 0
#define SALESMAN_TIME 0
#define TASK_ARRANGEMENT 1

#define NN (sizeof(c)/sizeof(int))
#define MM 3
#define SPACE(n) std::setw(n)<<" "
#define N 5

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

#if TASK_ARRANGEMENT
    
#endif

#if TASK_SUBSET_GENERATOR
    // Генератор множества подмножеств
    char  AA[][2] = { "A", "B", "C", "D" };
    std::cout << std::endl << " - Генератор множества всех подмножеств -";
    std::cout << std::endl << "Исходное множество: ";
    std::cout << "{ ";
    for (int i = 0; i < sizeof(AA) / 2; i++)
        std::cout << AA[i] << ((i < sizeof(AA) / 2 - 1) ? ", " : " ");
    std::cout << "}";
    std::cout << std::endl << "Генерация всех подмножеств  ";
    combi::subset s1(sizeof(AA) / 2);         // создание генератора   
    int  n = s1.getfirst();                  // первое (пустое) подмножество    
    while (n >= 0)                          // пока есть подмножества 
    {
        std::cout << std::endl << "{ ";
        for (int i = 0; i < n; i++)
            std::cout << AA[s1.ntx(i)] << ((i < n - 1) ? ", " : " ");
        std::cout << "}";
        n = s1.getnext();                   // cледующее подмножество 
    };
    std::cout << std::endl << "всего: " << s1.count() << std::endl;
#endif

#if TASK_KNAPSACK
    // Задача о рюкзаке
    int V = 600,              // вместимость рюкзака              
        v[] = { 25, 56, 67, 40, 20, 27, 37, 33, 33, 44, 53, 12,
               60, 75, 12, 55, 54, 42, 43, 14, 30, 37, 31, 12 },
        c[] = { 15, 26, 27, 43, 16, 26, 42, 22, 34, 12, 33, 30,
               12, 45, 60, 41, 33, 11, 14, 12, 25, 41, 30, 40 };
    short m[NN];
    int maxcc = 0;
    clock_t t1, t2;
    std::cout << std::endl << "-------- Задача о рюкзаке --------- ";
    std::cout << std::endl << "- вместимость рюкзака  : " << V;
    std::cout << std::endl << "-- количество ------ продолжительность -- ";
    std::cout << std::endl << "    предметов          вычисления  ";
    for (int i = 14; i <= NN; i++)
    {
        t1 = clock();
        maxcc = knapsack_s(V, i, v, c, m);
        t2 = clock();
        std::cout << std::endl << "       " << std::setw(2) << i
            << "               " << std::setw(5) << (t2 - t1);
    }
    std::cout << std::endl << std::endl;
#endif

#if TASK_COMBINATION_GENERATOR
    // Генератор сочетаний
    char  AAComb[][2] = { "A", "B", "C", "D", "E" };
    std::cout << std::endl << " --- Генератор сочетаний ---";
    std::cout << std::endl << "Исходное множество: ";
    std::cout << "{ ";
    for (int i = 0; i < sizeof(AAComb) / 2; i++)
        std::cout << AAComb[i] << ((i < sizeof(AAComb) / 2 - 1) ? ", " : " ");

    std::cout << "}";
    std::cout << std::endl << "Генерация сочетаний ";
    combi::xcombination xc(sizeof(AAComb) / 2, 3);
    std::cout << "из " << xc.n << " по " << xc.m;
    int  nComb = xc.getfirst();
    while (nComb >= 0)
    {

        std::cout << std::endl << xc.nc << ": { ";

        for (int i = 0; i < nComb; i++)


            std::cout << AAComb[xc.ntx(i)] << ((i < nComb - 1) ? ", " : " ");

        std::cout << "}";

        nComb = xc.getnext();
    };
    std::cout << std::endl << "всего: " << xc.count() << std::endl;
#endif

#if SHIP_LOAD
    int V = 1000,
        v[] = { 250, 560, 670, 400, 200, 270, 370, 330, 330, 440, 530, 120,
               200, 270, 370, 330, 330, 440, 700, 120, 550, 540, 420, 170,
               600, 700, 120, 550, 540, 420, 430, 140, 300, 370, 310, 120 };
    int c[] = { 15,26,  27,  43,  16,  26,  42,  22,  34,  12,  33,  30,
               42,22,  34,  43,  16,  26,  14,  12,  25,  41,  17,  28,
               12,45,  60,  41,  33,  11,  14,  12,  25,  41,  30,  40 };
    short r[MM];
    int maxcc = 0;
    clock_t t1, t2;
    std::cout << std::endl << "-- Задача об оптимальной загрузке судна -- ";
    std::cout << std::endl << "-  ограничение по весу    : " << V;
    std::cout << std::endl << "-  количество мест        : " << MM;
    std::cout << std::endl << "-- количество ------ продолжительность -- ";
    std::cout << std::endl << "   контейнеров        вычисления  ";
    for (int i = 24; i <= NN; i++)
    {
        t1 = clock();
        int maxcc = boat(V, MM, i, v, c, r);
        t2 = clock();
        std::cout << std::endl << SPACE(7) << std::setw(2) << i
            << SPACE(15) << std::setw(5) << (t2 - t1);
    }

    std::cout << std::endl << std::endl;
#endif

#if PERMUTATION
    char  AAPermut[][2] = { "A", "B", "C", "D" };
    std::cout << std::endl << " --- Генератор перестановок ---";
    std::cout << std::endl << "Исходное множество: ";
    std::cout << "{ ";
    for (int i = 0; i < sizeof(AAPermut) / 2; i++)

        std::cout << AAPermut[i] << ((i < sizeof(AAPermut) / 2 - 1) ? ", " : " ");
    std::cout << "}";
    std::cout << std::endl << "Генерация перестановок ";
    combi::permutation p(sizeof(AAPermut) / 2);
    __int64  nPermut = p.getfirst();
    while (nPermut >= 0)
    {
        std::cout << std::endl << std::setw(4) << p.np << ": { ";

        for (int i = 0; i < p.n; i++)

            std::cout << AAPermut[p.ntx(i)] << ((i < p.n - 1) ? ", " : " ");

        std::cout << "}";

        nPermut = p.getnext();
    };
    std::cout << std::endl << "всего: " << p.count() << std::endl;
#endif

#if SALESMAN
    int d[N][N] = { //0   1    2    3     4        
                  {  INF, 10,  26,  INF,   5},    //  0
                  {  5,  INF,  20,   63,   79},    //  1
                  {  7,   15, INF,   86,   54},    //  2 
                  { 23,   53,  20,  INF,   15},    //  3
                  { 88,   71,  52,   18,  INF}     //  4 
    };
    int r[N];                     // результат 
    int s = salesman(
        N,          // [in]  количество городов 
        (int*)d,          // [in]  массив [n*n] расстояний 
        r           // [out] массив [n] маршрут 0 x x x x  

    );
    std::cout << std::endl << "-- Задача коммивояжера -- ";
    std::cout << std::endl << "-- количество  городов: " << N;
    std::cout << std::endl << "-- матрица расстояний : ";
    for (int i = 0; i < N; i++)
    {
        std::cout << std::endl;
        for (int j = 0; j < N; j++)

            if (d[i][j] != INF) std::cout << std::setw(3) << d[i][j] << " ";

            else std::cout << std::setw(3) << "INF" << " ";
    }
    std::cout << std::endl << "-- оптимальный маршрут: ";
    for (int i = 0; i < N; i++) std::cout << r[i] << "-->"; std::cout << 0;
    std::cout << std::endl << "-- длина маршрута     : " << s;
    std::cout << std::endl;
#endif

#if SALESMAN_TIME
    int dSalesmanTime[N * N + 1], rSalesmanTime[N];
    auxil::start();
    for (int i = 0; i <= N * N; i++) {
        dSalesmanTime[i] = auxil::iget(10, 100);
    }
    std::cout << std::endl << "-- Задача коммивояжера -- ";
    std::cout << std::endl << "-- количество ------ продолжительность -- ";
    std::cout << std::endl << "      городов           вычисления  ";
    clock_t t1, t2;
    for (int i = 7; i <= N; i++)
    {
        t1 = clock();
        salesman(i, (int*)dSalesmanTime, rSalesmanTime);
        t2 = clock();
        std::cout << std::endl << SPACE(7) << std::setw(2) << i
            << SPACE(15) << std::setw(5) << (t2 - t1);
    }
    std::cout << std::endl;
#endif

    system("pause");
    return 0;
}