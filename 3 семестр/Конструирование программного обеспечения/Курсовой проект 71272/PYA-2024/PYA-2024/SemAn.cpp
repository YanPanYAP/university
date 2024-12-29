#include"stdafx.h"
#include <stack> 
#pragma warning(disable: 4996)
namespace Semantic {
	IT::IDDATATYPE getLiteralType(const char* value) {
		if (value[0] == '"' && value[strlen(value) - 1] == '"') {
			return IT::STR;
		}
		else if (value[0] == '\'' && value[strlen(value) - 1] == '\'' && strlen(value) == 3) {
			return IT::CHAR;
		}
		else if (isdigit(value[0]) || (value[0] == '-' && isdigit(value[1]))) {
			return IT::INT;
		}
		else {
			return IT::UNDEF;
		}
	}

	bool semanticsCheck(Lex::Tables& tables, Log::LOG& log) {
		bool sem_ok = true;
		char temp[7];
		int correctlyCount = 0; 
		std::stack<int> stk;
		IT::Entry e;
		for (int i = 0; i < tables.idtable.size; i++)
		{
			e = tables.idtable.table[i];
			for (int j = 0; j < tables.idtable.size; j++)
			{
				if (j != i)
					if (Lex::CMP(e.id, tables.idtable.table[j].id) && tables.idtable.table[j].idtype != IT::F && Lex::CMP(e.scope, tables.idtable.table[j].scope)) {
						sem_ok = false;
						throw ERROR_THROW_IN(311, tables.lextable.table[tables.idtable.table[j].idxfirstLE].sn + 1, e.idxfirstLE + 1);
					}
			}
		}
		for (int i = 0; i < tables.lextable.size; i++)
		{
			switch (tables.lextable.table[i].lexema) {
			case LEX_LITERAL: {
				if (tables.lextable.table[i].lexema == LEX_LITERAL) {
					const char* literalValue = tables.idtable.table[tables.lextable.table[i].idxTI].value.vstr.str;
					if (literalValue[0] == '\'' && literalValue[strlen(literalValue) - 1] != '\'') {
						sem_ok = false;
						throw ERROR_THROW_IN(314, tables.lextable.table[i].sn, i);
					}
				}
				break;
			}

			case LEX_NEW: {
				if (tables.lextable.table[i + 1].lexema != LEX_TYPE) {
					sem_ok = false;
					throw ERROR_THROW_IN(301, tables.lextable.table[i].sn, -1);
				}
				break;
			}
			case LEX_TYPE: {
				if (tables.lextable.table[i + 1].lexema == LEX_ID && tables.lextable.table[i - 1].lexema != LEX_NEW && tables.idtable.table[tables.lextable.table[i + 1].idxTI].idtype != IT::P) {
					sem_ok = false;
					throw ERROR_THROW_IN(302, tables.lextable.table[i].sn, -1);
				}
				break;
			}

			case LEX_ID: {
				if (tables.lextable.table[i + 1].lexema == LEX_LEFTHESIS) {
					int idxFunc = tables.lextable.table[i].idxTI;

					if (idxFunc == TI_NULLIDX || tables.idtable.table[idxFunc].idtype != IT::F) {
						sem_ok = false;
						throw ERROR_THROW_IN(307, tables.lextable.table[i].sn, i);
					}

					int expectedParams = tables.idtable.table[idxFunc].numberOfParam;
					int actualParams = 0;
					int j = i + 2;
					while (tables.lextable.table[j].lexema != LEX_RIGHTHESIS) {
						if (tables.lextable.table[j].lexema == LEX_ID || tables.lextable.table[j].lexema == LEX_LITERAL) {
							actualParams++;
						}
						j++;
						if (j >= tables.lextable.size) {
							sem_ok = false;
							throw ERROR_THROW_IN(308, tables.lextable.table[i].sn, i);
						}
					}

					if (actualParams != expectedParams) {
						sem_ok = false;
						throw ERROR_THROW_IN(320, tables.lextable.table[i].sn, i);
					}
				}
				break;
			}
			case LEX_GREATER:
			case LEX_SMALLER:
			case LEX_GREATEROREQUAL:
			case LEX_SMALLEROREQUAL: {
				int pos = 0;
				bool flag = true;
				if (i > 1 && tables.lextable.table[i - 1].idxTI != LT_TI_NULLIDX) {
					if (tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype != IT::INT) {
						pos = tables.idtable.table[tables.lextable.table[i - 1].idxTI].idxfirstLE;
						flag = false;
					}
				}
				if (i > 1 && tables.lextable.table[i + 1].idxTI != LT_TI_NULLIDX) {
					if (tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype != IT::INT) {
						pos = tables.idtable.table[tables.lextable.table[i + 1].idxTI].idxfirstLE;
						flag = false;
					}
				}
				if (!flag) {
					throw ERROR_THROW_IN(304, tables.lextable.table[i].sn, pos);
					sem_ok = false;
				}
				break;
			}
			case LEX_EQUAL: {
				int idxLeft = tables.lextable.table[i - 1].idxTI;
				int j = i + 1;
				while (tables.lextable.table[j].lexema != LEX_SEMICOLON && j < tables.lextable.size) {
					if (tables.lextable.table[j].lexema == LEX_ID) {
						int idxRight = tables.lextable.table[j].idxTI;
					}
					else if (tables.lextable.table[j].lexema == LEX_LITERAL) {
						int idxLiteral = tables.lextable.table[j].idxTI;

						if (idxLiteral == TI_NULLIDX) {
							sem_ok = false;
							throw ERROR_THROW_IN(307, tables.lextable.table[j].sn, j);
						}

						IT::IDDATATYPE literalType = tables.idtable.table[idxLiteral].iddatatype;

						if (tables.idtable.table[idxLeft].iddatatype != literalType) {
							sem_ok = false;
							throw ERROR_THROW_IN(305, tables.lextable.table[i].sn, i);
						}
					}
					j++;
				}
				break;
			}
			case LEX_DIRSLASH: {
				if (i + 1 < tables.lextable.size) {
					int idxRight = tables.lextable.table[i + 1].idxTI;

					if (tables.lextable.table[i + 1].lexema == LEX_LITERAL) {
						const IT::Entry& literalEntry = tables.idtable.table[idxRight];
						int intValue = literalEntry.value.vint;

						if (intValue == 0) {
							sem_ok = false;
							throw ERROR_THROW_IN(306, tables.lextable.table[i].sn, i + 1);
						}
					}
					else if (tables.lextable.table[i + 1].lexema == LEX_ID) {
						const IT::Entry& idEntry = tables.idtable.table[idxRight];
						if (idEntry.iddatatype == IT::INT) {
							int value = idEntry.value.vint;

							if (value == 0) {
								sem_ok = false;
								throw ERROR_THROW_IN(306, tables.lextable.table[i].sn, i + 1);
							}
						}
						else {
							throw ERROR_THROW_IN(309, tables.lextable.table[i].sn, i + 1);
						}
					}
				}
				else {
					throw ERROR_THROW_IN(610, tables.lextable.table[i].sn, i + 1);
				}

				break;
			}
			case LEX_PLUS: {
				if (i == 0 || tables.lextable.table[i - 1].lexema == LEX_EQUAL || tables.lextable.table[i - 1].lexema == LEX_LEFTHESIS) {
					if (tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype != IT::INT) {
						sem_ok = false;
						throw ERROR_THROW_IN(304, tables.lextable.table[i].sn, i);
					}
				}
				else {
					IT::IDDATATYPE leftType = tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype;
					IT::IDDATATYPE rightType = tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype;
				}
				break;
			}

			case LEX_MINUS: {
				if (tables.lextable.table[i - 1].lexema == LEX_EQUAL || tables.lextable.table[i - 1].lexema == LEX_LEFTHESIS) {
					IT::IDDATATYPE rightType = tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype;
					if (rightType != IT::INT) {
						sem_ok = false;
						throw ERROR_THROW_IN(304, tables.lextable.table[i].sn, i);
					}
				}
				else {
					IT::IDDATATYPE leftType = tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype;
					IT::IDDATATYPE rightType = tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype;
				}
				break;
			}

			case LEX_STAR:
			case LEX_REMAINDER: {
				IT::IDDATATYPE leftType = tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype;
				IT::IDDATATYPE rightType = tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype;
				break;
			}

			case LEX_STRLEN: {
				IT::Entry e = tables.idtable.table[tables.lextable.table[i + 2].idxTI];
				int p = tables.lextable.table[i + 2].idxTI;
				LT::Entry e1 = tables.lextable.table[i];
				if (p == TI_NULLIDX) {
					throw ERROR_THROW_IN(314, e1.sn + 1, i + 1);
				}
				else if (e.iddatatype == IT::BOOL) {
					throw ERROR_THROW_IN(315, e1.sn + 1, i + 1);
				}
				else if (e.iddatatype == IT::INT) {
					throw ERROR_THROW_IN(315, e1.sn + 1, i + 1);
				}
				else if (tables.idtable.table[tables.lextable.table[i - 2].idxTI].iddatatype != IT::INT) {
					throw ERROR_THROW_IN(303, tables.lextable.table[i - 2].sn, i - 1);
				}
				break;
			}
			case LEX_RAND: {
				IT::Entry e = tables.idtable.table[tables.lextable.table[i + 2].idxTI];
				if (e.iddatatype != IT::INT)
					throw ERROR_THROW_IN(312, tables.lextable.table[i].sn + 1, i + 1);
				break;
			}
			case LEX_ABS: {
				IT::Entry e = tables.idtable.table[tables.lextable.table[i + 2].idxTI];
				if (e.iddatatype != IT::INT)
					throw ERROR_THROW_IN(312, tables.lextable.table[i].sn + 1, i + 1);
				break;
			}
			case LEX_SQRT: {
				IT::Entry e = tables.idtable.table[tables.lextable.table[i + 2].idxTI];
				if (e.iddatatype != IT::INT)
					throw ERROR_THROW_IN(312, tables.lextable.table[i].sn + 1, i + 1);
				break;
			}
			case LEX_WRONG:
			case LEX_STATE:
			case LEX_CYCLE: {
				stk.push(tables.lextable.table[i].lexema);
				break;
			}

			case LEX_CORRECTLY: {
				stk.push(tables.lextable.table[i].lexema);
				correctlyCount++;
				break;
			}

			case LEX_BRACELET: {
				if (correctlyCount > 0 && stk.top() == 99) {
					correctlyCount--;
					stk.pop();
				}
				break;
			}

			case LEX_DOL: {
				if (stk.empty() || (stk.top() != 63 && stk.top() != 99 && stk.top() != 119 && stk.top() != 118)) {
					sem_ok = false;
					throw ERROR_THROW_IN(322, tables.lextable.table[i].sn, i);
				}
				stk.pop();
				break;
			}
			case LEX_RETURN: {
				if (!stk.empty()) {
					sem_ok = false;
					throw ERROR_THROW_IN(322, tables.lextable.table[i].sn, i);
				}
				break;
			}
			}
			
		}
		if (!stk.empty()) {
			while (!stk.empty()) {
				int topElement = stk.top();
				stk.pop();

				switch (topElement) {
				case LEX_STATE:
				case LEX_CYCLE:
				case LEX_WRONG:
					sem_ok = false;
					throw ERROR_THROW_IN(322, tables.lextable.table[tables.lextable.size - 1].sn, tables.lextable.size - 1);
					break;

				case LEX_CORRECTLY:
					sem_ok = false;
					throw ERROR_THROW_IN(323, tables.lextable.table[tables.lextable.size - 1].sn, tables.lextable.size - 1);
					break;

				default:
					sem_ok = false;
					throw ERROR_THROW_IN(324, tables.lextable.table[tables.lextable.size - 1].sn, tables.lextable.size - 1);
					break;
				}
			}
		}
		return sem_ok;
	}
}