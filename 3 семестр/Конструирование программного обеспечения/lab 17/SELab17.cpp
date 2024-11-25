#pragma comment(linker, "/STACK:16777216")
#include "stdafx.h"
#include <iostream>
#include "Error.h"		
#include "Parm.h"		
#include <cwchar>
#include <time.h>
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, ".1251");
	Out::OUT out = Out::INITOUT;
	Log::LOG log = Log::INITLOG;
	In::IN in;
	try
	{
		LT::LexTable lextab = LT::Create(LT_MAXSIZE - 1);
		IT::IdTable idtab = IT::Create(TI_MAXSIZE - 1);
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		Log::WriteLine(log, (char*)"Текст:", (char*)"без ошибки \n", "");
		Log::WriteLine(log, (wchar_t*)L"Текст", (wchar_t*)L" без ошибок \n", L"");
		out = Out::getout(parm.out);
		Log::WriteParm(log, parm);
		Log::WriteLog(log);
		In::IN in = In::getin(parm.in);

		Log::WriteIn(log, in);
		Out::WriteOut(in, parm.out);

		// Вывод всего текста
		for (size_t i = 0; i < in.text.size(); i++)
		{
			cout << in.text[i] << endl;
		}

		cout << "Всего символов:" << in.size << endl;
		cout << "Всего строк:" << in.lines << endl;

		Lexer::parse(in, lextab, idtab);
		Log::WriteLexTable(log, lextab);
		Log::Close(log);
		Out::Close(out);



	}
	catch (Error::ERROR e)
	{
		cout << "Ошибка" << e.id << ": " << e.message << endl << endl;
		cout << "строка:" << e.inext.line << " позиция:" << e.inext.col << endl << endl;
		Log::WriteError(log, e);
		Out::WriteError(out, e);
		if (e.inext.line != -1 && e.inext.col != -1)
		{
			cout << "Ошибка в строке " << ": " << e.inext.buff << endl << endl;

		}

	}

	system("pause");

	return 0;

}
