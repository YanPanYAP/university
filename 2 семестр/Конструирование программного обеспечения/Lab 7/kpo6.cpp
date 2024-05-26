#include <iostream>
#include "kpo6L.h"
using namespace std;

namespace call
{
	int _cdecl cdevl(int x, int y, int z)
	{
		return x + y + z;
	}

	int _stdcall cstd(int& x, int y, int z)
	{
		return x * y * z;
	}

	int _fastcall cfst(int x, int y, int z, int d)
	{
		return x + y + z + d;
	}
}

int main()
{
	int a = 4;
	int& b = a;

	int rc_cdecl = call::cdevl(1, 2, 3);
	int rc_stdcall = call::cstd(b, 5, 6);
	int rc_fastcall = call::cfst(7, 8, 9, 10);

	cout << rc_cdecl << endl;
	cout << rc_stdcall << endl;
	cout << rc_fastcall << endl;

	return 0;
}