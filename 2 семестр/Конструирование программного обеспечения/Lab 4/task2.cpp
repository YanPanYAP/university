#include <iostream>
using namespace std;

typedef string NumberType;
typedef double NumberValue;

struct Number
{
    NumberType type;
    NumberValue value;
};

Number operator + (const Number& a, const Number& b)
{
    Number result;
    result.type = "Complex";
    result.value = a.value + b.value;
    return result;
}

Number operator - (const Number& a, const Number& b)
{
    Number result;
    result.type = "Real";
    result.value = a.value - b.value;
    return result;
}

int main()
{
    Number num1 = { "Integer", 5 };
    Number num2 = { "Real", 3.5 };

    Number sum = num1 + num2;
    Number difference = num1 - num2;

    cout << "Sum: " << sum.value << " (" << sum.type << ")" << endl;
    cout << "Difference: " << difference.value << " (" << difference.type << ")" << endl;

    return 0;
}
