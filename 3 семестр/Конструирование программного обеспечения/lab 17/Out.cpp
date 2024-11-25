#include "stdafx.h" 
using namespace std;

namespace Out
{

	OUT getout(wchar_t outfile[])
	{
		OUT temp; 
		char* outFile = new char[wcslen(outfile) + 1];
		wcstombs_s(NULL, outFile, wcslen(outfile) + 1, outfile, wcslen(outfile) + 1); 
		temp.stream = new std::ofstream; 
		temp.stream->open(outFile);
		if (!temp.stream->is_open()) 
		{
			ERROR_THROW(113); 
		}
		wcscpy_s(temp.outfile, outfile);
		return temp; 
	};

	void WriteOut(In::IN in, wchar_t out[])
	{
		std::ofstream fout(out); 
		for (int i = 0; i < in.text.size(); i++) 
		{
			fout << in.text[i];
		}
	};

	void WriteError(OUT out, Error::ERROR error)
	{
		for (int i = 0; i < error.inext.MyVector.size(); i++) 
		{
			*out.stream << error.inext.MyVector[i]; 
		}
		*out.stream << endl; 
		*out.stream << "Ошибка" << error.id << ": " << error.message << endl << endl;
		*out.stream << "строка:" << error.inext.line << " позиция:" << error.inext.col << endl << endl;
		if (error.inext.line != -1 && error.inext.col != -1) 
		{
			*out.stream << "Ошибка в строке " << ": " << error.inext.buff << endl << endl;
		}
	}

	void Close(OUT out)
	{
		out.stream->close(); 
	}
}
