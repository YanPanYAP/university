//Доп 3-2
#include <iostream>
#include <fstream>

using namespace std;

void writeToFile(const char* filename, const char* data) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "Ошибка открытия файла " << filename << " для записи" << endl;
        return;
    }

    while (*data != '\0') {
        outFile.put(*data);
        ++data;
    }

    outFile.close();
}

bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool containsOnlyDigits(const char* word) {
    while (*word != '\0') {
        if (!isDigit(*word)) {
            return false;
        }
        ++word;
    }
    return true;
}

void readAndWriteWords(const char* inputFilename, const char* outputFilename) {
    ifstream inFile(inputFilename);
    if (!inFile.is_open()) {
        cout << "Ошибка открытия файла " << inputFilename << " для чтения" << endl;
        return;
    }

    ofstream outFile(outputFilename);
    if (!outFile.is_open()) {
        cout << "Ошибка открытия файла " << outputFilename << " для записи" << endl;
        inFile.close();
        return;
    }

    char currentChar;
    char word[256];
    int wordLength = 0;

    while (inFile.get(currentChar)) {
        if (isAlpha(currentChar)) {
            word[wordLength] = currentChar;
            ++wordLength;
        }
        else if (wordLength > 0) {
            word[wordLength] = '\0';
            if (!containsOnlyDigits(word)) {
                cout << word << endl;
                outFile << word << endl;
            }
            wordLength = 0;
        }
    }

    if (wordLength > 0) {
        word[wordLength] = '\0';
        if (!containsOnlyDigits(word)) {
            cout << word << endl;
            outFile << word << endl;
        }
    }

    inFile.close();
    outFile.close();
}

int main() {
    setlocale(LC_ALL, "ru");
    const char* inputFilename = "input.txt";
    const char* outputFilename = "output.txt";

    cout << "Введите строку символов: ";
    char inputString[256];
    cin.getline(inputString, sizeof(inputString));
    writeToFile(inputFilename, inputString);

    cout << "\nСлова в строке:\n";
    readAndWriteWords(inputFilename, outputFilename);

    cout << "\nПрограмма успешно завершена." << endl;

    return 0;
}