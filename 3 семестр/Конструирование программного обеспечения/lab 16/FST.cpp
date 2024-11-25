#include "FST.h"
#include <cstring> // Для strlen
#include <iostream>
using namespace std;

namespace FST {
	RELATION::RELATION(char c, short nn) {
		symbol = c;	//символ перехода
		nnode = nn;	//номер след состояния
	};

	NODE::NODE() {
		n_relation = 0;	//кол-во ребер
		RELATION* relations = NULL;	//указатель на массив ребер
	};

	NODE::NODE(short n, RELATION rel, ...) {
		n_relation = n;	//кол-во ребер
		RELATION* p = &rel;	//указатель на первый элемент переменного списка ребер
		relations = new RELATION[n];	//массив ребер
		for (short i = 0; i < n; i++) relations[i] = p[i];
	};

	FST::FST(char* s, short ns, NODE n, ...) {
		string = s;
		nstates = ns;	//кол-во состояний
		nodes = new NODE[ns];	//массив состояний
		NODE* p = &n;	//указатель на первый элемент переменного списка состояний
		for (int k = 0; k < ns; k++) nodes[k] = p[k];
		rstates = new short[nstates];
		memset(rstates, 0xff, sizeof(short) * nstates);
		rstates[0] = 0;
		position = -1;
	};

	bool step(FST& fst, short*& rstates)
	{
		bool rc = false;
		swap(rstates, fst.rstates);
		for (short i = 0; i < fst.nstates; i++) {
			if (rstates[i] == fst.position)
				for (short j = 0; j < fst.nodes[i].n_relation; j++) {	//перебираем все ребра для текущего состояния
					if (fst.nodes[i].relations[j].symbol == fst.string[fst.position]) {	//совпадает ли символ ребра с текущим символом строки
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;	//устанавливаем новое возможное состояние
						rc = true;
					}
				}
		}
		return rc;
	}

	bool execute(FST& fst)
	{
		short* rstates = new short[fst.nstates];
		memset(rstates, 0xff, sizeof(short) * fst.nstates);
		short lstring = strlen(fst.string);
		bool rc = true;
		for (short i = 0; i < lstring && rc; i++) {
			fst.position++;
			rc = step(fst, rstates);
		}
		delete[] rstates;

		return(rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
	};
};


