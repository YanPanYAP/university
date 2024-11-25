#include "stdafx.h"
#include <fstream>
namespace LT
{
	LT::LexTable::LexTable()
	{
		this->maxsize = LT_MAXSIZE;
		this->size = 0;
		this->table = new Entry[LT_MAXSIZE];
	}

	void Add(LexTable& lextable, Entry entry)
	{
		if (lextable.size < lextable.maxsize)
			lextable.table[lextable.size++] = entry;
		else
			throw ERROR_THROW(120);
	}

	Entry LexTable::GetEntry(int n)
	{
		if (n < this->maxsize && n >= 0)
			return this->table[n];
	}

	void Delete(LexTable& lextable)
	{
		delete[] lextable.table;
		lextable.table = nullptr;
	}

	void LexTable::PrintLexTable(const wchar_t* in)
	{
		cout << "--------- “аблица лексем ---------";

		int num_string = 0;		// Ќомер текущей строки
		for (int i = 0; i < this->size;)
		{
			if (num_string == this->table[i].sn)	// ≈сли номер строки совпадает с номером строки лексемы
				cout << this->table[i++].lexema;	// ¬ыводит лексему и переходит к следующей записи
			else
			{
				num_string++;
				if (this->table[i].sn != num_string)
				{
					continue;
				}
				cout << '\n' << num_string << ".\t";

				if (num_string == this->table[i].sn)
					cout << this->table[i++].lexema;
			}
		}
	}

	LT::Entry::Entry()
	{
		this->lexema = '\0';
		this->sn = LT_TI_NULLXDX;	//номер строки
		this->idxTI = LT_TI_NULLXDX;
	}

	LT::Entry::Entry(const char lex, int str_n, int idxTI)
	{
		this->lexema = lex;
		this->sn = str_n;
		this->idxTI = idxTI;
	}
}