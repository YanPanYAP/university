#include <iostream>
#include <Windows.h>
#include <stack>
#include <string>
using namespace std;

bool checkBrackets(const string& input)
{
    stack<char> bracketStack;

    for (char ch : input)
    {
        if (ch == '(' || ch == '[' || ch == '{')
        {
            bracketStack.push(ch);
        }
        else if (ch == ')' || ch == ']' || ch == '}')
        {
            if (bracketStack.empty())
            {
                return false;
            }

            char top = bracketStack.top();
            bracketStack.pop();

            if ((ch == ')' && top != '(') ||
                (ch == ']' && top != '[') ||
                (ch == '}' && top != '{')) {
                return false;
            }
        }
    }
    if (bracketStack.empty())
    {
        cout << "������ ���" << endl;
        system("pause");
    }
    return bracketStack.empty();
}

int main()
{
    setlocale(LC_ALL, "Rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string input;
    cout << "������� ������: ";
    getline(cin, input);

    if (checkBrackets(input))
    {
        cout << "������ ����������� �����" << endl;
    }
    else
    {
        cout << "������ ����������� �� �����" << endl;
    }

    return 0;
}



