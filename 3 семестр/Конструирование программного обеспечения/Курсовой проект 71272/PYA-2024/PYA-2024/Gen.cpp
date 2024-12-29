#include"Gen.h"
#include<string>
#include<string.h>
static int counterOfStates = 0;
static bool booleanLit = true;
void header(Lex::Tables& tables, vector<string>& v) {
	v.push_back(BEGIN);
	v.push_back(EXTRN);
	vector<string> vConst;
	vector<string> vData;
	vConst.push_back(CONST);
	vData.push_back(DATA);
	vData.push_back("\t\temptyString byte 0");

	for (int i = 0; i < tables.idtable.size; i++) {
		IT::Entry temp = tables.idtable.table[i];
		string str = "\t\t" + string(temp.id);

		if (temp.idtype == IT::IDTYPE::L) {
			switch (temp.iddatatype) {
			case IT::IDDATATYPE::INT:
				str += " SDWORD " + to_string(temp.value.vint);
				break;
			case IT::IDDATATYPE::STR:
				str += " byte \"" + string(temp.value.vstr.str) + "\", 0";
				break;
			case IT::IDDATATYPE::CHAR:
				str += " byte '" + string(temp.value.vstr.str) + "', 0";
				break;
			case IT::IDDATATYPE::BOOL:
				if (Lex::CMP(temp.value.vstr.str, "false")) {
					if (booleanLit) {
						vConst.push_back("\t\tTRUE equ 1");
						vConst.push_back("\t\tFALSE equ 0");
						booleanLit = false;
					}
					str += " word FALSE";
				}
				else if (Lex::CMP(temp.value.vstr.str, "true")) {
					if (booleanLit) {
						vConst.push_back("\t\tTRUE equ 1");
						vConst.push_back("\t\tFALSE equ 0");
						booleanLit = false;
					}
					str += " word TRUE";
				}
				break;
			}
			vConst.push_back(str);
		}
		else if (temp.idtype == IT::IDTYPE::V) {
			str = "\t\t" + string(temp.scope) + string(temp.id);
			switch (temp.iddatatype) {
			case IT::IDDATATYPE::INT:
				str += " sdword 0";
				break;
			case IT::IDDATATYPE::STR:
			case IT::IDDATATYPE::CHAR:
				str += " dword offset emptyString";
				break;
			case IT::IDDATATYPE::BOOL:
				str += " word FALSE";
				break;
			}
			vData.push_back(str);
		}
	}

	v.insert(v.end(), vConst.begin(), vConst.end());
	v.insert(v.end(), vData.begin(), vData.end());
	v.push_back(CODE);
}



string footerOfFunc(Lex::Tables& tables, int i, string funcName, int numOfPar)
{
	string temp = "\npop ebx\npop edx\n";
	if (LEXEMA(i + 1) != LEX_SEMICOLON)
		temp = temp + "mov eax, " + string(IT_ENTRY(i + 1).scope) + string(IT_ENTRY(i + 1).id) + "\n";
	temp += "ret\n";
	temp += funcName + " ENDP" + STR_SEPARATOR;
	return temp;
}

string headerOfFunc(Lex::Tables& tables, int i, string name, int param) {
	string temp = "";
	IT::Entry e = IT_ENTRY(i);
	IT::IDDATATYPE type = e.iddatatype;

	temp = temp + FUNCTION_NAME(name) + string(e.id) + string(" PROC,\n\t");
	int number = e.numberOfParam;
	int j = i + 3;
	while (number > 0) {
		temp = temp + string(IT_ENTRY(j).scope) + string(IT_ENTRY(j).id) + (IT_ENTRY(j).iddatatype == IT::INT ? " : sdword, " : " : dword, ");
		number--;
		j += 3;
	}
	int f = temp.rfind(',');
	if (f > 0)
		temp[f] = ' ';
	temp += "\npush ebx\npush edx\n";
	return temp;
}

string callFunc(Lex::Tables& tables, int pos) {
	string str = "";
	IT::Entry e = IT_ENTRY(pos + 3);
	stack<IT::Entry> st;
	int posOfFunc;
	for (int j = pos; LEXEMA(j) != '@'; j++) {
		if (LEXEMA(j) == LEX_ID || LEXEMA(j) == LEX_LITERAL)
			st.push(IT_ENTRY(j));
		posOfFunc = j;
	}
	while (!st.empty()) {
		if (st.top().iddatatype == (IT::IDDATATYPE::STR || IT::IDDATATYPE::CHAR) && st.top().idtype == IT::L) {
			str = str + "push offset " + st.top().id + "\n";
		}
		else {
			str = str + "push " + "main" + st.top().id + "\n";
		}

		st.pop();
	}
	str = str + "call " + string(e.id) + IN_COD_ENDL;
	return str;
}

string genStateCode(Lex::Tables& tables, int i, string& cyclecode) {
	string str;
	bool inCycle = false;
	if (!cyclecode.empty()) {
		str = str + cyclecode;
		inCycle = true;
	}
	if (!inCycle) {
		counterOfStates++;
		cyclecode.clear();
		IT::Entry left = IT_ENTRY(i + 2);
		IT::Entry right = IT_ENTRY(i + 4);
		bool correctly = false, wrong = false, cycle = false;
		string correctlyStroke, WrongStroke;
		for (int j = i + 6; LEXEMA(j) != LEX_DOL; j++)
		{
			if (LEXEMA(j) == LEX_CORRECTLY)
				correctly = true;
			if (LEXEMA(j) == LEX_WRONG || LEXEMA(j) == LEX_STATE)
				wrong = true;
			if (LEXEMA(j) == LEX_CYCLE)
				cycle = true;
		}
		bool b = false;
		if (left.iddatatype == IT::BOOL && right.iddatatype == IT::BOOL) {
			str = str + "push " + "main" + left.id + "\n";
			str = str + "pop bx\n";
			b = true;
		}
		if (!b && left.idtype == IT::L && right.idtype == IT::L)
			str = str + "mov edx, " + left.id + "\ncmp edx, " + right.id + "\n";
		else if (!b && left.idtype != IT::L && right.idtype == IT::L)
			str = str + "mov edx, " + left.scope + left.id + "\ncmp edx, " + right.id + "\n";
		else if (!b && right.idtype != IT::L && right.idtype == IT::L)
			str = str + "mov edx, " + left.id + "\ncmp edx, " + right.scope + right.id + "\n";
		else {
			if (!b)
				str = str + "mov edx, " + left.scope + left.id + "\ncmp edx, " + right.scope + right.id + "\n";
		}
		switch (LEXEMA(i + 3))
		{
		case LEX_GREATER: {
			correctlyStroke = "jg";
			WrongStroke = "jl";
			break;
		}
		case LEX_SMALLER: {
			correctlyStroke = "jl";
			WrongStroke = "jg";
			break;
		}
		case LEX_GREATEROREQUAL: {
			correctlyStroke = "jae";
			WrongStroke = "jbe";
			break;
		}
		case LEX_SMALLEROREQUAL: {
			correctlyStroke = "jbe";
			WrongStroke = "jae";
			break;
		}
		case LEX_OR: {
			str = str + "or bx, cx\n\n";
			str = str + "cmp bx, TRUE\n";
			correctlyStroke = "jz";
			WrongStroke = "jnz";
			break;
		}
		case LEX_AND: {
			str = str + "and bx, cx\n\n";
			str = str + "cmp bx, TRUE\n";
			correctlyStroke = "jz";
			WrongStroke = "jnz";
			break;
		}
		}
		if (correctly) {
			str = str + "\n" + correctlyStroke + " right" + to_string(counterOfStates);
		}
		if (wrong) {
			str = str + "\n" + WrongStroke + " wrong" + to_string(counterOfStates);
		}
		if (cycle && !inCycle) {
			str = str + "\n" + correctlyStroke + " repeat" + to_string(counterOfStates);
			cyclecode = str;
			str = str + "\njmp repeatnext" + to_string(counterOfStates);
			str = str + "\nrepeat" + to_string(counterOfStates) + ":\n";
		}
		if (inCycle) {
			str = str + "\n" + correctlyStroke + " repeat" + to_string(counterOfStates);
			str = str + "\nrepeatnext" + to_string(counterOfStates) + ":";
			inCycle = false;
		}

		else if (!correctly && !cycle || !wrong && !cycle)  str = str + "\njmp next" + to_string(counterOfStates);
	}
	else {
		str = str + "\nrepeatnext" + to_string(counterOfStates) + ":";
	}


	return str;
}


string generateEqual(Lex::Tables& tables, int pos) {
	string str;
	IT::Entry e = IT_ENTRY(pos - 1);

	switch (e.iddatatype) {
	case IT::IDDATATYPE::INT:
		str += "pop eax\n";
		str += "mov " + string(e.scope) + e.id + ", eax\n";
		break;
	case IT::IDDATATYPE::STR:
	case IT::IDDATATYPE::CHAR:
		str += "mov " + string(e.scope) + e.id + ", offset " + IT_ENTRY(pos + 1).id + "\n";
		break;
	case IT::IDDATATYPE::BOOL:
		str += "mov cx, " + string(e.scope) + e.id + "\n";
		break;
	}

	return str;
}


namespace Generator {
	void CodeGeneration(Lex::Tables& tables) {
		ofstream asmFile("C:\\PYA-2024\\Generated\\Generated.asm");
		vector<string> v;
		stack<int>posCorrectly;
		stack<int>posWrong;
		header(tables, v);
		string cyclecode;
		string buff = "";
		string buffForFuncName = "";
		int nesting = 0;
		int count = 0;
		int numberOfParam;
		bool findmain = false;
		for (int i = 0; i < tables.lextable.size; i++) {
			switch (LEXEMA(i))
			{
			case LEX_MAIN: {
				buff = buff + FUNCTION_NAME("MAIN") + "main PROC";
				buffForFuncName = "main";
				numberOfParam = 0;
				findmain = true;
				break;
			}
			case LEX_FUNCTION:
			{
				buffForFuncName = IT_ENTRY(i + 1).id;
				numberOfParam = tables.idtable.table[tables.lextable.table[i + 1].idxTI].numberOfParam;
				buff = headerOfFunc(tables, i + 1, buffForFuncName, numberOfParam);
				break;
			}
			case LEX_STATE: {
				buff = genStateCode(tables, i, cyclecode);
				break;
			}
			case LEX_CORRECTLY: {
				buff = buff + "\tright" + to_string(counterOfStates) + ":";
				posCorrectly.push(i);
				break;
			}
			case LEX_WRONG: {
				buff = buff + "\twrong" + to_string(counterOfStates - nesting) + ":";
				posWrong.push(i);
				if (posWrong.size() != posCorrectly.size()) {
					nesting++;
				}

				break;
			}
			case LEX_BRACELET: {
				if (LEXEMA(i + 1) == LEX_CORRECTLY || LEXEMA(i + 1) == LEX_WRONG) {
					buff = buff + "jmp next" + to_string(counterOfStates - nesting);

				}
				break;
			}
			case LEX_RETURN: {
				if (!findmain) {
					buff = footerOfFunc(tables, i, buffForFuncName, numberOfParam);
					break;
				}
				else {
					buff = buff + STR_SEPARATOR;
					break;
				}
			}
			case LEX_EQUAL: {
				buff = generateEqual(tables, i);
				while (LEXEMA(++i) != LEX_SEMICOLON);
				break;
			}
			case LEX_WRITELINE: {
				IT::Entry e = IT_ENTRY(i + 1);
				if (findmain) {
					char* t = (char*)data(buffForFuncName);
					char buff[10];
					for (int j = 0; j < tables.idtable.size; j++) {
						if (Lex::CMP(IT_ENTRY_IN_TABLE(j).scope, t) && Lex::CMP(e.id, IT_ENTRY_IN_TABLE(j).id)) {
							e = IT_ENTRY_IN_TABLE(j);
							break;
						}
					}
				}
				switch (e.iddatatype)
				{
				case IT::IDDATATYPE::INT: {
					if (e.idtype != IT::L)
						buff = buff + "\npush " + e.scope + e.id + "\ncall WritelineNumb\n";
					else
						buff = buff + "\npush " + e.id + "\ncall WritelineNumb\n";
					break;
				}
				case IT::IDDATATYPE::CHAR:
				case IT::IDDATATYPE::STR: {
					if (e.idtype == IT::IDTYPE::L)  buff = buff + "\npush offset " + e.id + "\ncall WritelineStroke\n";
					else  buff = buff + "\npush " + e.scope + e.id + "\ncall WritelineStroke\n";
					break;
				}
				}
				buff = buff + "\n";
				break;
			}
			case LEX_WRITE: {
				IT::Entry e = IT_ENTRY(i + 1);
				if (findmain) {
					char* t = (char*)data(buffForFuncName);
					char buff[10];
					for (int j = 0; j < tables.idtable.size; j++) {
						if (Lex::CMP(IT_ENTRY_IN_TABLE(j).scope, t) && Lex::CMP(e.id, IT_ENTRY_IN_TABLE(j).id)) {
							e = IT_ENTRY_IN_TABLE(j);
							break;
						}
					}
				}
				switch (e.iddatatype)
				{
				case IT::IDDATATYPE::INT: {
					if (e.idtype != IT::L)
						buff = buff + "\npush " + e.scope + e.id + "\ncall WriteNumb\n";
					else
						buff = buff + "\npush " + e.id + "\ncall WriteNumb\n";
					break;
				}
				case IT::IDDATATYPE::CHAR:
				case IT::IDDATATYPE::STR: {
					if (e.idtype == IT::IDTYPE::L)  buff = buff + "\npush offset " + e.id + "\ncall WriteStroke\n";
					else  buff = buff + "\npush " + e.scope + e.id + "\ncall WriteStroke\n";
					break;
				}
				}
				buff = buff + "\n";
				break;
			}


			case LEX_DOL: {
				if (LEXEMA(i - 1) == LEX_BRACELET) {
					if (!cyclecode.empty()) {
						buff = genStateCode(tables, i, cyclecode);
						break;
					}
					if (posCorrectly.size() == posWrong.size()) {
						buff += "next" + to_string(counterOfStates - nesting) + ":";
						nesting = 0;
					}
					else if (posCorrectly.size() != posWrong.size()) {
						buff += "next" + to_string(counterOfStates) + ":";
					}
					break;
				}
			}
			}
			if (!buff.empty()) {
				v.push_back(buff);
			}
			buff.clear();
		}
		v.push_back(END);
		for (string x : v)
			asmFile << x << endl;
	}
}