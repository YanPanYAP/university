#include "MFST.h"

namespace MFST
{
	MfstState::MfstState()
	{
		lenta_position = 0;
		nrule = -1;		//идентификатор правила
		nrulechain = -1;	//идентификатор цепи правила
	};

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain)
	{
		lenta_position = pposition;
		st = pst;	//состо€ние стека
		nrulechain = pnrulechain;
	};

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain)
	{
		lenta_position = pposition;
		st = pst;
		nrule = pnrule;
		nrulechain = pnrulechain;
	};

	Mfst::MfstDiagnosis::MfstDiagnosis()
	{
		lenta_position = -1;
		rc_step = SURPRISE;		//шаг состо€ни€
		nrule = -1;
		nrule_chain = -1;
	};

	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)
	{
		lenta_position = plenta_position;
		rc_step = prc_step;
		nrule = pnrule;
		nrule_chain = pnrule_chain;
	};

	Mfst::Mfst() { lenta = 0; lenta_size = lenta_position = 0; };

	Mfst::Mfst(LT::LexTable& lextable, GRB::Greibach pgrebach)
	{
		grebach = pgrebach;		//объект грамматики
		lex = lextable;
		lenta = new short[lenta_size = lex.size];
		for (int k = 0; k < lenta_size; k++)
			lenta[k] = GRB::Rule::Chain::T(lex.table[k].lexema);	// ѕреобразуем лексемы в форматы правил и сохран€ем в ленте
		lenta_position = 0;
		st.push(grebach.stbottomT);
		st.push(grebach.startN);
		nrulechain = -1;
	}

	// ћетод дл€ выполнени€ одного шага анализа
	Mfst::RC_STEP Mfst::step()
	{
		RC_STEP rc = SURPRISE;	// переменную результата 
		if (lenta_position < lenta_size)
		{
			if (GRB::Rule::Chain::isN(st.top()))	// ≈сли верхний элемент стека - нетерминал
			{
				GRB::Rule rule;		// —оздаем объект правила
				if ((nrule = grebach.getRule(st.top(), rule)) >= 0)		// ѕолучаем правило по верхнему элементу стека
				{
					GRB::Rule::Chain chain;		// —оздаем объект цепи
					if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0)	// ѕолучаем следующую цепь правила
					{
						MFST_TRACE1
							savestate(); st.pop(); push_chain(chain); rc = NS_OK;	// —охран€ем состо€ние, убираем верхний элемент и добавл€ем цепь
						MFST_TRACE2
					}
					else
					{
						// ≈сли цепь не найдена, сохран€ем диагноз и устанавливаем код ошибки
						MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE")
							savediagnosis(NS_NORULECHAIN); rc = resetstate() ? NS_NORULECHAIN : NS_NORULE;
					};
				}
				else rc = NS_ERROR;
			}
			else if ((st.top() == lenta[lenta_position]))
			{
				lenta_position++; st.pop(); nrulechain = -1; rc = TS_OK;
				MFST_TRACE3		// “рассировка успешного шага
			}
			else { MFST_TRACE4(TS_NOK / NS_NORULECHAIN) rc = resetstate() ? TS_NOK : NS_NORULECHAIN; };
		}
		else
		{
			rc = LENTA_END;
			MFST_TRACE4(LENTA_END);
		};
		return rc;
	};

	// ћетод дл€ добавлени€ цепи в стек
	bool Mfst::push_chain(GRB::Rule::Chain chain)
	{
		for (int k = chain.size - 1; k >= 0; k--) st.push(chain.nt[k]);
		return true;
	};

	// ћетод дл€ сохранени€ текущего состо€ни€
	bool Mfst::savestate()
	{
		storestate.push(MfstState(lenta_position, st, nrule, nrulechain));
		MFST_TRACE6("SAVESTATE:", storestate.size());
		return true;
	};

	// ћетод дл€ сброса состо€ни€
	bool Mfst::resetstate()
	{
		bool rc = false;
		MfstState state;	// —оздаем объект состо€ни€	
		if (rc = (storestate.size() > 0))
		{
			state = storestate.top();
			lenta_position = state.lenta_position;
			st = state.st;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop();
			MFST_TRACE5("RESSTATE")
				MFST_TRACE2
		};
		return rc;
	};

	// ћетод дл€ сохранени€ диагноза
	bool Mfst::savediagnosis(RC_STEP prc_step)
	{
		bool rc = false;
		short k = 0;

		while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)
			k++;

		if (rc = (k < MFST_DIAGN_NUMBER))
		{
			diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain);	// —охран€ем диагноз

			for (int i = k + 1; i < MFST_DIAGN_NUMBER; i++)
				diagnosis[i].lenta_position = -1;
		}

		return rc;
	};

	// ћетод дл€ запуска анализа
	bool Mfst::start()
	{
		bool rc = false;
		RC_STEP rc_step = SURPRISE;		//шаг состо€ни€
		char buf[MFST_DIAGN_MAXSIZE]{};		// Ѕуфер дл€ хранени€ сообщений диагностики
		rc_step = step();
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
			rc_step = step();

		switch (rc_step)
		{
		case LENTA_END:
		{
			MFST_TRACE4("------>LENTA_END")
				std::cout << "------------------------------------------------------------------------------------------   ------" << std::endl;
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: всего строк %d, синтаксический анализ выполнен без ошибок", 0, lex.table[lex.size - 1].sn);
			std::cout << std::setw(4) << std::left << 0 << "всего строк " << lex.table[lex.size - 1].sn << ", синтаксический анализ выполнен без ошибок" << std::endl;
			rc = true;
			break;
		}

		case NS_NORULE:
		{
			MFST_TRACE4("------>NS_NORULE")
				std::cout << "------------------------------------------------------------------------------------------   ------" << std::endl;
			std::cout << getDiagnosis(0, buf) << std::endl;
			std::cout << getDiagnosis(1, buf) << std::endl;
			std::cout << getDiagnosis(2, buf) << std::endl;
			break;
		}

		case NS_NORULECHAIN:	MFST_TRACE4("------>NS_NORULECHAIN") break;
		case NS_ERROR:			MFST_TRACE4("------>NS_ERROR") break;
		case SURPRISE:			MFST_TRACE4("------>NS_SURPRISE") break;


		}
		return rc;
	};

	// ћетод дл€ получени€ строкового представлени€ состо€ни€ стека
	char* Mfst::getCSt(char* buf)
	{
		short p;
		for (int k = (signed)st.size() - 1; k >= 0; --k)
		{
			p = st.c[k];	// ѕолучаем значение верхнего элемента стека
			buf[st.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p);		// ѕреобразуем значение в символ и сохран€ем в буфере
		}
		buf[st.size()] = '\0';
		return buf;
	}

	// ћетод дл€ получени€ строкового представлени€ части ленты
	char* Mfst::getCLenta(char* buf, short pos, short n)
	{
		short i = 0, k = (pos + n < lenta_size) ? pos + n : lenta_size;		// ќпредел€ем границу получени€ подстроки ленты

		for (int i = pos; i < k; i++)
			buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);	// ѕреобразуем значени€ ленты в символы и сохран€ем в буфере


		return buf;
	}

	// ћетод дл€ получени€ сообщени€ о диагнозе
	char* Mfst::getDiagnosis(short n, char* buf)
	{
		char* rc = new char[200] {};
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = grebach.getRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = Error::geterror(errid);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d,%s", err.id, lex.table[lpos].sn, err.message);
			rc = buf;
		}
		return rc;
	}

	// ћетод дл€ печати правил
	void Mfst::printrules()
	{
		MfstState state;
		GRB::Rule rule;
		for (unsigned short i = 0; i < storestate.size(); i++)
		{
			state = storestate.c[i];
			rule = grebach.getRule(state.nrule);
			MFST_TRACE7
		};
	};

	// ћетод дл€ сохранени€ информации о выводе
	bool Mfst::savededucation()
	{
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = storestate.size()];
		deducation.nrulechains = new short[deducation.size];

		for (unsigned short i = 0; i < storestate.size(); i++)
		{
			state = storestate.c[i];
			deducation.nrules[i] = state.nrule;
			deducation.nrulechains[i] = state.nrulechain;
		}
		return true;
	}
}