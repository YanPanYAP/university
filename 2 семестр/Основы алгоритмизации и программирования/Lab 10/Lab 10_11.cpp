#include <iostream>
using namespace std;

int sum(int n, int x) {

	if (n == 0)
	{
		return 0;
	}

	if (n == 1)
	{
		return 2 * x;
	}
	if (n > 1)
	{
		return 2 * n / (n - 1) * sum(n - 1, x) + n - 1 / (2 * n) * sum(n - 2, x);
	}

}

void main()
{
	setlocale(LC_CTYPE, "rus");
	int x, n;

	cout << "¬веедите x: ", cin >> x;
	cout << "¬ведите n: ", cin >> n;
	cout << "\n—умма: " << sum(n, x) << endl;
}