#pragma once
#include "stdafx.h"

namespace Lexer
{
	void parse(In::IN in, LT::LexTable& lextable, IT::IdTable& idtable);
	bool check(std::string l, FST::FST fst);

}
