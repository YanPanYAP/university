//Доп 3-1
#include <iostream>
#include <fstream>

using namespace std;

int countVowels(const char* str) {
    int count = 0;
    while (*str != '\0') {
        char c = *str;
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            count++;
        }
        str++;
    }
    return count;
}

int main() {
    setlocale(LC_ALL, "ru");

    ifstream file1Read("FILE1.txt");
    ofstream file2("FILE2.txt");
    if (!file1Read.is_open() || !file2.is_open()) {
        cout << "Ошибка открытия файлов FILE1.txt или FILE2.txt" << endl;
        return 1;
    }

    int k;
    cout << "Введите k() " << endl;
    cin >> k;
    char line[256];

    for (int i = 0; i <= k + 3; ++i) {
        if (file1Read.getline(line, sizeof(line))) {
            if (i >= k) {
                file2 << line << endl;;
            }
        }
        else if (!file1Read.eof()) {
            cout << "Ошибка чтения из файла FILE1.txt" << endl;
            return 1;
        }
    }

    file1Read.close();
    file2.close();

    ifstream file2Read("FILE2.txt");
    if (!file2Read.is_open()) {
        cout << "Ошибка открытия файла FILE2.txt" << endl;
        return 1;
    }

    int vowelsCount = 0;
    while (file2Read.getline(line, sizeof(line))) {
        vowelsCount += countVowels(line);
    }

    file2Read.close();

    cout << "Количество гласных букв в FILE2: " << vowelsCount << endl;

    return 0;
}