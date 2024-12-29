#pragma once
#include "stdafx.h"
namespace FST
{
	struct RELATION
	{
		char symbol;
		int nnode;

		RELATION(
			char c = 0x00,
			short ns = 0
		);
	};

	struct NODE
	{
		short n_relation;
		RELATION* relations;

		NODE();

		NODE(
			short n,
			RELATION rel,
			...
		);
	};

	struct FST
	{
		char* string;
		short position;
		short nstates;
		NODE* nodes;
		short* rstates;

		FST(
			char* s,
			short ns,
			NODE n,
			...
		);
	};

	bool execute(
		FST& fst
	);

	short* setRelState(
		char symb,
		FST& fst,
		short* pNRStates,
		short* rstates);

	bool isAllowed(
		char symb,
		short*& rstates,
		short nNRStates,
		FST& fst);

	bool isLastState(
		short* rstates,
		short length,
		short countStates);

}