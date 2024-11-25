#pragma once

#include "stdafx.h"
namespace FST
{
	struct RELATION // Ребро: символ -> вершина графа КА
	{
		char symbol; 
		short nnode; // Смежная вершина

		RELATION(
			unsigned char c = 'a', // Символ перехода
			short nn = 0 // Новое состояние
		);
	};


	struct NODE // Вершина графа переходов
	{
		short n_relation; // Количество инцидентных рёбер
		RELATION* relations;

		NODE();
		NODE(short n, RELATION rel, ...);
	};

	struct FST // Недетерминированный КА
	{
		std::string string; 
		short position; 
		short nstates; 
		NODE* nodes; // Граф переходов 
		short* rstates; 

		FST(
			std::string s,
			short ns, 
			NODE n, ... // Граф переходов 
		);
	};

	bool execute(FST& fst);
	void WriteError(Error::ERROR e);

}


