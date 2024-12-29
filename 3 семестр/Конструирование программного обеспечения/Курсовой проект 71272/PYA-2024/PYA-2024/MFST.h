#pragma once
using namespace std;

#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

static int FST_TRACE_n = -1;
static char rbuf[205], sbuf[205], lbuf[1024];

#define MFST_TRACE_START(pars)	pars<< std::setw(4)<<std::left<<"Шаг"<<":"\
									<< std::setw(20)<<std::left<<" правило"\
									<< std::setw(30)<<std::left<<" иходная лента"\
									<< std::setw(20)<<std::left<<" стек"\
									<< std::endl;



#define MFST_TRACE1(pars)		pars<<std::setw(4)<<std::left<<++FST_TRACE_n<< ": "	\
									<<std::setw(20)<<std::left<<rule.getCRule(rbuf, nrulechain) \
									<<std::setw(30)<<std::left<<getCLenta(lbuf, lenta_position)	\
									<<std::setw(20)<<std::left<<getCSt(sbuf)	\
									<<std::endl;

#define MFST_TRACE2(pars)		pars<<std::setw(4)<<std::left<<FST_TRACE_n<< ": "	\
									<<std::setw(20)<<std::left<< " "	\
									<<std::setw(30)<<std::left<<getCLenta(lbuf, lenta_position)	\
									<<std::setw(20)<<std::left<<getCSt(sbuf)	\
									<<std::endl;

#define MFST_TRACE3(pars)		pars<<std::setw(4)<<std::left<<++FST_TRACE_n<< ": "	\
									<<std::setw(20)<<std::left<< " "	\
									<<std::setw(30)<<std::left<<getCLenta(lbuf, lenta_position)	\
									<<std::setw(20)<<std::left<<getCSt(sbuf)	\
									<<std::endl;

#define MFST_TRACE4(c,pars)		pars<<std::setw(4)<<std::left<<++FST_TRACE_n<< ": "<<std::setw(20)<<std::left<<c<<std::endl;

#define MFST_TRACE5(c,pars)		pars<<std::setw(4)<<std::left<<  FST_TRACE_n<< ": "<<std::setw(20)<<std::left<<c<<std::endl;

#define MFST_TRACE6(c,k,pars)	pars<<std::setw(4)<<std::left<<  FST_TRACE_n<< ": "<<std::setw(20)<<std::left<<c<<k<<std::endl;

#define	MFST_TRACE7(pars)		pars<< std::setw(4) << std::left << state.lenta_position<<": "	\
									<<std::setw(20)<<std::left<<rule.getCRule(rbuf, state.nrulechain)	\
									<<std::endl;

using namespace std;
typedef vector<short> MFSTSTSTACK;
namespace MFST
{
	struct MfstState
	{
		short lenta_position;
		short nrule;
		short nrulechain;
		MFSTSTSTACK st;

		MfstState();
		MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain);
		MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain);

	};

	struct Mfst
	{
		enum RC_STEP
		{
			NS_OK,// найдено правило и цепочка, цепочка записана в стек
			NS_NORULE,// не найдено правило грамматики (ошибка в грамматике)
			NS_NORULECHAIN,// не найдена поход€ща€ цепочка правила (ошибка в исходном коде)
			NS_ERROR,// неизвесный нетерминальный символ грамматики
			TS_OK,// тек. символ ленты == вершине стека, продвинулась лента, pop стека
			TS_NOK,// тек. символ ленты != вершине стека, восстановленно состо€ние
			LENTA_END,// теуща€ позици€ ленты >= lenta_size
			SURPRISE// неожиданный код возврата (ошибка в step)
		};

		struct MfstDiagnosis
		{
			short lenta_position;
			RC_STEP rc_step;
			short nrule;
			short nrule_chain;
			MfstDiagnosis();
			MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain);
		} diagnosis[MFST_DIAGN_NUMBER];

		GRBALPHABET* lenta;
		short lenta_position;
		short nrule;
		short nrulechain;
		short lenta_size;
		GRB::Greibach grebach;
		LT::LexTable lexTable;
		MFSTSTSTACK st;
		vector<MfstState> storestate;

		Mfst();
		Mfst(LT::LexTable& plexTable, GRB::Greibach pgrebach, wchar_t parsfile[]);

		char* getCSt(char* buf);
		char* getCLenta(char* buf, short pos, short n = 25);
		char* getDiagnosis(short n, char* buf);

		bool savestate();
		bool resetstate();
		bool push_chain(GRB::Rule::Chain chain);

		RC_STEP step();
		bool start();
		bool savedDiagnosis(RC_STEP prc_step);

		void printRules();

		struct Deducation
		{
			short size;
			short* nrules;
			short* nrulechains;

			Deducation()
			{
				this->size = 0;
				this->nrules = 0;
				this->nrulechains = 0;
			}
		} deducation;

		bool savededucation();

		ofstream* pars;
	};
}