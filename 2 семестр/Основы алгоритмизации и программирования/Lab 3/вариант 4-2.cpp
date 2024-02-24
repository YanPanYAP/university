//Доп 4-2
#include<iostream>
#include<fstream>

using namespace std;

bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

void inputString(char* buff, int size) {
    cout << "Введите строку: " << endl;
    cin.getline(buff, size);
}

void findLongestAlphaWord(const char* buff, char* maxWord, int& max) {
    int count = 0;
    int i = 0;

    while (buff[i] != '\0') {
        if (isAlpha(buff[i])) {
            count++;
        }
        else {
            if (count > max) {
                max = count;

                for (int j = 0; j < count; j++) {
                    maxWord[j] = buff[i - count + j];
                }
                maxWord[count] = '\0';
            }
            count = 0;
        }
        i++;
    }

    if (count > max) {
        max = count;

        for (int j = 0; j < count; j++) {
            maxWord[j] = buff[i - count + j];
        }
        maxWord[count] = '\0';
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    char buff[100];
    ofstream file("FILE.txt");

    if (!file) {
        cout << "Ошибка открытия файла!" << endl;
        return 1;
    }

    inputString(buff, sizeof(buff));
    file << buff << endl;
    file.close();

    ifstream file1("FILE.txt");

    if (!file1) {
        cout << "Ошибка открытия файла!" << endl;
        return 1;
    }

    int max = 0;
    char maxWord[100];
    char line[100];

    while (file1.getline(line, sizeof(line))) {
        findLongestAlphaWord(line, maxWord, max);
    }

    file1.close();

    if (max > 0) {
        cout << "Самое длинное слово: " << maxWord << endl;
        cout << "Длина самого длинного слова: " << max << endl;
    }
    else {
        cout << "Строка не содержит слов." << endl;
    }

    return 0;
}