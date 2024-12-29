#include"stdafx.h"
#include"out.h"
#include<iostream>
#include<fstream>
#pragma warning(disable : 4996)

using namespace std;

namespace Out
{
	OUT getOut(wchar_t outfile[])
	{
		OUT out;
		out.stream = new ofstream;
		out.stream->open(outfile);
		if (out.stream->fail())
			throw ERROR_THROW(112);
		wcscpy_s(out.outfile, outfile);
		return out;
	}
	void WriteOut(OUT out, In::IN in)
	{
		char* arr = (char*)in.text;
		*out.stream << "---- Исходные данные ----\n" << arr;
	}
	void WriteError(OUT out, Error::ERROR error)
	{
		if (out.stream)
		{
			*out.stream << "\nошибка " << error.id
				<< ": " << error.message
				<< "\nстрока " << error.inext.line << endl;
			Close(out);
		}
		else
		{
			cout << "\nошибка " << error.id
				<< ": " << error.message
				<< "\nстрока " << error.inext.line << endl;
		}
	}
	void Close(OUT out)
	{
		out.stream->close();
		delete out.stream;
	}
}