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
        cout << "—кобок нет" << endl;
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
    cout << "¬ведите строку: ";
    getline(cin, input);

    if (checkBrackets(input))
    {
        cout << "скобки расставлены верно" << endl;
    }
    else
    {
        cout << "скобки расставлены не верно" << endl;
    }

    return 0;
}



