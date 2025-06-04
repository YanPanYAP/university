#define _CRT_SECURE_NO_WARNINGS
#include "Levenshtein.h"
#include "lcs.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <chrono> 
#include <random> 

#define FIRST 0
#define SECOND_RESULT 1
#define SECOND_TIME 0

std::string generate_random_string(int length) {
    static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    static std::random_device rd;                                        // Источник случайных чисел
    static std::mt19937 gen(rd());                                       // Генератор случайных чисел
    static std::uniform_int_distribution<> dis(0, sizeof(alphabet) - 2); // Равномерное распределение

    std::string result;
    for (int i = 0; i < length; ++i) {
        result += alphabet[dis(gen)]; 
    }
    return result;
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
#if FIRST
    int lx = 15; 
    int ly = 15; 
    std::string x = generate_random_string(lx);
    std::string y = generate_random_string(ly);

    std::cout << "Сгенерированная строка x: " << x << std::endl;
    std::cout << "Сгенерированная строка y: " << y << std::endl;

    std::cout << std::endl << "-- расстояние Левенштейна -----" << std::endl;
    std::cout << std::endl << "--длина --- рекурсия -- дин.програм. ---"
        << std::endl;

    std::cout << std::endl;
    std::cout << std::endl << "-- расстояние Левенштейна -----" << std::endl;
    std::cout << std::endl << "--длина --- рекурсия(мс) -- дин.програм.(мс) ---"
        << std::endl;

    for (int i = 8; i < std::min(lx, ly); i++)
    {
        // Измерение времени для рекурсивного метода
        auto start1 = std::chrono::high_resolution_clock::now();
        levenshtein_r(i, x, i - 2, y);
        auto end1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count();

        // Измерение времени для метода динамического программирования
        auto start2 = std::chrono::high_resolution_clock::now();
        levenshtein(i, x, i - 2, y);
        auto end2 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count();

        // Вывод результатов
        std::cout << std::right << std::setw(2) << i - 2 << "/" << std::setw(2) << i
            << "        " << std::left << std::setw(10) << duration1
            << "      " << std::setw(10) << duration2 << std::endl;
    }
#endif
#if SECOND_RESULT
    char z[100] = "";
    char x[] = "HRWCYUJ",
        y[] = "WLPCAU";

    int l = lcsd(x, y, z);
    std::cout << std::endl
        << "-- наибольшая общая подпоследовательость - LCS(динамическое"
        << "программирование)" << std::endl;
    std::cout << std::endl << "последовательость X: " << x;
    std::cout << std::endl << "последовательость Y: " << x;
    std::cout << std::endl << "                LCS: " << z;
    std::cout << std::endl << "          длина LCS: " << l;
    std::cout << std::endl;
#endif    
#if SECOND_TIME
    char z[100] = "";
    char x[] = "ABCBDABSDMOMSDFCK,SEFXK,SED",
        y[] = "BDCABACS:lmclsdomVPloxMC";

    int min_length = std::min(strlen(x), strlen(y));

    std::cout << std::endl << "-- Сравнение времени выполнения LCS (рекурсия vs динамическое программирование) --" << std::endl;
    std::cout << std::endl << "Длина | Рекурсия (мкс) | Динамическое программирование (мкс) | Длина LCS" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;

    for (int i = 1; i <= min_length; ++i) {
        char x_cut[100] = { 0 };
        char y_cut[100] = { 0 };

        // Обрезаем строки до длины i
        strncpy(x_cut, x, i);
        strncpy(y_cut, y, i);

        // Измерение времени для рекурсивного метода
        auto start_recursion = std::chrono::high_resolution_clock::now();
        int l_recursion = lcs(i, x_cut, i, y_cut); // Предполагается, что lcs - рекурсивная функция
        auto end_recursion = std::chrono::high_resolution_clock::now();
        auto duration_recursion = std::chrono::duration_cast<std::chrono::microseconds>(end_recursion - start_recursion).count();

        // Измерение времени для динамического программирования
        auto start_dp = std::chrono::high_resolution_clock::now();
        int l_dp = lcsd(x_cut, y_cut, z); // Предполагается, что lcsd - функция динамического программирования
        auto end_dp = std::chrono::high_resolution_clock::now();
        auto duration_dp = std::chrono::duration_cast<std::chrono::microseconds>(end_dp - start_dp).count();

        // Вывод результатов в таблицу
        std::cout << std::right << std::setw(5) << i << " | "
            << std::setw(14) << duration_recursion << " | "
            << std::setw(32) << duration_dp << " | "
            << std::setw(10) << l_dp << std::endl;
    }
#endif

    system("pause");
    std::cout << std::endl;

    return 0;
}