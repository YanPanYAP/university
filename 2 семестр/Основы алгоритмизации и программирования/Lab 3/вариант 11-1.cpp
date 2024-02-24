#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    setlocale(LC_ALL, "rus");
    ifstream file1("FILE1");
    ofstream file2("FILE2");

    char line[100];
    int charCount = 0;

    while (file1.getline(line, 100))
    {
        int wordCount = 0;
        for (int i = 0; line[i] != '\0'; i++)
        {
            if (line[i] == ' ')
            {
                wordCount++;
                break;
            }
        }

        if (wordCount == 0)
        {
            file2 << line << endl;
            charCount += strlen(line);
        }
    }

    cout << "Количество символов в файле FILE2: " << charCount << endl;

    file1.close();
    file2.close();

    return 0;
}