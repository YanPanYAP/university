#include <iostream>
using namespace std;

namespace call_1
{
	int defaultparm(int a, int b, int c, int k = 4, int f = 5)
	{
		return (a + b + c + k + f) / 5;
	}
}

namespace call_2
{
	int defaultparm(int a, int b, int c, int d, int e, int k = 6, int f = 7)
	{
		return (a + b + c + d + e + k + f) / 7;
	}
}

void main()
{
	int resualt1 = call_1::defaultparm(1, 2, 3);
	int resualt2 = call_2::defaultparm(1, 2, 3, 4, 5);

	cout << resualt1 << endl;
	cout << resualt2 << endl;
}