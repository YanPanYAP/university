#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    setlocale(LC_ALL, "RUS");
    char line1[100], line2[100];
    ofstream file("file.txt");

    cout << "������� ������ ������: ";
    cin.getline(line1, 100);
    file << line1 << endl;

    cout << "������� ������ ������: ";
    cin.getline(line2, 100);
    file << line2 << endl;

    file.close();

    ifstream readFile("file.txt");
    char word[100], shortestWord[100], longestWord[100];

    readFile >> word;
    strcpy_s(shortestWord, word);
    strcpy_s(longestWord, word);

    while (readFile >> word)
    {
        if (strlen(word) < strlen(shortestWord))
        {
            strcpy_s(shortestWord, word);
        }
        if (strlen(word) > strlen(longestWord))
        {
            strcpy_s(longestWord, word);
        }
    }

    cout << "����� �������� ����� � ������ ������: " << shortestWord << endl;
    cout << "����� ������� ����� �� ������ ������: " << longestWord << endl;

    readFile.close();

    return 0;
}