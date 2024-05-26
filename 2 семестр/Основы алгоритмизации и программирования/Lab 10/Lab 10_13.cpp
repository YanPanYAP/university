#include <iostream>
using namespace std;

int dnx(int n, int x) {

    if (x == 1) {
        return 1;
    }
    else
    {
        int count = dnx(n, x - 1);
        if (n % x == 0)
        {
            count++;
        }
        return count;
    }
}

int dn(int n) {
    return dnx(n, n);
}

int main()
{
    setlocale(LC_CTYPE, "rus");
    int n;
    cout << "¬ведите натуральное число n: ";
    cin >> n;

    int total_divisors = dn(n);

    cout << " оличество всех положительных делителей числа " << n << ": " << total_divisors << endl;

    return 0;
}
