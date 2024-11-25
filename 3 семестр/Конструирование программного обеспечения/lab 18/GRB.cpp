#include "GRB.h"
#include "Rules.h"
#include "string.h"

namespace GRB
{
	//  онструктор класса Chain, принимает размер и элементы алфавита
	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
	{
		this->nt = new GRBALPHABET[this->size = psize];

		int* ptr = (int*)&s;
		for (int i = 0; i < psize; i++)
			this->nt[i] = (short)ptr[i];
	}

	//  онструктор класса Rule, который инициализирует правило с заданным нетерминалом и идентификатором ошибки
	Rule::Rule(GRBALPHABET pnn, int iderroe, short psize, Chain c, ...)
	{
		this->nn = pnn;									//нетерминал
		this->iderror = iderroe;						//идентификатор ошибки
		this->chains = new Chain[this->size = psize];	//массив цепей правил
		Chain* ptr = &c;								//указатель на цепь
		for (int i = 0; i < psize; i++)
			this->chains[i] = ptr[i];
	}

	//  онструктор класса Greibach, инициализирует стартовый нетерминал и терминал
	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottomT, short psize, Rule r, ...)
	{
		this->startN = pstartN;
		this->stbottomT = pstbottomT;
		this->rules = new Rule[this->size = psize];
		Rule* ptr = &r;
		for (int i = 0; i < psize; i++)
			rules[i] = ptr[i];
	}

	// ћетод получени€ правила по заданному нетерминалу
	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
	{
		short rc = -1, k = 0;
		while (k < this->size && rules[k].nn != pnn)
			k++;
		if (k < this->size)			// ≈сли нашли правило
			prule = rules[rc = k];	// ¬озвращаем правило и обновл€ем rc
		return rc;					// ¬озвращаем индекс правила или -1, если не найдено
	}

	// ћетод получени€ правила по индексу
	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < this->size)
			rc = rules[n];
		return rc;
	}

	// ћетод получени€ строкового представлени€ правила
	char* Rule::getCRule(char* b, short nchain)
	{
		char buf[200];
		b[0] = Chain::alphabet_to_char(this->nn);	// «аписываем нетерминал в строку
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		this->chains[nchain].getCChain(buf);	// ѕолучаем строковое представление цепи
		strcat_s(b, sizeof(buf) + 5, buf);	// ќбъедин€ем строки

		return b;
	}

	// ћетод получени€ следующей цепи дл€ заданного терминала
	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;

		while (j < this->size && this->chains[j].nt[0] != t)	// »щем соответствующую цепь
			j++;

		rc = (j < this->size ? j : -1);
		if (rc >= 0)
			pchain = chains[rc];	// —охран€ем цепь
		return rc;
	}

	// ћетод получени€ строкового представлени€ цепи
	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < this->size; i++)
			b[i] = Chain::alphabet_to_char(this->nt[i]);	// ѕреобразуем элементы в символы
		b[this->size] = 0;
		return b;
	}

	// ћетод получени€ экземпл€ра Greibach
	Greibach getGreibach()
	{
		return greibach;
	}
}