#include <iostream>
#pragma comment(lib,"..\\Debug\\SE_asm01a.lib")
using namespace std;

extern "C"
{
	int _stdcall getmin(int*, int);
	int _stdcall getmax(int*, int);
};

int main() {
	setlocale(LC_ALL, "ru");
	int array[10] = { 10, 2, 5, -9, 45, 49, 1, 11, -8, 0 };
	int max = getmax(array, 10);
	int min = getmin(array, 10);
	int result = min + max;
	cout << max << " + " << min << " = " << result << endl;

	return 0;
}