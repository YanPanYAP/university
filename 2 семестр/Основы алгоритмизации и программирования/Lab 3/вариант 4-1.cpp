//��� 4-1
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    setlocale(LC_CTYPE, "RU");
    char buff[50];

    ofstream fout("FILE2.txt");
    ifstream fin("FILE1.txt");

    if (!fin.is_open()) {
        cout << "���� �� ����� ���� ������!\n";
    }

    while (fin.getline(buff, 50)) {
        if (strcspn(buff, "0123456789") == strlen(buff)) {
            fout << buff << endl;
        }
    }

    fin.close();
    fout.close();

    int consonants = 0;

    ifstream fin2("FILE2.txt");
    if (!fin2.is_open()) {
        cout << "���� �� ����� ���� ������!\n";
    }
    else {

        while (fin2.getline(buff, 50)) {

            if (buff[0] == 'A') {
                consonants++;
            }

        }
    }

    fin2.close();
    cout << "� ����� FILE2 " << consonants << " �����, ������������ �� ����� �." << endl;

    return 0;


}