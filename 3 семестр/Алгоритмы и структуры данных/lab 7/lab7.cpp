#include <iostream>
#include <cstdlib>
#include <ctime>
#include <locale>
#include <Windows.h>
using namespace std;

int main()
{
    wcout.imbue(locale("ru_RU.UTF-8"));
    srand(static_cast<unsigned>(time(nullptr)));
    int N = 0;
    wcout << L"Введите N: ";
    cin >> N;
    wcout << L"\nStart: ";

    int* nArray = new int[N];
    for (int i = 0; i < N; i++) {
        nArray[i] = rand() % 201 - 100;
        cout << nArray[i] << " ";
    }
    cout << endl;

    if (N == 0 || N < 0) {
        wcout << L"No elements in the sequence." << endl;
        delete[] nArray;
        return 0;
    }

    int* lengths = new int[N];
    int* previous = new int[N];
    int maxLength = 1;
    int maxIndex = 0;

    for (int i = 0; i < N; i++) {
        lengths[i] = 1;
        previous[i] = -1;
    }

    // длины
    for (int i = 1; i < N; i++) {
        for (int j = 0; j < i; j++) {
            if (nArray[j] < nArray[i] && lengths[j] + 1 > lengths[i]) {
                lengths[i] = lengths[j] + 1;
                previous[i] = j;
            }
        }

        if (lengths[i] > maxLength) {
            maxLength = lengths[i];
            maxIndex = i;
        }
    }

    wcout << L"Length of the longest: " << maxLength << endl;

    int* subsequence = new int[maxLength];
    int currentIndex = maxIndex;
    for (int i = maxLength - 1; i >= 0; i--) {
        subsequence[i] = nArray[currentIndex];
        currentIndex = previous[currentIndex];
    }

    wcout << L"Longest: ";
    for (int i = 0; i < maxLength; i++) {
        cout << subsequence[i] << " ";
    }
    cout << endl;

    delete[] nArray;
    delete[] lengths;
    delete[] previous;
    delete[] subsequence;

    return 0;
}
