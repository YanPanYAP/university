//Доп 12-1
#include<iostream>
#include<fstream>

using namespace std;

int main() {
    setlocale(LC_ALL, "rus");
    char line[1000];
    char word[100];
    char first_word[100];

    ofstream out("FILE2.txt");
    ifstream from("FILE1.txt");
    if (!from || !out) {
        cout << "Файл не открыт\n";
    }
    else {
        cout << "Файл открыт\n";
    }

    from >> first_word;
    cout << "Первое слово:" << first_word << endl;


    while (from.getline(line, 1000, '\n')) {
        bool set = false;
        int i = 0;
        int wordIndex = 0;
        while (line[i] != '\0') {
            if (line[i] != ' ') {
                word[wordIndex++] = line[i];
            }
            else if (line[i] == ' ') {
                word[wordIndex] = '\0';
                bool wordMatch = true;
                for (int k = 0; k < wordIndex; k++) {
                    if (word[k] != first_word[k]) {
                        wordMatch = false;
                        break;
                    }
                }
                if (wordMatch) {
                    set = true;
                    break;
                }
                wordIndex = 0;
            }
            i++;
        }
        if (set) {
            out << line << endl;
        }
    }


    from.close();
    out.close();

    int letters = 0;

    ifstream fin2("FILE2.txt");
    if (!fin2.is_open())
        cout << "Файл не может быть открыт!\n";
    else {
        while (fin2.getline(line, 1000)) {

            for (int i = 0; line[i] != '\0'; ++i) {
                if (isalpha(line[i])) {
                    letters++;
                }
            }
        }
    }

    fin2.close();
    cout << "В файле FILE2 " << letters << " букв." << endl;

    return 0;


}