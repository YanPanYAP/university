#include <iostream>
#include <fstream>
using namespace std;

void generate_combinations(int length, int max_digit, int* current_combination, int index, ofstream& file) {

    if (index == length)
    {
        for (int i = 0; i < length; i++)
        {
            file << current_combination[i];
            cout << current_combination[i];
        }
        file << '\n';
        cout << '\n';
        return;
    }

    for (int i = 0; i <= max_digit; i++)
    {
        current_combination[index] = i;
        generate_combinations(length, max_digit, current_combination, index + 1, file);
    }
}

int main()
{
    setlocale(LC_CTYPE, "rus");
    int A;
    cout << "¬ведите A: ";

    cin >> A;

    ofstream file("data.txt", ios_base::trunc);

    int* current_combination = new int[A];

    generate_combinations(A, A - 1, current_combination, 0, file);

    file.close();
    delete[] current_combination;

    return 0;
}