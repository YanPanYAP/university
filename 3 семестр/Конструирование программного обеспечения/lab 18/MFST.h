#pragma once
#include "stdafx.h"

// ќпредел€ем класс my_stack_SHORT, который наследует стандартный стек дл€ работы с короткими целыми числами
class my_stack_SHORT :public std::stack<short> {
public:
	using std::stack<short>::c; // стек автомата
};

#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

static int FST_TRACE_n = -1;
static char rbuf[205], sbuf[205], lbuf[1024];



#define MFST_TRACE_START std::cout<< std::setw(4)<<std::left<<"Ўаг"<<":"\
								  << std::setw(20)<<std::left<<" ѕравило"\
								  << std::setw(30)<<std::left<<" ¬ходна€ лента"\
								  << std::setw(20)<<std::left<<" —тек"\
								  << std::endl;

// ћакрос дл€ записи информации о текущем шаге и правиле
#define MFST_TRACE1		 std::cout<< std::setw(4)<<std::left<<++FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<rule.getCRule(rbuf,nrulechain)\
								  << std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
								  << std::setw(20)<<std::left<<getCSt(sbuf)\
								  << std::endl;

// ћакрос дл€ записи информации о текущем шаге без указани€ правила
#define MFST_TRACE2		 std::cout<< std::setw(4)<<std::left<<FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<" "\
								  << std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
								  << std::setw(20)<<std::left<<getCSt(sbuf)\
								  << std::endl;

// ћакрос дл€ записи информации о шаге с увеличением счетчика
#define MFST_TRACE3		 std::cout<< std::setw(4)<<std::left<<++FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<" "\
								  << std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
								  << std::setw(20)<<std::left<<getCSt(sbuf)\
								  << std::endl;

// ћакрос дл€ записи трассировки с переданным сообщением
#define MFST_TRACE4(c)		std::cout<<std::setw(4)<<std::left << ++FST_TRACE_n << ": "<<std::setw(20)<< std::left <<c<<std::endl;

// ћакрос дл€ записи трассировки без увеличени€ счетчика и с переданным сообщением
#define MFST_TRACE5(c)		std::cout<<std::setw(4)<<std::left << FST_TRACE_n << ": "<<std::setw(20)<< std::left <<c<<std::endl;

// ћакрос дл€ записи трассировки с переданным сообщением и счетчиком
#define MFST_TRACE6(c,k)	std::cout<<std::setw(4)<<std::left << FST_TRACE_n << ": "<<std::setw(20)<< std::left << c << k <<std::endl;

// ћакрос дл€ записи состо€ни€ стека в трассировке
#define MFST_TRACE7			std::cout<<std::setw(4)<<std::left << state.lenta_position << ": "\
							<<std::setw(20)<< std::left << rule.getCRule(rbuf,state.nrulechain)\
							<<std::endl;

// ќпредел€ем тип дл€ стека состо€ний автомата
typedef my_stack_SHORT MFSTSTSTACK;

namespace MFST
{
	struct MfstState					//состо€ние автомата дл€ сохранени€
	{
		short lenta_position;			//состо€ние автомата дл€ сохранени€
		short nrule;					//номер текущего правила
		short nrulechain;				//номер текущей цепчки, текущего правила
		MFSTSTSTACK st;					//стек автомата
		MfstState();
		MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain);	//(позици€ на ленте;стек автомата; номер текущей цепочки текущего правила)
		MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain);//(позици€ на ленте;стек автомата; номер текущего правила; номер текущей цепочки текущего правила)

	};
	class my_stack_MfstState :public std::stack<MfstState> {
	public:
		using std::stack<MfstState>::c;
	};
	struct Mfst							//магазинный автомат
	{
		enum RC_STEP {					//код вовзрата функции step
			NS_OK,					//найдено правило и цепочка, цепочка записана в стек
			NS_NORULE,				//не найдено правило грамматики(ошибка в грамматике)
			NS_NORULECHAIN,			//не найдена подход€ща€ цепочка правила(ошибка в исходном коде)
			NS_ERROR,				//неизвествный нетерминальный символ грамматики
			TS_OK,					//тек. символ ленты == вершине стека, продвинулась лента, pop стека
			TS_NOK,					//тек. символ ленты != вершине стека, продвинулась лента, pop стека
			LENTA_END,				//текуща€ позици€ ленты >= lenta_size
			SURPRISE,				//неожиданный код возврата (ошибка в step)
		};
		struct MfstDiagnosis		//диагностика
		{
			short lenta_position;		//позици€ на ленте
			RC_STEP rc_step;			//код завершени€ шага
			short nrule;				//номер правила
			short nrule_chain;			//номер цепочки правила
			MfstDiagnosis();			//==
			MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain);

		} diagnosis[MFST_DIAGN_NUMBER]; 		// последние самые глубокие сообщени€

		GRBALPHABET* lenta;					//перекодированны€ (TN/NS) лента (из LEX)
		short lenta_position;				//текуща€ позици€ на ленте
		short nrule;						//номер текущего правила
		short nrulechain;					//номер текущей цепочки,текущего правила
		short lenta_size;					//размер ленты
		GRB::Greibach grebach;				//грамматика √рейбах
		LT::LexTable lex;					//результат работы лексического анализатора
		MFSTSTSTACK st;						//стек автомата
		my_stack_MfstState storestate;		//стек дл€ хранени€ состо€ний
		Mfst();
		Mfst(LT::LexTable& plex, GRB::Greibach pgrebach);
		char* getCSt(char* buf);			//получить содержимое стека
		char* getCLenta(char* buf, short pos, short n = 25);	//лента: n символов с pos
		char* getDiagnosis(short n, char* buf);					//получить n-ю строку диагностики или 0х00
		bool savestate();					//сохранить состо€ние автомата
		bool resetstate();					//восстановить состо€ние автомата
		bool push_chain(GRB::Rule::Chain chain);		//поместить цепочку правила в стек
		RC_STEP step();						//выполнить шаг автомата
		bool start();						//запустить автомат
		bool savediagnosis(RC_STEP pprc_step);			//код завершени€ шага
		void printrules();					//вывести послдеовательность правил


		// —труктура дл€ хранени€ информации о дедукции
		struct Deducation
		{
			short size;
			short* nrules;
			short* nrulechains;
			Deducation()
			{
				size = 0;
				nrules = 0;
				nrulechains = 0;
			};
		}deducation;

		bool savededucation();
	};
}
