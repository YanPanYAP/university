#include "\\Mac\Home\Documents\fit\kpo\kpo6\kpo6L\Dictionary.h"

using namespace std;
using namespace Dictionary;

#define TEST_DICTIONARY
//#define TEST_CREATE_01
//#define TEST_CREATE_02
//#define TEST_ADDENTRY_03
//#define TEST_ADDENTRY_04 
//#define TEST_GETENTRY_05
//#define TEST_GETENTRY_06
//#define TEST_UPDENTRY_07
//#define TEST_UPDENTRY_08
//#define TEST9

#if defined(TEST_CREATE_01)+defined(TEST_CREATE_02)+defined(TEST_ADDENTRY_03)+defined(TEST_ADDENTRY_04)+defined(TEST_GETENTRY_05)+defined(TEST_GETENTRY_06)+defined(TEST_UPDENTRY_07)+defined(TEST_UPDENTRY_08)+defined(TEST_DICTIONARY)>1
#error "���������� ����� ������ ������� �����"
#endif

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	try
	{
#ifdef TEST_CREATE_01 //#ifndef � ������������ �������� ���������� ��� ��������������� ������������� ���������; 
		Instance test = Create((char*)"AAAAAAAAAAAAAAAAAAAAAAAA", 5);
#endif // TEST_CREATE_1
		//#endif � ����� ����� �������� ����������
#ifdef TEST_CREATE_02
		Instance test = Create((char*)"TEST", 5);
#endif // TEST_CREATE_2

#ifdef TEST_ADDENTRY_03
		Instance test = Create((char*)"TEST", 0);
		Entry test_e = { 1,"aaaaaa" };
		AddEntry(test, test_e);
#endif // TEST_ADDENTRY_1

#ifdef TEST_ADDENTRY_04 
		Instance test = Create((char*)"TEST", 5);
		Entry test_e = { 1,"aaaaaa" };
		AddEntry(test, test_e);
		AddEntry(test, test_e);
#endif // TEST_ADDENTRY_2

#ifdef TEST_GETENTRY_05
		Instance test = Create((char*)"TEST", 5);
		Entry test_e = { 1,"aaaaaaaaa" };
		AddEntry(test, test_e);
		GetEntry(test, 2);
#endif // TEST_GETENTRY_1

#ifdef TEST_GETENTRY_06
		Instance test = Create((char*)"TEST", 5);
		Entry test_e = { 1,"aaaaaaaaaa" };
		AddEntry(test, test_e);
		DelEntry(test, 2);
#endif // TEST_DELENTRY_1

#ifdef TEST_UPDENTRY_07
		Instance test7 = Create((char*)"TEST", 5);
		Entry test_e7 = { 1,"aaaaaaaaa" }, test_upd_e7 = { 2,"zzzzzzzz" };
		AddEntry(test7, test_e7);
		UpdEntry(test7, 3, test_upd_e7);
#endif // TEST_UPDENTRY_1

#ifdef TEST_UPDENTRY_08
		Instance test = Create((char*)"TEST", 5);
		Entry test_e = { 1,"aaaaaaa" }, test_upd_e = { 1,"zzzzzzzz" };
		AddEntry(test, test_e);
		UpdEntry(test, 1, test_upd_e);
#endif // TEST_UPDENTRY_1

#ifdef TEST_DICTIONARY //TEST9
		Instance d1 = Create((char*)"�������������", 5);//�������� ������� 
		Entry	e1 = { 1,"�������" }, e2 = { 2,"�������" }, e3 = { 3,"������" },
			e4 = { 4, "���������" }, e5 = { 5, "�����" };//�������� ������� 
		AddEntry(d1, e1);// ���������� �������� � ������� 
		AddEntry(d1, e2);// ���������� �������� � ������� 
		AddEntry(d1, e3);// ���������� �������� � ������� 
		AddEntry(d1, e4);// ���������� �������� � ������� 
		Entry getE = GetEntry(d1, 4);//����� ������� � ������� �� ��������������� 
		DelEntry(d1, 2);//������� ������� �� ������� �� ��������������� 
		Entry newentry1 = { 6, "�����" };//������� ������� 
		UpdEntry(d1, 3, newentry1);//�������� ������� ������� �� �������������� 
		Print(d1);//����������� �������� ������� 
		Instance d2 = Create((char*)"���l����", 5);
		Entry s1 = { 1,"�������" }, s2 = { 2,"�������" }, s3 = { 4,"�������" };
		AddEntry(d2, s1);
		AddEntry(d2, s2);
		AddEntry(d2, s3);
		Entry newentry3 = { 3, "�������" };
		UpdEntry(d2, 4, newentry3);
		Print(d2);
		Delete(d1);
		Delete(d2);
#endif 
	}
	catch (char* e) { //��������� ���������� ������� 
		cout << e << endl;
	}
	system("pause");
	return 0;
}