#include <iostream>
#include "FST.h"
using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    const char* chains[] = {
        "acbefg",      // Пример, который должен быть распознан
        "acbefghijk",  // Пример, при котором разбор завершается, не перебрав все символы
        "acbeffg",     // Пример, который проходит все символы, но не распознается
        "acbeg",       // Пример, не распознается из-за отсутствия символа 'f'
        "abbbbbefg",   // Пример с повторяющимся 'b', должен быть распознан
        "acccbefg",    // Пример с повторяющимся 'c', должен быть распознан
        "abefc",       // Пример с неправильным порядком символов, не распознается
        "abcef",       // Пример, где отсутствуют 'b' и 'g', не распознается
        "abefbg",       // Пример с отсутствующим 'c' или 'd', не распознается
        "abefg",
        "acccbefbg",
        "abbbcdcdbefbbbg"
    };

    int numChains = sizeof(chains) / sizeof(chains[0]);

    for (int i = 0; i < numChains; ++i) {
        FST::FST fst(
            (char*)chains[i],
            10,
            FST::NODE(1, FST::RELATION('a', 1)),
            FST::NODE(4, FST::RELATION('b', 2), FST::RELATION('c', 3), FST::RELATION('d', 4), FST::RELATION('b', 5)),
            FST::NODE(4, FST::RELATION('b', 2), FST::RELATION('c', 3), FST::RELATION('d', 4), FST::RELATION('b', 5)),
            FST::NODE(3, FST::RELATION('b', 5), FST::RELATION('c', 3), FST::RELATION('d', 4)),
            FST::NODE(3, FST::RELATION('b', 5), FST::RELATION('c', 3), FST::RELATION('d', 4)),
            FST::NODE(4, FST::RELATION('b', 5), FST::RELATION('c', 3), FST::RELATION('d', 4), FST::RELATION('e', 6)),
            FST::NODE(1, FST::RELATION('f', 7)),
            FST::NODE(2, FST::RELATION('b', 8), FST::RELATION('g', 9)),
            FST::NODE(2, FST::RELATION('b', 8), FST::RELATION('g', 9)),
            FST::NODE()
        );

        cout << "\nЦепочка " << (i + 1) << ": " << chains[i] << endl;

        if (FST::execute(fst)) {
            cout << "Цепочка распознана" << endl;
        } else {
            cout << "Цепочка не распознана" << endl;
        }
    }

    return 0;
}
