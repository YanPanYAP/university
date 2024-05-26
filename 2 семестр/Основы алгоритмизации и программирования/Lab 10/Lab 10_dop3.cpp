#include <iostream>
#include <ctime>
using namespace std;

const int n = 5;
int nums[n];

void outOfTransposition(int k)
{
    if (k == n)
    {
        for (int i = 0; i < n; i++)
        {
            cout << " " << nums[i];
        }
        cout << endl;
    }
    else
    {
        for (int i = k; i < n; i++)
        {
            swap(nums[k], nums[i]);
            outOfTransposition(k + 1);
            swap(nums[k], nums[i]);
        }
    }
}

int main()
{
    srand(time(0));
    for (int i = 0; i < n; i++)
    {
        nums[i] = rand() % 10;
    }
    outOfTransposition(0);

    return 0;
}