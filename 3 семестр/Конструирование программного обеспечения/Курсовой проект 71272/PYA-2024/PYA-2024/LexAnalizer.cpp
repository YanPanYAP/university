#include"stdafx.h"
#pragma warning(disable: 4996)

namespace Lex {
	char* buff = NULL;
	char* NameOfFunc = NULL;
	char* id = new char[TI_MAXSIZE];
	bool Flag = false;
	bool findFunc = false;
	int LitNumber = 0;
	int numberOfPar = 0;
	int posOfFunction;
	bool findState = false;
	bool isMainDetected = false;
	bool CMP(char* str1, const char* str2) {
		int i = 0;
		while (str1[i] != '\0' && str2[i] != '\0') {
			if (str1[i] != str2[i]) {
				return false;
			}
			i++;
			if (str1[i] == '\0' && str2[i] != '\0' || str2[i] == '\0' && str1[i] != '\0') {
				return false;
			}

		}
		if (str1[i] == '\0' && str2[i] != '\0' || str2[i] == '\0' && str1[i] != '\0') {
			return false;
		}
		return true;
	}
	char* changeID(char* identificator) {
		if (strlen(identificator) > ID_MAXSIZE) {
			identificator[ID_MAXSIZE] = '\0';
		}
		return identificator;
	}


	void getid(int& stroke, char* id, char** words, int m) {
		int size = strlen(words[stroke]);
		if (size + /*counter*/ +1 > ID_MAXSIZE)
			throw ERROR_THROW(162);
		strcpy(id, words[stroke]);
	}

	Tables LexAnalizer(In::IN in, Parm::PARM parm) {
		Tables tables;
		LT::LexTable lextable = LT::Create(in.size);
		IT::IdTable idtable = IT::Create(in.size);
		LT::Entry LtEntr;
		IT::Entry ItEntr;
		stack<bool> st;
		bool FindMain = false;
		int countOfMain = 0;

		char** words = new char* [MAX_NUMBER_OF_WORDS];
		for (int i = 0; i < MAX_NUMBER_OF_WORDS; i++)
			words[i] = new char[MAX_LENGTH_OF_WORD] {NULL};
		words = DivideTexT((char*)in.text, in.size);
		int stroke = -1;
		int sn = 0;
		int realColumn = 1;
		int lineNumber = 0;
		bool newLineFlag = false;
		int tokenLength = 0;
		while (words[stroke] != NULL) {
			stroke++;
			if (words[stroke] == NULL) {
				break;
			}

			char* currentWord = words[stroke];
			int tokenLength = strlen(currentWord);
			for (int i = 0; i < tokenLength; i++) {
				char currentChar = currentWord[i];
				if (currentChar == '\n') {
					lineNumber++;
					realColumn = 1;
					newLineFlag = true;
				}
			}
			if (!newLineFlag) {
				realColumn++;
			}
			newLineFlag = false;
			//std::cout << realColumn << ' ';

			FST::FST fstState(words[stroke], FST_STATE);
			if (FST::execute(fstState)) {
				LtEntr.lexema = LEX_STATE;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				findState = true;
				st.push(findState);
				continue;
			}
			FST::FST fstand(words[stroke], FST_AND);
			if (FST::execute(fstand)) {
				LtEntr.lexema = LEX_AND;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}
			FST::FST fstor(words[stroke], FST_OR);
			if (FST::execute(fstor)) {
				LtEntr.lexema = LEX_OR;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}
			FST::FST fstinver(words[stroke], FST_INVERSION);
			if (FST::execute(fstinver)) {
				LtEntr.lexema = LEX_INVERSION;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}
			FST::FST fstCycle(words[stroke], FST_CYCLE);
			if (FST::execute(fstCycle)) {
				LtEntr.lexema = LEX_CYCLE;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}

			FST::FST fstcol(words[stroke], FST_COLON);
			if (FST::execute(fstcol)) {
				LtEntr.lexema = LEX_COLON;
				LtEntr.sn = sn;
				LtEntr.idxTI = TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}
			FST::FST fstG1(words[stroke], FST_GREATER);
			if (FST::execute(fstG1)) {
				LtEntr.lexema = LEX_GREATER;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}

			FST::FST fstS1(words[stroke], FST_SMALLER);
			if (FST::execute(fstS1)) {
				LtEntr.lexema = LEX_SMALLER;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}
			FST::FST fstD(words[stroke], FST_DOL);
			if (FST::execute(fstD)) {
				LtEntr.lexema = LEX_DOL;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				if ((CMP(words[stroke - 2], "$") || CMP(words[stroke - 3], "$") || CMP(words[stroke - 2], "]") && !st.empty() || CMP(words[stroke - 1], "]") && !st.empty())) {
					st.pop();
				}
				continue;
			}
			FST::FST fstCor(words[stroke], FST_CORRECTLY);
			if (FST::execute(fstCor) && !st.empty() && st.top() == true) {
				LtEntr.lexema = LEX_CORRECTLY;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}
			FST::FST fstWr(words[stroke], FST_WRONG);
			if (FST::execute(fstWr) && !st.empty() && st.top() == true) {
				LtEntr.lexema = LEX_WRONG;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}
			FST::FST FSTTR1(words[stroke], FST_TRUE);
			FST::FST FSTFAL1(words[stroke], FST_FALSE);
			FST::FST LSR1(words[stroke], FST_STRLIT);
			FST::FST LIT1(words[stroke], FST_INTLIT);
			FST::FST LCHR1(words[stroke], FST_SYMBLIT);
			if ((FST::execute(LSR1) || FST::execute(LIT1) || FST::execute(LCHR1) || FST::execute(FSTTR1) || FST::execute(FSTFAL1)) && !st.empty() && st.top() == true) {
				bool here = false;
				int pos;
				bool toInt = false;
				for (int i = 0; i < strlen(words[stroke]); i++) {
					if (int(words[stroke][i]) < 48 || int(words[stroke][i]) > 58) {
						toInt = false;
						break;
					}
					toInt = true;
				}
				for (int i = 0; i < idtable.size; i++)
				{
					if (CMP(idtable.table[i].value.vstr.str, words[stroke]) || idtable.table[i].idtype == IT::L && idtable.table[i].iddatatype == IT::INT && toInt && stoi(words[stroke]) == idtable.table[i].value.vint) {	//ѕроверка на наличие в таблице идентификаторов
						here = true;
						pos = i;
						break;
					}
				}
				if (!here) {
					char tem[99] = "-";
					bool minus = false;
					if (words[stroke][0] == '-') {
						stroke++;
						char buff[99];
						strcpy_s(buff, words[stroke]);
						strcat_s(tem, buff);
						minus = true;
					}
					LtEntr.lexema = LEX_LITERAL;
					LtEntr.sn = sn;
					LtEntr.idxTI = idtable.size;
					LT::Add(lextable, LtEntr);
					int symbols = strlen(words[stroke]);
					if (symbols > TI_STR_MAXSIZE)
						throw ERROR_THROW_IN(164, sn + 1, stroke + 1);

					ItEntr.idtype = IT::L;
					if (words[stroke][0] == '\'') {
						if (ItEntr.iddatatype = IT::STR) {
							strcpy_s(ItEntr.value.vstr.str, words[stroke]);
							ItEntr.value.vstr.len = strlen(words[stroke]);
						}
					}
					else if (words[stroke][0] == '\"') {
						if (ItEntr.iddatatype = IT::CHAR) {
							if (strlen(words[stroke]) - 2 > 1) {
								throw ERROR_THROW_IN(310, sn + 1, stroke + 1);
							}
							strcpy_s(ItEntr.value.vstr.str, words[stroke]);
							ItEntr.value.vstr.len = strlen(words[stroke]);
						}
					}
					else if (CMP(words[stroke], "false") || CMP(words[stroke], "true")) {
						if (ItEntr.iddatatype = IT::BOOL) {
							strcpy_s(ItEntr.value.vstr.str, words[stroke]);
							ItEntr.value.vstr.len = strlen(words[stroke]);
						}

					}
					else {
						ItEntr.iddatatype = IT::INT;
						if (!minus)
							ItEntr.value.vint = stoi(words[stroke]);
						else
							ItEntr.value.vint = stoi(tem);
					}

					ItEntr.idxfirstLE = lextable.size;

					char t[10];
					char lit[10] = "L";
					strcat(lit, itoa(LitNumber, t, 10));
					strcpy(ItEntr.id, lit);
					IT::Add(idtable, ItEntr);
					ItEntr.iddatatype = IT::UNDEF;
					LitNumber++;

					continue;
				}
				else {
					IT::Entry e = IT::GetEntry(idtable, pos);
					LT::Entry e1 = LT::GetEntry(lextable, e.idxfirstLE - 1);
					e1.sn = sn;
					LT::Add(lextable, e1);
					continue;
				}
			}


			FST::FST fstWriteline(words[stroke], FST_WRITELINE);
			if (FST::execute(fstWriteline)) {
				LtEntr.lexema = LEX_WRITELINE;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}

			FST::FST fstWrite(words[stroke], FST_WRITE);
			if (FST::execute(fstWrite)) {
				LtEntr.lexema = LEX_WRITE;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}
			FST::FST cx(words[stroke], FST_ID);
			if (FST::execute(cx) && !st.empty() && st.top() == true) {
				LtEntr.lexema = LEX_ID;
				LtEntr.sn = sn;
				char* temp = new char[TI_MAXSIZE];
				getid(stroke, temp, words, 0);
				int tmp = IT::IsId(idtable, temp);
				LtEntr.idxTI = tmp;
				LT::Add(lextable, LtEntr);
				continue;
			}
			FST::FST FSTB1(words[stroke], FST_BOOLEAN);
			if (FST::execute(FSTB1)) {
				LtEntr.lexema = LEX_BOOL;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}

			FST::FST fstG(words[stroke], FST_GREATER);
			if (FST::execute(fstG)) {
				LtEntr.lexema = LEX_GREATER;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LtEntr.priority = 7;
				LT::Add(lextable, LtEntr);
				LtEntr.priority = 0;
				continue;
			}
			FST::FST fstS(words[stroke], FST_SMALLER);
			if (FST::execute(fstS)) {
				LtEntr.lexema = LEX_SMALLER;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LtEntr.priority = 7;
				LT::Add(lextable, LtEntr);
				LtEntr.priority = 0;
				continue;
			}
			FST::FST FSTGE(words[stroke], FST_GREATER_OR_EQUAL);
			if (FST::execute(FSTGE)) {
				LtEntr.lexema = LEX_GREATEROREQUAL;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LtEntr.priority = 7;
				LT::Add(lextable, LtEntr);
				LtEntr.priority = 0;
				continue;
			}
			FST::FST FSTSE(words[stroke], FST_SMALLER_OR_EQUAL);
			if (FST::execute(FSTSE)) {
				LtEntr.lexema = LEX_SMALLEROREQUAL;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LtEntr.priority = 7;
				LT::Add(lextable, LtEntr);
				LtEntr.priority = 0;
				continue;
			}
			FST::FST fstSym(words[stroke], FST_SYMBOL);
			if (FST::execute(fstSym)) {
				LtEntr.lexema = LEX_SYMBOL;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}
			FST::FST fstCol(words[stroke], FST_COLON);
			if (FST::execute(fstCol)) {
				LtEntr.lexema = LEX_COLON;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}
			FST::FST fstInt(words[stroke], FST_NUMB);
			if (FST::execute(fstInt)) {

				LtEntr.lexema = LEX_NUMB;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}
			FST::FST fstString(words[stroke], FST_STROKE);
			if (FST::execute(fstString)) {

				LtEntr.lexema = LEX_STROKE;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;

			}
			FST::FST fstsymb(words[stroke], FST_SYMBOL);
			if (FST::execute(fstString)) {

				LtEntr.lexema = LEX_SYMBOL;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}
			FST::FST fstbool(words[stroke], FST_BOOLEAN);
			if (FST::execute(fstString)) {

				LtEntr.lexema = LEX_BOOL;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;

			}
			FST::FST fstFunc(words[stroke], FST_FUNCTION);
			if (FST::execute(fstFunc)) {
				if (isMainDetected) {
					throw ERROR_THROW_IN(616, sn + 1, stroke + 1);
				}
				LtEntr.lexema = LEX_FUNCTION;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;

			}
			FST::FST fstNew(words[stroke], FST_NEW);
			if (FST::execute(fstNew)) {

				LtEntr.lexema = LEX_NEW;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				FST::FST bol(words[stroke + 1], FST_BOOLEAN);
				FST::FST num(words[stroke + 1], FST_NUMB);
				FST::FST str(words[stroke + 1], FST_STROKE);
				FST::FST sym(words[stroke + 1], FST_SYMBOL);
				if (FST::execute(bol) || FST::execute(num) || FST::execute(str) || FST::execute(sym))
					continue;
				throw ERROR_THROW_IN(317, sn + 1, stroke + 1);
			}
			FST::FST fstRet(words[stroke], FST_RETURN);
			if (FST::execute(fstRet)) {
				LtEntr.lexema = LEX_RETURN;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}

			FST::FST fstMain(words[stroke], FST_MAIN);
			if (FST::execute(fstMain)) {
				countOfMain++;
				if (countOfMain > 1)
					throw ERROR_THROW_IN(308, sn, stroke);
				isMainDetected = true;
				LtEntr.lexema = LEX_MAIN;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				strcpy(ItEntr.scope, "main");
				NameOfFunc = (char*)"main";
				buff = (char*)"main";
				LT::Add(lextable, LtEntr);
				FindMain = true;
				continue;

			}
			FST::FST fstSemi(words[stroke], FST_SEMICOLON);
			if (FST::execute(fstSemi)) {
				LtEntr.lexema = LEX_SEMICOLON;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				if (CMP(words[stroke + 1], ";"))
					throw ERROR_THROW_IN(319, sn + 1, stroke + 1);
				continue;

			}
			FST::FST fstNewLine(words[stroke], FST_NEWLINE);
			if (FST::execute(fstNewLine)) {
				sn++;
				continue;
			}

			FST::FST fstLBrace(words[stroke], FST_LEFTBRACE);
			if (FST::execute(fstLBrace)) {
				LtEntr.lexema = LEX_LEFTBRACE;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}
			FST::FST fstBrace(words[stroke], FST_BRACELET);
			if (FST::execute(fstBrace)) {
				LtEntr.lexema = LEX_BRACELET;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}
			FST::FST fstLH(words[stroke], FST_LEFTTHESIS);
			if (FST::execute(fstLH)) {
				if (idtable.table[lextable.table[posOfFunction].idxTI].idtype == IT::F) {
					findFunc = true;
				}
				LtEntr.priority = 1;
				LtEntr.lexema = LEX_LEFTHESIS;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;

			}
			FST::FST fstRH(words[stroke], FST_RIGHTTHESIS);
			if (FST::execute(fstRH)) {
				if (findFunc) {
					idtable.table[lextable.table[posOfFunction].idxTI].numberOfParam = numberOfPar;
					numberOfPar = 0;
					findFunc = false;
					posOfFunction = 0;
				}
				LtEntr.priority = 1;
				LtEntr.lexema = LEX_RIGHTHESIS;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				if (NameOfFunc != buff) {
					buff = NameOfFunc;
				}
				continue;
			}
			FST::FST fsrComma(words[stroke], FST_COMMA);
			if (FST::execute(fsrComma)) {
				LtEntr.lexema = LEX_COMMA;
				LtEntr.sn = sn;
				LtEntr.idxTI = LT_TI_NULLIDX;
				LT::Add(lextable, LtEntr);
				continue;
			}

			if (stroke >= 1 && CMP(words[stroke - 1], "return")) {
				FST::FST fstN(words[stroke], FST_INTLIT);
				if (FST::execute(fstN))
					LtEntr.lexema = LEX_LITERAL;
				else
					LtEntr.lexema = LEX_ID;
				LtEntr.sn = sn;
				LtEntr.idxTI = TI_NULLIDX;
				getid(stroke, id, words, 0);
				int position = IT::IsId(idtable, id);
				LtEntr.idxTI = position;
				LT::Add(lextable, LtEntr);
				continue;
			}
			if (stroke > 1) {
				FST::FST fstpr(words[stroke - 1], FST_WRITELINE);
				FST::FST fstpr1(words[stroke - 1], FST_WRITE);
				if (FST::execute(fstpr)) {
					FST::FST FSTTR(words[stroke], FST_TRUE);
					FST::FST FSTFAL(words[stroke], FST_FALSE);
					FST::FST LSR(words[stroke], FST_STRLIT);
					FST::FST LIT(words[stroke], FST_INTLIT);
					FST::FST LCHR(words[stroke], FST_SYMBLIT);
					if (FST::execute(LSR) || FST::execute(LIT) || FST::execute(LCHR) || FST::execute(FSTTR) || FST::execute(FSTFAL)) {
						bool here = false;
						int pos;
						bool toInt = false;
						for (int i = 0; i < strlen(words[stroke]); i++) {
							if (int(words[stroke][i]) < 48 || int(words[stroke][i]) > 58) {
								toInt = false;
								break;
							}
							toInt = true;
						}
						for (int i = 0; i < idtable.size; i++)
						{
							if (CMP(idtable.table[i].value.vstr.str, words[stroke]) || idtable.table[i].idtype == IT::L && idtable.table[i].iddatatype == IT::INT && toInt && stoi(words[stroke]) == idtable.table[i].value.vint) {	//ѕроверка на наличие в таблице идентификаторов
								here = true;
								pos = i;
								break;
							}
						}
						if (!here) {
							char tem[99] = "-";
							bool minus = false;
							if (words[stroke][0] == '-') {
								stroke++;
								char buff[99];
								strcpy_s(buff, words[stroke]);
								strcat_s(tem, buff);
								minus = true;
							}
							LtEntr.lexema = LEX_LITERAL;
							LtEntr.sn = sn;
							LtEntr.idxTI = idtable.size;
							LT::Add(lextable, LtEntr);
							int symbols = strlen(words[stroke]);
							if (symbols > TI_STR_MAXSIZE)
								throw ERROR_THROW_IN(164, sn + 1, stroke + 1);

							ItEntr.idtype = IT::L;
							if (words[stroke][0] == '\'') {
								if (ItEntr.iddatatype = IT::STR) {
									if (strlen(words[stroke]) > 40) {
										throw ERROR_THROW_IN(316, sn + 1, stroke + 1);
									}
									strcpy_s(ItEntr.value.vstr.str, words[stroke]);
									ItEntr.value.vstr.len = strlen(words[stroke]);
								}
							}
							else if (words[stroke][0] == '\"') {
								if (ItEntr.iddatatype = IT::CHAR) {
									if (strlen(words[stroke]) - 2 > 1) {
										throw ERROR_THROW_IN(310, sn + 1, stroke + 1);
									}
									strcpy_s(ItEntr.value.vstr.str, words[stroke]);
									ItEntr.value.vstr.len = strlen(words[stroke]);
								}
							}
							else if (CMP(words[stroke], "false") || CMP(words[stroke], "true")) {
								if (ItEntr.iddatatype = IT::BOOL) {
									strcpy_s(ItEntr.value.vstr.str, words[stroke]);
									ItEntr.value.vstr.len = strlen(words[stroke]);
								}

							}
							else {
								ItEntr.iddatatype = IT::INT;
								if (stoi(words[stroke]) > 65536)
									throw ERROR_THROW_IN(300, sn + 1, stroke + 1);
								if (!minus)
									ItEntr.value.vint = stoi(words[stroke]);
								else
									ItEntr.value.vint = stoi(tem);
							}

							ItEntr.idxfirstLE = lextable.size;

							char t[10];
							char lit[10] = "L";
							strcat(lit, itoa(LitNumber, t, 10));
							strcpy(ItEntr.id, lit);
							IT::Add(idtable, ItEntr);
							ItEntr.iddatatype = IT::UNDEF;
							LitNumber++;
							continue;
						}
						else {
							IT::Entry e = IT::GetEntry(idtable, pos);
							LT::Entry e1 = LT::GetEntry(lextable, e.idxfirstLE - 1);
							e1.sn = sn;
							LT::Add(lextable, e1);
							stroke++;
							continue;
						}
					}
					getid(stroke, id, words, 0);
					int position = IT::IsId(idtable, id);
					LtEntr.idxTI = position;
					LtEntr.lexema = LEX_ID;
					LtEntr.sn = sn;
					LT::Add(lextable, LtEntr);
					continue;

				}
				if (FST::execute(fstpr1)) {
					FST::FST FSTTR(words[stroke], FST_TRUE);
					FST::FST FSTFAL(words[stroke], FST_FALSE);
					FST::FST LSR(words[stroke], FST_STRLIT);
					FST::FST LIT(words[stroke], FST_INTLIT);
					FST::FST LCHR(words[stroke], FST_SYMBLIT);
					if (FST::execute(LSR) || FST::execute(LIT) || FST::execute(LCHR) || FST::execute(FSTTR) || FST::execute(FSTFAL)) {
						bool here = false;
						int pos;
						bool toInt = false;
						for (int i = 0; i < strlen(words[stroke]); i++) {
							if (int(words[stroke][i]) < 48 || int(words[stroke][i]) > 58) {
								toInt = false;
								break;
							}
							toInt = true;
						}
						for (int i = 0; i < idtable.size; i++)
						{
							if (CMP(idtable.table[i].value.vstr.str, words[stroke]) || idtable.table[i].idtype == IT::L && idtable.table[i].iddatatype == IT::INT && toInt && stoi(words[stroke]) == idtable.table[i].value.vint) {	//ѕроверка на наличие в таблице идентификаторов
								here = true;
								pos = i;
								break;
							}
						}
						if (!here) {
							char tem[99] = "-";
							bool minus = false;
							if (words[stroke][0] == '-') {
								stroke++;
								char buff[99];
								strcpy_s(buff, words[stroke]);
								strcat_s(tem, buff);
								minus = true;
							}
							LtEntr.lexema = LEX_LITERAL;
							LtEntr.sn = sn;
							LtEntr.idxTI = idtable.size;
							LT::Add(lextable, LtEntr);
							int symbols = strlen(words[stroke]);
							if (symbols > TI_STR_MAXSIZE)
								throw ERROR_THROW_IN(164, sn + 1, stroke + 1);

							ItEntr.idtype = IT::L;
							if (words[stroke][0] == '\'') {
								if (ItEntr.iddatatype = IT::STR) {
									if (strlen(words[stroke]) > 40) {
										throw ERROR_THROW_IN(316, sn + 1, stroke + 1);
									}
									strcpy_s(ItEntr.value.vstr.str, words[stroke]);
									ItEntr.value.vstr.len = strlen(words[stroke]);
								}
							}
							else if (words[stroke][0] == '\"') {
								if (ItEntr.iddatatype = IT::CHAR) {
									if (strlen(words[stroke]) - 2 > 1) {
										throw ERROR_THROW_IN(310, sn + 1, stroke + 1);
									}
									strcpy_s(ItEntr.value.vstr.str, words[stroke]);
									ItEntr.value.vstr.len = strlen(words[stroke]);
								}
							}
							else if (CMP(words[stroke], "false") || CMP(words[stroke], "true")) {
								if (ItEntr.iddatatype = IT::BOOL) {
									strcpy_s(ItEntr.value.vstr.str, words[stroke]);
									ItEntr.value.vstr.len = strlen(words[stroke]);
								}

							}
							else {
								ItEntr.iddatatype = IT::INT;
								if (stoi(words[stroke]) > 65536)
									throw ERROR_THROW_IN(300, sn + 1, stroke + 1);
								if (!minus)
									ItEntr.value.vint = stoi(words[stroke]);
								else
									ItEntr.value.vint = stoi(tem);
							}

							ItEntr.idxfirstLE = lextable.size;

							char t[10];
							char lit[10] = "L";
							strcat(lit, itoa(LitNumber, t, 10));
							strcpy(ItEntr.id, lit);
							IT::Add(idtable, ItEntr);
							ItEntr.iddatatype = IT::UNDEF;
							LitNumber++;
							continue;
						}
						else {
							IT::Entry e = IT::GetEntry(idtable, pos);
							LT::Entry e1 = LT::GetEntry(lextable, e.idxfirstLE - 1);
							e1.sn = sn;
							LT::Add(lextable, e1);
							stroke++;
							continue;
						}
					}
					getid(stroke, id, words, 0);
					int position = IT::IsId(idtable, id);
					LtEntr.idxTI = position;
					LtEntr.lexema = LEX_ID;
					LtEntr.sn = sn;
					LT::Add(lextable, LtEntr);
					continue;

				}
			}

			FST::FST FSTI(words[stroke - 2], FST_NUMB);
			FST::FST FSTS(words[stroke - 2], FST_STROKE);
			FST::FST FSTb(words[stroke - 2], FST_BOOLEAN);
			FST::FST FSTs(words[stroke - 2], FST_SYMBOL);
			FST::FST FSTF(words[stroke - 1], FST_FUNCTION);
			if (stroke >= 2 && (FST::execute(FSTI) || FST::execute(FSTS) || FST::execute(FSTb) || FST::execute(FSTs)) && FST::execute(FSTF)) {
				int size = strlen(words[stroke]);
				if (size > ID_MAXSIZE) {
					char* t;
					t = changeID(words[stroke]);
					strcpy(ItEntr.id, words[stroke]);
				}
				else {
					strcpy(ItEntr.id, words[stroke]);
				}
				if (CMP(words[stroke - 2], "stroke")) {
					ItEntr.iddatatype = IT::STR;
				}
				else if (CMP(words[stroke - 2], "numb")) {
					ItEntr.iddatatype = IT::INT;
				}
				else if (CMP(words[stroke - 2], "symbol")) {
					ItEntr.iddatatype = IT::CHAR;
				}
				else if (CMP(words[stroke - 2], "boolean")) {
					ItEntr.iddatatype = IT::BOOL;
				}
				ItEntr.idtype = IT::F;
				ItEntr.idxfirstLE = lextable.size;
				strcpy(ItEntr.scope, words[stroke]);
				LtEntr.idxTI = idtable.size;
				LtEntr.sn = sn;
				LtEntr.lexema = LEX_ID;
				posOfFunction = lextable.size;

				strcpy(ItEntr.scope, words[stroke]);
				LT::Add(lextable, LtEntr);
				IT::Add(idtable, ItEntr);
				NameOfFunc = words[stroke];
				ItEntr.iddatatype = IT::UNDEF;
				continue;
			}
			FST::FST FSTSt(words[stroke - 1], FST_STROKE);
			FST::FST FSTIn(words[stroke - 1], FST_NUMB);
			FST::FST fstsy(words[stroke - 1], FST_SYMBOL);
			FST::FST fstboole(words[stroke - 1], FST_BOOLEAN);
			FST::FST FSTC(words[stroke - 2], FST_COMMA);
			FST::FST FSTLH(words[stroke - 2], FST_LEFTTHESIS);
			if (stroke >= 2 && (FST::execute(FSTSt) || FST::execute(FSTIn) || FST::execute(fstsymb) || FST::execute(fstboole)) && (FST::execute(FSTC) || FST::execute(FSTLH))) {//(integer x, integer y) || (string a, string b) 
				if (CMP(words[stroke - 1], "stroke")) {
					ItEntr.iddatatype = IT::STR;
				}
				else if (CMP(words[stroke - 1], "numb")) {
					ItEntr.iddatatype = IT::INT;
				}
				else if (CMP(words[stroke - 1], "symbol")) {
					ItEntr.iddatatype = IT::CHAR;
				}
				else if (CMP(words[stroke - 1], "boolean")) {
					ItEntr.iddatatype = IT::BOOL;
				}
				ItEntr.idtype = IT::P;
				ItEntr.idxfirstLE = lextable.size;
				LtEntr.idxTI = idtable.size;
				LtEntr.lexema = LEX_ID;
				LtEntr.sn = sn;
				numberOfPar++;
				int col = /*strlen(NameOfFunc)*/ +strlen(words[stroke]);
				if (col + 1 > ID_MAXSIZE)
					throw ERROR_THROW(162);

				strcpy(ItEntr.id, words[stroke]);

				if (ItEntr.iddatatype == IT::INT)
				{
					ItEntr.value.vint = 0;
				}
				else
				{
					strcpy_s(ItEntr.value.vstr.str, "");
					ItEntr.value.vstr.len = 0;
				}

				LT::Add(lextable, LtEntr);
				IT::Add(idtable, ItEntr);
				ItEntr.iddatatype = IT::UNDEF;
				continue;
			}
			FST::FST FSTDecl(words[stroke - 2], FST_NEW);
			FST::FST FSTSTR(words[stroke - 1], FST_STROKE);
			FST::FST FSTINT(words[stroke - 1], FST_NUMB);
			FST::FST FSTSYMB(words[stroke - 1], FST_SYMBOL);
			FST::FST FSTBOL(words[stroke - 1], FST_BOOLEAN);
			FST::FST FSTFunc(words[stroke], FST_FUNCTION);
			if (stroke >= 2 && !FST::execute(FSTDecl) && (FST::execute(FSTSTR) || FST::execute(FSTINT) || FST::execute(FSTSYMB) || FST::execute(FSTBOL)) && !FST::execute(FSTFunc))
				throw ERROR_THROW_IN(302, sn + 1, stroke + 1);
			if (stroke >= 2 && FST::execute(FSTDecl) && (FST::execute(FSTSTR) || FST::execute(FSTINT) || FST::execute(FSTSYMB) || FST::execute(FSTBOL)) && !FST::execute(FSTFunc)) {//new numb x;
				if (CMP(words[stroke - 1], "stroke")) {
					ItEntr.iddatatype = IT::STR;
				}
				else if (CMP(words[stroke - 1], "numb")) {
					ItEntr.iddatatype = IT::INT;
				}
				else if (CMP(words[stroke - 1], "symbol")) {
					ItEntr.iddatatype = IT::CHAR;
				}
				else if (CMP(words[stroke - 1], "boolean")) {
					ItEntr.iddatatype = IT::BOOL;
				}
				ItEntr.idxfirstLE = lextable.size;
				LtEntr.lexema = LEX_ID;
				LtEntr.idxTI = idtable.size;
				LtEntr.sn = sn;
				int col =/* strlen(NameOfFunc)*/ +strlen(words[stroke]);
				if (col + 1 > ID_MAXSIZE) //область функции+ param + \0
					throw ERROR_THROW(162);
				strcpy(ItEntr.id, words[stroke]);
				if (ItEntr.iddatatype == IT::INT)
				{
					ItEntr.value.vint = 0;
				}
				else
				{
					strcpy_s(ItEntr.value.vstr.str, "");
					ItEntr.value.vstr.len = 0;
				}
				ItEntr.idtype = IT::V;
				LT::Add(lextable, LtEntr);
				IT::Add(idtable, ItEntr);
				ItEntr.iddatatype = IT::UNDEF;
				continue;
			}
			FST::FST fstEqua(words[stroke + 1], FST_EQUAL);
			FST::FST fstword(words[stroke], FST_ID);
			if (FST::execute(fstEqua) && FST::execute(fstword))
				continue;
			FST::FST fstEq(words[stroke], FST_EQUAL);
			if (FST::execute(fstEq)) {
				FST::FST FSTID(words[stroke - 1], FST_ID);
				if (FST::execute(FSTID)) {
					bool inState = false;
					if (!st.empty() && st.top() == true) {
						inState = true;
					}
					if (!CMP(words[stroke - 3], "new") && !inState) {
						int position = IT::IsId(idtable, words[stroke - 1]);
						LtEntr.idxTI = position;
						LtEntr.lexema = LEX_ID;
						LtEntr.sn = sn;
						LT::Add(lextable, LtEntr);
					}
				}
				LtEntr.idxTI = TI_NULLIDX;
				LtEntr.lexema = LEX_EQUAL;
				LtEntr.priority = 0;
				LtEntr.sn = sn;
				LT::Add(lextable, LtEntr);
				stroke++;
				while (!CMP(words[stroke], ";")) {
					FST::FST fstand1(words[stroke], FST_AND);
					if (FST::execute(fstand1)) {
						LtEntr.lexema = LEX_AND;
						LtEntr.sn = sn;
						LtEntr.idxTI = LT_TI_NULLIDX;
						LT::Add(lextable, LtEntr);
						stroke++;
						continue;
					}
					FST::FST fstor1(words[stroke], FST_OR);
					if (FST::execute(fstor1)) {
						LtEntr.lexema = LEX_OR;
						LtEntr.sn = sn;
						LtEntr.idxTI = LT_TI_NULLIDX;
						LT::Add(lextable, LtEntr);
						stroke++;
						continue;
					}
					FST::FST fstinver1(words[stroke], FST_INVERSION);
					if (FST::execute(fstinver1)) {
						LtEntr.lexema = LEX_INVERSION;
						LtEntr.sn = sn;
						LtEntr.idxTI = LT_TI_NULLIDX;
						LT::Add(lextable, LtEntr);
						stroke++;
						continue;
					}
					FST::FST fstLH2(words[stroke], FST_LEFTTHESIS);
					if (FST::execute(fstLH2)) {
						LtEntr.priority = 1;
						LtEntr.lexema = LEX_LEFTHESIS;
						LtEntr.sn = sn;
						LtEntr.idxTI = LT_TI_NULLIDX;
						LT::Add(lextable, LtEntr);
						stroke++;
						continue;
					}
					FST::FST fstNewLine1(words[stroke], FST_NEWLINE);
					if (FST::execute(fstNewLine1)) {
						sn++;
						if (!CMP(words[stroke - 1], ";"))
							throw ERROR_THROW_IN(318, sn, stroke - 1);
						continue;
					}
					FST::FST FSTR(words[stroke], FST_REMAINDER);
					if (FST::execute(FSTR)) {
						LtEntr.priority = 4;
						LtEntr.lexema = LEX_REMAINDER;
						LtEntr.sn = sn;
						LtEntr.idxTI = LT_TI_NULLIDX;
						LT::Add(lextable, LtEntr);
						stroke++;
						continue;
					}
					FST::FST fstRH2(words[stroke], FST_RIGHTTHESIS);
					if (FST::execute(fstRH2)) {
						LtEntr.priority = 1;
						LtEntr.lexema = LEX_RIGHTHESIS;
						LtEntr.sn = sn;
						LtEntr.idxTI = LT_TI_NULLIDX;
						LT::Add(lextable, LtEntr);
						stroke++;
						continue;
					}
					FST::FST FSTPl(words[stroke], FST_PLUS);
					if (FST::execute(FSTPl)) {
						LtEntr.priority = 5;
						LtEntr.lexema = LEX_PLUS;
						LtEntr.sn = sn;
						LtEntr.idxTI = LT_TI_NULLIDX;
						LT::Add(lextable, LtEntr);
						stroke++;
						continue;
					}
					FST::FST FSTMin(words[stroke], FST_MINUS);
					if (FST::execute(FSTMin)) {
						LtEntr.priority = 5;
						LtEntr.lexema = LEX_MINUS;
						LtEntr.sn = sn;
						LtEntr.idxTI = LT_TI_NULLIDX;
						LT::Add(lextable, LtEntr);
						stroke++;
						continue;
					}
					FST::FST FSTSt(words[stroke], FST_STAR);
					if (FST::execute(FSTSt)) {
						LtEntr.priority = 4;
						LtEntr.lexema = LEX_STAR;
						LtEntr.sn = sn;
						LtEntr.idxTI = LT_TI_NULLIDX;
						LT::Add(lextable, LtEntr);
						stroke++;
						continue;
					}
					FST::FST FSTDS(words[stroke], FST_DIRSLASH);
					if (FST::execute(FSTDS)) {
						LtEntr.priority = 4;
						LtEntr.lexema = LEX_DIRSLASH;
						LtEntr.sn = sn;
						LtEntr.idxTI = TI_NULLIDX;
						LT::Add(lextable, LtEntr);
						stroke++;
						continue;
					}
					FST::FST FSTcomma(words[stroke], FST_COMMA);
					if (FST::execute(FSTcomma)) {
						LtEntr.lexema = LEX_COMMA;
						LtEntr.sn = sn;
						LtEntr.idxTI = LT_TI_NULLIDX;
						LT::Add(lextable, LtEntr);
						stroke++;
						continue;
					}
					FST::FST FSTSTRL(words[stroke], FST_STRLEN);
					FST::FST FSTRAND(words[stroke], FST_RAND);
					FST::FST FSTINP(words[stroke], FST_INPUT);
					FST::FST FSTABS(words[stroke], FST_ABS);
					FST::FST FSTSQRT(words[stroke], FST_SQRT);
					if (FST::execute(FSTSTRL) || FST::execute(FSTRAND) || FST::execute(FSTINP) || FST::execute(FSTABS) || FST::execute(FSTSQRT)) {
						LtEntr.idxTI = idtable.size;
						bool findRand = false, findStrlen = false, findinput = false, findABS = false, findSQRT = false;
						if (CMP(words[stroke], "strlen")) {
							LtEntr.lexema = LEX_STRLEN;
							findStrlen = true;
						}
						else if (CMP(words[stroke], "rand")) {
							findRand = true;
							LtEntr.lexema = LEX_RAND;
						}
						else if (CMP(words[stroke], "input")) {
							findinput = true;
							LtEntr.lexema = LEX_INPUT;
						}
						else if (CMP(words[stroke], "abs")) {
							findABS = true;
							LtEntr.lexema = LEX_ABS;
						}
						else if (CMP(words[stroke], "sqrt")) {
							findSQRT = true;
							LtEntr.lexema = LEX_SQRT;
						}
						LtEntr.sn = sn;
						LtEntr.priority = 0;
						LT::Add(lextable, LtEntr);
						ItEntr.idxfirstLE = lextable.size;
						strcpy(ItEntr.id, words[stroke]);
						strcpy(ItEntr.scope, NameOfFunc);
						ItEntr.idtype = IT::F;
						ItEntr.iddatatype = IT::INT;
						IT::Add(idtable, ItEntr);
						int counter = 0;
						while (!CMP(words[++stroke], ")")) {
							FST::FST fstLH1(words[stroke], FST_LEFTTHESIS);
							if (FST::execute(fstLH1)) {
								LtEntr.priority = 1;
								LtEntr.lexema = LEX_LEFTHESIS;
								LtEntr.sn = sn;
								LtEntr.idxTI = LT_TI_NULLIDX;
								LT::Add(lextable, LtEntr);
								counter++;
								continue;
							}
							FST::FST FSTTR(words[stroke], FST_TRUE);
							FST::FST FSTFAL(words[stroke], FST_FALSE);
							FST::FST LSR(words[stroke], FST_STRLIT);
							FST::FST LIT(words[stroke], FST_INTLIT);
							FST::FST LCHR(words[stroke], FST_SYMBLIT);
							if (FST::execute(LSR) || FST::execute(LIT) || FST::execute(LCHR) || FST::execute(FSTTR) || FST::execute(FSTFAL)) {
								bool here = false;
								int pos;
								bool toInt = false;
								for (int i = 0; i < strlen(words[stroke]); i++) {
									if (int(words[stroke][i]) < 48 || int(words[stroke][i]) > 58) {
										toInt = false;
										break;
									}
									toInt = true;
								}
								for (int i = 0; i < idtable.size; i++)
								{
									if (CMP(idtable.table[i].value.vstr.str, words[stroke]) || idtable.table[i].idtype == IT::L && idtable.table[i].iddatatype == IT::INT && toInt && stoi(words[stroke]) == idtable.table[i].value.vint) {	//ѕроверка на наличие в таблице идентификаторов
										here = true;
										pos = i;
										break;
									}
								}
								if (!here) {
									char tem[99] = "-";
									bool minus = false;
									if (words[stroke][0] == '-') {
										stroke++;
										char buff[99];
										strcpy_s(buff, words[stroke]);
										strcat_s(tem, buff);
										minus = true;
									}
									LtEntr.lexema = LEX_LITERAL;
									LtEntr.sn = sn;
									LtEntr.idxTI = idtable.size;
									LT::Add(lextable, LtEntr);
									int symbols = strlen(words[stroke]);
									if (symbols > TI_STR_MAXSIZE)
										throw ERROR_THROW_IN(164, sn + 1, stroke + 1);

									ItEntr.idtype = IT::L;
									if (words[stroke][0] == '\'') {
										if (ItEntr.iddatatype = IT::STR) {
											if (strlen(words[stroke]) > 255) {
												throw ERROR_THROW_IN(316, sn + 1, stroke + 1);
											}
											strcpy_s(ItEntr.value.vstr.str, words[stroke]);
											ItEntr.value.vstr.len = strlen(words[stroke]);
										}
									}
									else if (words[stroke][0] == '\"') {
										if (ItEntr.iddatatype = IT::CHAR) {
											if (strlen(words[stroke]) - 2 > 1) {
												throw ERROR_THROW_IN(310, sn + 1, stroke + 1);
											}
											strcpy_s(ItEntr.value.vstr.str, words[stroke]);
											ItEntr.value.vstr.len = strlen(words[stroke]);
										}
									}
									else if (CMP(words[stroke], "false") || CMP(words[stroke], "true")) {
										if (ItEntr.iddatatype = IT::BOOL) {
											strcpy_s(ItEntr.value.vstr.str, words[stroke]);
											ItEntr.value.vstr.len = strlen(words[stroke]);
										}

									}
									else {
										ItEntr.iddatatype = IT::INT;
										if (stoi(words[stroke]) > 65536)
											throw ERROR_THROW_IN(300, sn + 1, stroke + 1);
										if (!minus)
											ItEntr.value.vint = stoi(words[stroke]);
										else
											ItEntr.value.vint = stoi(tem);
									}

									ItEntr.idxfirstLE = lextable.size;

									char t[10];
									char lit[10] = "L";
									strcat(lit, itoa(LitNumber, t, 10));
									strcpy(ItEntr.id, lit);
									IT::Add(idtable, ItEntr);
									ItEntr.iddatatype = IT::UNDEF;
									LitNumber++;
									continue;
								}
								else {
									IT::Entry e = IT::GetEntry(idtable, pos);
									LT::Entry e1 = LT::GetEntry(lextable, e.idxfirstLE - 1);
									e1.sn = sn;
									LT::Add(lextable, e1);
									continue;
								}
							}
							FST::FST FSTID(words[stroke], FST_ID);
							if (FST::execute(FSTID)) {
								getid(stroke, id, words, 0);
								int position = IT::IsId(idtable, id);
								LtEntr.idxTI = position;
								LtEntr.lexema = LEX_ID;
								LtEntr.sn = sn;
								LT::Add(lextable, LtEntr);
								continue;
							}
							else {
								throw ERROR_THROW_IN(312, sn + 1, stroke + 1);
							}
						}
						FST::FST fstRH1(words[stroke], FST_RIGHTTHESIS);
						if (FST::execute(fstRH1)) {
							LtEntr.priority = 1;
							LtEntr.lexema = LEX_RIGHTHESIS;
							LtEntr.sn = sn;
							LtEntr.idxTI = LT_TI_NULLIDX;
							LT::Add(lextable, LtEntr);
							if (NameOfFunc != buff) {
								buff = NameOfFunc;
							}
							counter++;
							stroke++;
						}
						if (counter % 2 != 0) {
							throw ERROR_THROW_IN(312, sn + 1, stroke + 1);
						}
						continue;
					}
					FST::FST FSTTR(words[stroke], FST_TRUE);
					FST::FST FSTFAL(words[stroke], FST_FALSE);
					FST::FST LSR(words[stroke], FST_STRLIT);
					FST::FST LIT(words[stroke], FST_INTLIT);
					FST::FST LCHR(words[stroke], FST_SYMBLIT);
					if (FST::execute(LSR) || FST::execute(LIT) || FST::execute(LCHR) || FST::execute(FSTTR) || FST::execute(FSTFAL)) {
						bool here = false;
						int pos;
						bool toInt = false;
						for (int i = 0; i < strlen(words[stroke]); i++) {
							if (int(words[stroke][i]) < 48 || int(words[stroke][i]) > 58) {
								toInt = false;
								break;
							}
							toInt = true;
						}
						for (int i = 0; i < idtable.size; i++)
						{
							if (CMP(idtable.table[i].value.vstr.str, words[stroke]) || idtable.table[i].idtype == IT::L && idtable.table[i].iddatatype == IT::INT && toInt && stoi(words[stroke]) == idtable.table[i].value.vint) {	//ѕроверка на наличие в таблице идентификаторов
								here = true;
								pos = i;
								break;
							}
						}
						if (!here) {
							char tem[99] = "-";
							bool minus = false;
							if (words[stroke][0] == '-') {
								stroke++;
								char buff[99];
								strcpy_s(buff, words[stroke]);
								strcat_s(tem, buff);
								minus = true;
							}
							LtEntr.lexema = LEX_LITERAL;
							LtEntr.sn = sn;
							LtEntr.idxTI = idtable.size;
							LT::Add(lextable, LtEntr);
							int symbols = strlen(words[stroke]);
							if (symbols > TI_STR_MAXSIZE)
								throw ERROR_THROW_IN(164, sn + 1, stroke + 1);

							ItEntr.idtype = IT::L;
							if (words[stroke][0] == '\'') {
								if (ItEntr.iddatatype = IT::STR) {
									if (strlen(words[stroke]) > 40) {
										throw ERROR_THROW_IN(316, sn + 1, stroke + 1);
									}
									strcpy_s(ItEntr.value.vstr.str, words[stroke]);
									ItEntr.value.vstr.len = strlen(words[stroke]);
								}
							}
							else if (words[stroke][0] == '\"') {
								if (ItEntr.iddatatype = IT::CHAR) {
									if (strlen(words[stroke]) - 2 > 1) {
										throw ERROR_THROW_IN(310, sn + 1, stroke + 1);
									}
									strcpy_s(ItEntr.value.vstr.str, words[stroke]);
									ItEntr.value.vstr.len = strlen(words[stroke]);
								}
							}
							else if (CMP(words[stroke], "false") || CMP(words[stroke], "true")) {
								if (ItEntr.iddatatype = IT::BOOL) {
									strcpy_s(ItEntr.value.vstr.str, words[stroke]);
									ItEntr.value.vstr.len = strlen(words[stroke]);
								}

							}
							else {
								ItEntr.iddatatype = IT::INT;
								if (stoi(words[stroke]) > 65536)
									throw ERROR_THROW_IN(300, sn + 1, stroke + 1);
								if (!minus)
									ItEntr.value.vint = stoi(words[stroke]);
								else
									ItEntr.value.vint = stoi(tem);
							}
							ItEntr.idxfirstLE = lextable.size;

							char t[10];
							char lit[10] = "L";
							strcat(lit, itoa(LitNumber, t, 10));
							strcpy(ItEntr.id, lit);
							IT::Add(idtable, ItEntr);
							ItEntr.iddatatype = IT::UNDEF;
							LitNumber++;
							stroke++;
							continue;
						}
						else {
							IT::Entry e = IT::GetEntry(idtable, pos);
							LT::Entry e1 = LT::GetEntry(lextable, e.idxfirstLE - 1);
							e1.sn = sn;
							LT::Add(lextable, e1);
							stroke++;
							continue;
						}
					}
					FST::FST FSTD(words[stroke], FST_ID);
					if (FST::execute(FSTD)) {
						getid(stroke, id, words, 0);
						int position = IT::IsId(idtable, id);
						LtEntr.idxTI = position;
						LtEntr.lexema = LEX_ID;
						LtEntr.sn = sn;
						LT::Add(lextable, LtEntr);
						stroke++;
						continue;
					}
				}
				FST::FST fstSemi2(words[stroke], FST_SEMICOLON);
				if (FST::execute(fstSemi2)) {
					LtEntr.lexema = LEX_SEMICOLON;
					LtEntr.sn = sn;
					LtEntr.idxTI = LT_TI_NULLIDX;
					LT::Add(lextable, LtEntr);
					if (CMP(words[stroke + 1], ";"))
						throw ERROR_THROW_IN(319, sn + 1, stroke + 1);
					continue;

				}
				
			}
			
			throw ERROR_THROW_IN(163, sn + 1, stroke + 1);
		}

		if (FindMain == false)
			throw ERROR_THROW(165);
		for (int j = 0; j < idtable.size; j++)
		{
			if (idtable.table[j].iddatatype == IT::UNDEF && (idtable.table[j].idtype == IT::V || idtable.table[j].idtype == IT::P || idtable.table[j].idtype == IT::F)) {
				throw ERROR_THROW_IN(307, lextable.table[idtable.table[j].idxfirstLE].sn, -1);
			}
		}

		tables.idtable = idtable;
		tables.lextable = lextable;
		return tables;
	}
}
