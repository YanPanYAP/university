#include"stdafx.h"
#include"SemAn.h"
#include"Gen.h"

int _tmain(int argc, TCHAR* argv[]) {
	setlocale(LC_CTYPE, "ru_RU.UTF-8");
	Out::OUT out = Out::INITOUT;
	Log::LOG log = Log::INITLOG;
	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		In::IN in = In::getin(parm.in);
		out = Out::getOut(parm.out);
		Out::WriteOut(out, in);
		log = Log::getlog(parm.log);
		Log::WriteLine(log, (char*)"Текст:", (char*)" без ошибок\n", "");
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Log::WriteIn(log, in);

		Lex::Tables tables = Lex::LexAnalizer(in, parm);
		MFST::Mfst mfs();
		MFST::Mfst mfst(tables.lextable, GRB::getGreibach(), parm.greibach);

		bool checkSynt = mfst.start();
		mfst.savededucation();
		mfst.printRules();
		if (!checkSynt) {
			cout << "Ошибка в ходе синтаксического анализа.\n";
			return 0;
		}
		
		bool SemanticOk = Semantic::semanticsCheck(tables, log);
		if (!SemanticOk) {
			cout << "Ошибка в ходе семантического анализа.\n";
		}
		std::cout << "Лексический анализ выполнен без ошибок." << std::endl;
		std::cout << "Синтаксический анализ выполнен без ошибок." << std::endl;
		std::cout << "Семантический анализ выполнен без ошибок." << std::endl;

		LT::showTable(tables.lextable); std::cout << std::endl;
		IT::showTable(tables.idtable);
		Generator::CodeGeneration(tables);
		Out::Close(out);
		Log::Close(log);
	}
	catch (Error::ERROR e) {
		std::cerr << "\nОшибка " << e.id
			<< ": " << e.message
			<< "\nстрока " << (e.inext.line != -1 ? e.inext.line : 0) << std::endl;
		Log::WriteError(log, e);
	}
}