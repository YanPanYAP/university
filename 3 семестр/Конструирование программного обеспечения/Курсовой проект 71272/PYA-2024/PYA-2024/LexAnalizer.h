#pragma once
#include"stdafx.h"
#include<iostream>
#define MaxNumberOfSeparators 13

namespace Lex {
	struct Tables {
		IT::IdTable idtable;
		LT::LexTable lextable;
	};
	Tables LexAnalizer(In::IN in, Parm::PARM parm);
	void getid(int& stroke, char* id, char** words, int m);
	bool CMP(char* str1, const char* str2);
}