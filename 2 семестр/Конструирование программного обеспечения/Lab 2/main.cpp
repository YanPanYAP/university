#include <iostream>
#include <Windows.h>
using namespace std;

int sum(int x, int y)
{
    return x + y;
}

int(*f) (int, int);

int main()
{
    setlocale(LC_ALL, "RU");

    int n = 11;
    int X = 9 + n;
    int Y = 10 + n;
    int Z = 11 + n;
    float S = 1.0 + n;

    bool t4_1 = false; //00 0x0000005B82EFECF4
    bool t4_2 = true; //01 0x0000005B82EFECF4

    char t5 = 'a'; //ASCII 61 0x0000005B82EFECF4

    char t6 = 'а'; //ASCII e0 0x0000005B82EFECF4

    wchar_t t7 = 'P'; //00 50 Unicod (англ) 0x0000005B82EFECF4

    wchar_t t8 = 'П'; //ff cf Unicod (рус) 0x0000005B82EFECF4

    short t9_1 = X; //0x0014  00 14 0x0000005B82EFED8A
    short t9_2 = -X; //0xffec  ff ec 0x0000003EE57CEE04

    short t10_1 = 0x7fff; //0x7fff = 32767  
    short t10_2 = 0x8000; //0x8000 = -32768  

    unsigned short t11_1 = 0xffff; // 65535
    unsigned short t11_2 = 0x0000; //0

    int t12_1 = Y; //0x00000015  00 00 00 15 0x0000005B82EFEE20
    int t12_2 = -Y; //0xffffffeb  ff ff ff eb 0x0000005B82EFEE20

    int t13_1 = 0X7FFFFFFF; //2147483647
    int t13_2 = 0X80000000; //-2147483648

    unsigned int t14_1 = 0xffffffff; //4294967295
    unsigned int t14_2 = 0x00000000; //0 

    long t15_1 = Z; //0x00000016  00 00 00 16 0x0000005B82EFEE20
    long t15_2 = -Z; //0xffffffea  ff ff ff ea 0x00000083802FEFE4

    long t16_1 = 0x7fffffff; //2147483647
    long t16_2 = 0x80000000; //-2147483648

    unsigned long t17_1 = 0xffffffff; //4294967295
    unsigned long t17_2 = 0x00000000; //0

    float t18_1 = S; //12.0000000  41 40 00 00 0x0000005B82EFEFE2
    float t18_2 = -S; //-12.0000000  c1 40 00 00 0x0000005B82EFEFE2

    float t19_1 = t18_1 / 0; // положительная бесконечность
    float t19_2 = t18_2 / 0; //отрицательная бесконечность
    float t19_3 = sqrt(-2.0f); // Отрицательная неопределённость

    //Переменные
    char t20_1 = 'P'; //0x50 (англ)
    wchar_t t20_2 = 'П'; //0xffcf (рус)
    short t20_3 = 26; //0x001a
    int t20_4 = 260; //0x00000104
    float t20_5 = 26.5; //0x41d40000
    double t20_6 = 26.0005; //0x403a0020c49ba5e3

    //Указатели
    char* p20_1 = &t20_1; //0x0000005b82eff134
    wchar_t* p20_2 = &t20_2; //0x0000005b82eff154
    short* p20_3 = &t20_3; //0x00005b82eff174
    int* p20_4 = &t20_4; //0x0000005b82eff194
    float* p20_5 = &t20_5; //0x0000005b82eff1b4
    double* p20_6 = &t20_6; //0x0000005b82eff1d8

    //Увеличение на 3
    char* p20_1_1 = p20_1 + 3; //0x0000005b82eff137
    wchar_t* p20_2_2 = p20_2 + 3; //0x0000005b82eff15a
    short* p20_3_3 = p20_3 + 3; //0x00005b82eff17a
    int* p20_4_4 = p20_4 + 3; //0x0000005b82eff1a0
    float* p20_5_5 = p20_5 + 3; //0x0000005b82eff1c0
    double* p20_6_6 = p20_6 + 3; //0x0000005b82eff1f0

    f = sum;
    int t21 = f(1, 8); //0x00000009
    cout << t21;

    return 0;
}