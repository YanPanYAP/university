//Доп 12-2
#include <iostream>
#include <fstream>
using namespace std;

void inFile(ifstream& f, char s[100]) {
    f.open(s);
    if (f.fail()) {
        cout << "\n Ошибка открытия файла";
        exit(1);
    }

    char word[100];
    while (f >> word) {

        if (isdigit(word[0])) {
            int number = atoi(word);
            if (number % 2 != 0) {
                cout << number << endl;
            }
        }
    }

    f.close();
}

void fromFile(ofstream& f, char s[100]) {
    f.open(s);
    if (f.fail()) {
        cout << "\n Ошибка открытия файла";
        exit(1);
    }
    else {
        cout << "Введите строку: " << endl;
        cin.ignore();
        cin.getline(s, 100);
        f << s;
    }

    f.close();
}

int main() {
    setlocale(LC_CTYPE, "RU");
    char str[100];
    ifstream ifile;
    ofstream ofile;

    cout << "\n Ввести имя файла для записи: \n";
    cin >> str;
    fromFile(ofile, str);
    cout << endl;

    cout << "\n Ввести имя файла для чтения: \n";
    cin >> str;
    inFile(ifile, str);

    return 0;
}