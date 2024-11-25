#include <iostream>
#include <vector>
#include <locale>
#include <algorithm>

using namespace std;

void findAllSubsequences(int* nArray, vector<vector<int>>& previous, int currentIndex, vector<int>& currentSubsequence, vector<vector<int>>& printed) {
    currentSubsequence.push_back(nArray[currentIndex]);

    if (previous[currentIndex].empty()) {
        vector<int> resultSubsequence = currentSubsequence;
        reverse(resultSubsequence.begin(), resultSubsequence.end());

        for (const auto& seq : printed) {
            if (seq == resultSubsequence) {
                currentSubsequence.pop_back();
                return;
            }
        }

        printed.push_back(resultSubsequence);
        for (int val : resultSubsequence) {
            cout << val << " ";
        }
        cout << endl;
    }
    else {
        for (int prevIndex : previous[currentIndex]) {
            findAllSubsequences(nArray, previous, prevIndex, currentSubsequence, printed);
        }
    }

    currentSubsequence.pop_back();
}

int main() {
    wcout.imbue(locale("ru_RU.UTF-8"));
    int n;
    wcout << L"Введите N: ";
    cin >> n;

    if (n <= 0) {
        wcout << L"Неверный ввод." << endl;
        return 0;
    }

    int* nArray = new int[n];
    wcout << L"Введите массив: ";
    for (int i = 0; i < n; i++) {
        cin >> nArray[i];
    }

    vector<int> lengths(n, 1);
    vector<vector<int>> previous(n);
    int maxLength = 1;

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nArray[j] < nArray[i]) {
                if (lengths[j] + 1 > lengths[i]) {
                    lengths[i] = lengths[j] + 1;
                    previous[i].clear();
                    previous[i].push_back(j);
                }
                else if (lengths[j] + 1 == lengths[i]) {
                    previous[i].push_back(j);
                }
            }
        }
        maxLength = max(maxLength, lengths[i]);
    }

    cout << "\nДлина наибольшей: " << maxLength << endl;

    cout << "\nПоследовательности: " << endl;
    vector<vector<int>> printed;
    vector<int> currentSubsequence;
    for (int i = 0; i < n; i++) {
        if (lengths[i] == maxLength) {
            findAllSubsequences(nArray, previous, i, currentSubsequence, printed);
        }
    }

    delete[] nArray;

    return 0;
}
