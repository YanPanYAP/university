#include <iostream>

extern "C" 
{
	int getmin(int* arr, int len) {
		int min = INT_MAX;
		for (int i = 0; i < len; i++) {
			if (arr[i] < min) {
				min = arr[i];
			}
		}
		return min;
	}

	int getmax(int* arr, int len) {
		int max = INT_MIN;
		for (int i = 0; i < len; i++) {
			if (arr[i] > max) {
				max = arr[i];
			}
		}
		return max;
	}
}
