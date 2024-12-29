#include <iostream>
#include <cmath>
#include <ctime>

extern "C" {
    int _stdcall Rand(int range = 1000) {
        static bool isSeeded = false;
        if (!isSeeded) {
            srand((unsigned)time(NULL));
            isSeeded = true;
        }
        return rand() % range;
    }

    int _stdcall Input() {
        int i;
        std::cin >> i;
        return i;
    }

    int _stdcall Writestroke(char* str) {
        setlocale(LC_CTYPE, "ru");
        if (str == NULL || str[0] == '\0') {
            return 0;
        }
        std::cout << str;
        return 0;
    }

    int _stdcall Writelinestroke(char* str) {
        setlocale(LC_CTYPE, "ru");
        if (str == NULL || str[0] == '\0') {
            return 0;
        }
        std::cout << str << std::endl;
        return 0;
    }

    int _stdcall Strlen(char* str) {
        setlocale(LC_CTYPE, "ru");
        return (str == NULL) ? 0 : strlen(str);
    }

    int _stdcall WriteNumb(int numb) {
        setlocale(LC_CTYPE, "ru");
        std::cout << numb;
        return 0;
    }

    int _stdcall WritelineNumb(int numb) {
        setlocale(LC_CTYPE, "ru");
        std::cout << numb << std::endl;
        return 0;
    }

    int _stdcall Remainder(int dividend, int divisor) {
        if (divisor == 0) {
            return 0;
        }
        return dividend % divisor;
    }

    int _stdcall Abs(int a) {
        return (a >= 0) ? a : -a;
    }

    int _stdcall Sqrt(int a) {
        if (a < 0) {
            return -1.0;
        }
        return sqrt(a);
    }
}
