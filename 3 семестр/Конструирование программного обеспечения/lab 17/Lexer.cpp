#include "stdafx.h"
namespace  Lexer
{

	FST::FST typeInteger("", 8,
		FST::NODE(1, FST::RELATION('i', 1)),
		FST::NODE(1, FST::RELATION('n', 2)),
		FST::NODE(1, FST::RELATION('t', 3)),
		FST::NODE(1, FST::RELATION('e', 4)),
		FST::NODE(1, FST::RELATION('g', 5)),
		FST::NODE(1, FST::RELATION('e', 6)),
		FST::NODE(1, FST::RELATION('r', 7)),
		FST::NODE()
	);

	FST::FST typeString("", 7,
		FST::NODE(1, FST::RELATION('s', 1)),
		FST::NODE(1, FST::RELATION('t', 2)),
		FST::NODE(1, FST::RELATION('r', 3)),
		FST::NODE(1, FST::RELATION('i', 4)),
		FST::NODE(1, FST::RELATION('n', 5)),
		FST::NODE(1, FST::RELATION('g', 6)),
		FST::NODE()
	);

	FST::FST typeFunction("", 9,
		FST::NODE(1, FST::RELATION('f', 1)),
		FST::NODE(1, FST::RELATION('u', 2)),
		FST::NODE(1, FST::RELATION('n', 3)),
		FST::NODE(1, FST::RELATION('c', 4)),
		FST::NODE(1, FST::RELATION('t', 5)),
		FST::NODE(1, FST::RELATION('i', 6)),
		FST::NODE(1, FST::RELATION('o', 7)),
		FST::NODE(1, FST::RELATION('n', 8)),
		FST::NODE()
	);

	FST::FST typeDeclare("", 8,
		FST::NODE(1, FST::RELATION('d', 1)),
		FST::NODE(1, FST::RELATION('e', 2)),
		FST::NODE(1, FST::RELATION('c', 3)),
		FST::NODE(1, FST::RELATION('l', 4)),
		FST::NODE(1, FST::RELATION('a', 5)),
		FST::NODE(1, FST::RELATION('r', 6)),
		FST::NODE(1, FST::RELATION('e', 7)),
		FST::NODE()
	);

	FST::FST typeReturn("", 7,
		FST::NODE(1, FST::RELATION('r', 1)),
		FST::NODE(1, FST::RELATION('e', 2)),
		FST::NODE(1, FST::RELATION('t', 3)),
		FST::NODE(1, FST::RELATION('u', 4)),
		FST::NODE(1, FST::RELATION('r', 5)),
		FST::NODE(1, FST::RELATION('n', 6)),
		FST::NODE()
	);

	FST::FST typeMain("", 5,
		FST::NODE(1, FST::RELATION('m', 1)),
		FST::NODE(1, FST::RELATION('a', 2)),
		FST::NODE(1, FST::RELATION('i', 3)),
		FST::NODE(1, FST::RELATION('n', 4)),
		FST::NODE()
	);

	FST::FST typePrint("", 6,
		FST::NODE(1, FST::RELATION('p', 1)),
		FST::NODE(1, FST::RELATION('r', 2)),
		FST::NODE(1, FST::RELATION('i', 3)),
		FST::NODE(1, FST::RELATION('n', 4)),
		FST::NODE(1, FST::RELATION('t', 5)),
		FST::NODE()
	);

	FST::FST typeSpace("", 2,
		FST::NODE(1, FST::RELATION(' ', 1)),
		FST::NODE()
	);
	FST::FST typeLeftBrace("", 2,
		FST::NODE(1, FST::RELATION('{', 1)),
		FST::NODE()
	);
	FST::FST typeRightBrace("", 2,
		FST::NODE(1, FST::RELATION('}', 1)),
		FST::NODE()
	);
	FST::FST typeLeftThesis("", 2,
		FST::NODE(1, FST::RELATION('(', 1)),
		FST::NODE()
	);
	FST::FST typeRightThesis("", 2,
		FST::NODE(1, FST::RELATION(')', 1)),
		FST::NODE()
	);
	FST::FST typeSemicolon("", 2,
		FST::NODE(1, FST::RELATION(';', 1)),
		FST::NODE()
	);
	FST::FST typeComma("", 2,
		FST::NODE(1, FST::RELATION(',', 1)),
		FST::NODE()
	);
	FST::FST typePlus("", 2,
		FST::NODE(1, FST::RELATION('+', 1)),
		FST::NODE());
	FST::FST typeMinus("", 2,
		FST::NODE(1, FST::RELATION('-', 1)),
		FST::NODE());
	FST::FST typeDel("", 2,
		FST::NODE(1, FST::RELATION('/', 1)),
		FST::NODE());
	FST::FST typeMulti("", 2,
		FST::NODE(1, FST::RELATION('*', 1)),
		FST::NODE());
	FST::FST typeEquals("", 2,
		FST::NODE(1, FST::RELATION('=', 1)),
		FST::NODE());
	//обработка индентификторов алфавита
	FST::FST typeIdentificator("", 2,
		FST::NODE(52,
			FST::RELATION('a', 0), FST::RELATION('b', 0),
			FST::RELATION('c', 0), FST::RELATION('d', 0),
			FST::RELATION('e', 0), FST::RELATION('f', 0),
			FST::RELATION('g', 0), FST::RELATION('h', 0),
			FST::RELATION('i', 0), FST::RELATION('j', 0),
			FST::RELATION('k', 0), FST::RELATION('l', 0),
			FST::RELATION('m', 0), FST::RELATION('n', 0),
			FST::RELATION('o', 0), FST::RELATION('p', 0),
			FST::RELATION('q', 0), FST::RELATION('r', 0),
			FST::RELATION('s', 0), FST::RELATION('t', 0),
			FST::RELATION('u', 0), FST::RELATION('v', 0),
			FST::RELATION('w', 0), FST::RELATION('x', 0),
			FST::RELATION('y', 0), FST::RELATION('z', 0),
			FST::RELATION('a', 1), FST::RELATION('b', 1),
			FST::RELATION('c', 1), FST::RELATION('d', 1),
			FST::RELATION('e', 1), FST::RELATION('f', 1),
			FST::RELATION('g', 1), FST::RELATION('h', 1),
			FST::RELATION('i', 1), FST::RELATION('j', 1),
			FST::RELATION('k', 1), FST::RELATION('l', 1),
			FST::RELATION('m', 1), FST::RELATION('n', 1),
			FST::RELATION('o', 1), FST::RELATION('p', 1),
			FST::RELATION('q', 1), FST::RELATION('r', 1),
			FST::RELATION('s', 1), FST::RELATION('t', 1),
			FST::RELATION('u', 1), FST::RELATION('v', 1),
			FST::RELATION('w', 1), FST::RELATION('x', 1),
			FST::RELATION('y', 1), FST::RELATION('z', 1)
		),

		FST::NODE()
	);


	FST::FST typeStringLiteral("", 4,
		//обработка комментариев
		FST::NODE(1,
			FST::RELATION('\'', 1)),
		FST::NODE(384,
			FST::RELATION('!', 2), FST::RELATION('@', 2), FST::RELATION('#', 2), FST::RELATION('$', 2), FST::RELATION('%', 2), FST::RELATION('^', 2),
			FST::RELATION('&', 2), FST::RELATION('*', 2), FST::RELATION('(', 2), FST::RELATION(')', 2), FST::RELATION('-', 2), FST::RELATION('_', 2),
			FST::RELATION('+', 2), FST::RELATION('=', 2), FST::RELATION('~', 2), FST::RELATION('`', 2), FST::RELATION('1', 2), FST::RELATION('2', 2),
			FST::RELATION('3', 2), FST::RELATION('4', 2), FST::RELATION('5', 2), FST::RELATION('6', 2), FST::RELATION('7', 2), FST::RELATION('8', 2),
			FST::RELATION('9', 2), FST::RELATION('0', 2), FST::RELATION('	', 2), FST::RELATION(' ', 2), FST::RELATION('q', 2), FST::RELATION('w', 2),
			FST::RELATION('e', 2), FST::RELATION('r', 2), FST::RELATION('t', 2), FST::RELATION('y', 2), FST::RELATION('u', 2), FST::RELATION('i', 2),
			FST::RELATION('o', 2), FST::RELATION('p', 2), FST::RELATION('{', 2), FST::RELATION('[', 2), FST::RELATION('}', 2), FST::RELATION(']', 2),
			FST::RELATION('\\', 2), FST::RELATION('|', 2), FST::RELATION('Q', 2), FST::RELATION('W', 2), FST::RELATION('R', 2), FST::RELATION('T', 2), FST::RELATION('Y', 2),
			FST::RELATION('U', 2), FST::RELATION('I', 2), FST::RELATION('O', 2), FST::RELATION('P', 2), FST::RELATION('a', 2), FST::RELATION('s', 2),
			FST::RELATION('d', 2), FST::RELATION('f', 2), FST::RELATION('g', 2), FST::RELATION('h', 2), FST::RELATION('j', 2), FST::RELATION('k', 2),
			FST::RELATION('l', 2), FST::RELATION(':', 2), FST::RELATION(';', 2), FST::RELATION('\'', 2), FST::RELATION('"', 2), FST::RELATION('A', 2),
			FST::RELATION('S', 2), FST::RELATION('D', 2), FST::RELATION('F', 2), FST::RELATION('G', 2), FST::RELATION('H', 2), FST::RELATION('J', 2),
			FST::RELATION('K', 2), FST::RELATION('L', 2), FST::RELATION('z', 2), FST::RELATION('x', 2), FST::RELATION('c', 2), FST::RELATION('v', 2),
			FST::RELATION('b', 2), FST::RELATION('n', 2), FST::RELATION('m', 2), FST::RELATION(',', 2), FST::RELATION('.', 2), FST::RELATION('/', 2),
			FST::RELATION('Z', 2), FST::RELATION('X', 2), FST::RELATION('C', 2), FST::RELATION('V', 2), FST::RELATION('B', 2), FST::RELATION('N', 2),
			FST::RELATION('M', 2), FST::RELATION('<', 2), FST::RELATION('>', 2), FST::RELATION('?', 2), FST::RELATION('ё', 2), FST::RELATION('Ё', 2),
			FST::RELATION('й', 2), FST::RELATION('ц', 2), FST::RELATION('у', 2), FST::RELATION('к', 2), FST::RELATION('е', 2), FST::RELATION('н', 2),
			FST::RELATION('г', 2), FST::RELATION('ш', 2), FST::RELATION('щ', 2), FST::RELATION('з', 2), FST::RELATION('х', 2), FST::RELATION('ъ', 2),
			FST::RELATION('Й', 2), FST::RELATION('Ц', 2), FST::RELATION('У', 2), FST::RELATION('К', 2), FST::RELATION('Е', 2), FST::RELATION('Н', 2),
			FST::RELATION('Г', 2), FST::RELATION('Ш', 2), FST::RELATION('Щ', 2), FST::RELATION('З', 2), FST::RELATION('Х', 2), FST::RELATION('Ъ', 2),
			FST::RELATION('ф', 2), FST::RELATION('ы', 2), FST::RELATION('в', 2), FST::RELATION('а', 2), FST::RELATION('п', 2), FST::RELATION('р', 2),
			FST::RELATION('о', 2), FST::RELATION('л', 2), FST::RELATION('д', 2), FST::RELATION('ж', 2), FST::RELATION('э', 2), FST::RELATION('Ф', 2),
			FST::RELATION('Ы', 2), FST::RELATION('В', 2), FST::RELATION('А', 2), FST::RELATION('П', 2), FST::RELATION('Р', 2), FST::RELATION('О', 2),
			FST::RELATION('Л', 2), FST::RELATION('Д', 2), FST::RELATION('Ж', 2), FST::RELATION('Э', 2), FST::RELATION('я', 2), FST::RELATION('ч', 2),
			FST::RELATION('с', 2), FST::RELATION('м', 2), FST::RELATION('и', 2), FST::RELATION('т', 2), FST::RELATION('ь', 2), FST::RELATION('б', 2),
			FST::RELATION('ю', 2), FST::RELATION('Я', 2), FST::RELATION('Ч', 2), FST::RELATION('С', 2), FST::RELATION('М', 2), FST::RELATION('И', 2),
			FST::RELATION('Т', 2), FST::RELATION('Ь', 2), FST::RELATION('Б', 2), FST::RELATION('Ю', 2),
			FST::RELATION('!', 1), FST::RELATION('@', 1), FST::RELATION('#', 1), FST::RELATION('$', 1), FST::RELATION('%', 1), FST::RELATION('^', 1),
			FST::RELATION('&', 1), FST::RELATION('*', 1), FST::RELATION('(', 1), FST::RELATION(')', 1), FST::RELATION('-', 1), FST::RELATION('_', 1),
			FST::RELATION('+', 1), FST::RELATION('=', 1), FST::RELATION('~', 1), FST::RELATION('`', 1), FST::RELATION('1', 1), FST::RELATION('2', 1),
			FST::RELATION('3', 1), FST::RELATION('4', 1), FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1),
			FST::RELATION('9', 1), FST::RELATION('0', 1), FST::RELATION('	', 1), FST::RELATION(' ', 1), FST::RELATION('q', 1), FST::RELATION('w', 1),
			FST::RELATION('e', 1), FST::RELATION('r', 1), FST::RELATION('t', 1), FST::RELATION('y', 1), FST::RELATION('u', 1), FST::RELATION('i', 1),
			FST::RELATION('o', 1), FST::RELATION('p', 1), FST::RELATION('{', 1), FST::RELATION('[', 1), FST::RELATION('}', 1), FST::RELATION(']', 1),
			FST::RELATION('\\', 1), FST::RELATION('|', 1), FST::RELATION('Q', 1), FST::RELATION('W', 1), FST::RELATION('R', 1), FST::RELATION('T', 1), FST::RELATION('Y', 1),
			FST::RELATION('U', 1), FST::RELATION('I', 1), FST::RELATION('O', 1), FST::RELATION('P', 1), FST::RELATION('a', 1), FST::RELATION('s', 1),
			FST::RELATION('d', 1), FST::RELATION('f', 1), FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('j', 1), FST::RELATION('k', 1),
			FST::RELATION('l', 1), FST::RELATION(':', 1), FST::RELATION(';', 1), FST::RELATION('\'', 1), FST::RELATION('"', 1), FST::RELATION('A', 1),
			FST::RELATION('S', 1), FST::RELATION('D', 1), FST::RELATION('F', 1), FST::RELATION('G', 1), FST::RELATION('H', 1), FST::RELATION('J', 1),
			FST::RELATION('K', 1), FST::RELATION('L', 1), FST::RELATION('z', 1), FST::RELATION('x', 1), FST::RELATION('c', 1), FST::RELATION('v', 1),
			FST::RELATION('b', 1), FST::RELATION('n', 1), FST::RELATION('m', 1), FST::RELATION(',', 1), FST::RELATION('.', 1), FST::RELATION('/', 1),
			FST::RELATION('Z', 1), FST::RELATION('X', 1), FST::RELATION('C', 1), FST::RELATION('V', 1), FST::RELATION('B', 1), FST::RELATION('N', 1),
			FST::RELATION('M', 1), FST::RELATION('<', 1), FST::RELATION('>', 1), FST::RELATION('?', 1), FST::RELATION('ё', 1), FST::RELATION('Ё', 1),
			FST::RELATION('й', 1), FST::RELATION('ц', 1), FST::RELATION('у', 1), FST::RELATION('к', 1), FST::RELATION('е', 1), FST::RELATION('н', 1),
			FST::RELATION('г', 1), FST::RELATION('ш', 1), FST::RELATION('щ', 1), FST::RELATION('з', 1), FST::RELATION('х', 1), FST::RELATION('ъ', 1),
			FST::RELATION('Й', 1), FST::RELATION('Ц', 1), FST::RELATION('У', 1), FST::RELATION('К', 1), FST::RELATION('Е', 1), FST::RELATION('Н', 1),
			FST::RELATION('Г', 1), FST::RELATION('Ш', 1), FST::RELATION('Щ', 1), FST::RELATION('З', 1), FST::RELATION('Х', 1), FST::RELATION('Ъ', 1),
			FST::RELATION('ф', 1), FST::RELATION('ы', 1), FST::RELATION('в', 1), FST::RELATION('а', 1), FST::RELATION('п', 1), FST::RELATION('р', 1),
			FST::RELATION('о', 1), FST::RELATION('л', 1), FST::RELATION('д', 1), FST::RELATION('ж', 1), FST::RELATION('э', 1), FST::RELATION('Ф', 1),
			FST::RELATION('Ы', 1), FST::RELATION('В', 1), FST::RELATION('А', 1), FST::RELATION('П', 1), FST::RELATION('Р', 1), FST::RELATION('О', 1),
			FST::RELATION('Л', 1), FST::RELATION('Д', 1), FST::RELATION('Ж', 1), FST::RELATION('Э', 1), FST::RELATION('я', 1), FST::RELATION('ч', 1),
			FST::RELATION('с', 1), FST::RELATION('м', 1), FST::RELATION('и', 1), FST::RELATION('т', 1), FST::RELATION('ь', 1), FST::RELATION('б', 1),
			FST::RELATION('ю', 1), FST::RELATION('Я', 1), FST::RELATION('Ч', 1), FST::RELATION('С', 1), FST::RELATION('М', 1), FST::RELATION('И', 1),
			FST::RELATION('Т', 1), FST::RELATION('Ь', 1), FST::RELATION('Б', 1), FST::RELATION('Ю', 1)
		),
		FST::NODE(1,
			FST::RELATION('\'', 3)),
		FST::NODE()
	);

	FST::FST typeNumbLiteral("", 3,
		FST::NODE(20,
			FST::RELATION('0', 2), FST::RELATION('1', 2), FST::RELATION('2', 2), FST::RELATION('3', 2), FST::RELATION('4', 2),
			FST::RELATION('5', 2), FST::RELATION('6', 2), FST::RELATION('7', 2), FST::RELATION('8', 2), FST::RELATION('9', 2),
			FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),
			FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1)),

		FST::NODE(20,
			FST::RELATION('0', 2), FST::RELATION('1', 2), FST::RELATION('2', 2), FST::RELATION('3', 2), FST::RELATION('4', 2),
			FST::RELATION('5', 2), FST::RELATION('6', 2), FST::RELATION('7', 2), FST::RELATION('8', 2), FST::RELATION('9', 2),
			FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),
			FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1)),

		FST::NODE()

	);

	// Функция для проверки строки s с использованием заданного конечного автомата fst
	bool check(std::string s, FST::FST fst)
	{
		// Устанавливаем строку внутри конечного автомата fst
		fst.string = s;

		// Запускаем выполнение конечного автомата и возвращаем результат
		return FST::execute(fst);
	}


	// Объявление функции getIdentifierContext, которая принимает лексическую таблицу и текущий индекс
	std::string getIdentifierContext(const LT::LexTable& lextable, int currentIndex)
	{
		std::string context;
		std::string buffer;
		std::string buff;
		std::string type;

		if (context.empty())
		{
			for (int i = currentIndex - 1; i >= 0; i--)
			{
				if (lextable.table[i].lexema == 't' && lextable.table[i + 1].comm == lextable.table[currentIndex - 1].comm)
				{
					if (lextable.table[i].comm == "integer")
					{
						type += "_integer_";
					}
					if (lextable.table[i].comm == "string")
					{
						type += "_string_";
					}

					break;
				}
			}

			for (int i = currentIndex - 1; i >= 0; i--)
			{
				const LT::Entry& entry = lextable.table[i];
				if (entry.lexema == LEX_FUNCTION)
				{
					buffer += "function";

					if (lextable.table[i - 1].comm == "integer")
					{
						type += "_integer_";
					}
					if (lextable.table[i - 1].comm == "string") {
						type += "_string_";
					}

					// Получение имени функции и добавление его в контекст
					buff = lextable.table[i + 1].comm;
					buffer += type;
					buffer += buff;

					type.clear();
					buff.clear();

					break;
				}
				else if (entry.lexema == LEX_LEFTBRACE)
				{
					for (int j = i; j >= 0; j--)
					{
						if (lextable.table[j].lexema == LEX_FUNCTION)
						{
							if (lextable.table[j].comm == LIB_FUNCTION1 ||
								lextable.table[j].comm == LIB_FUNCTION2)
							{
								continue;
							}

							buff = lextable.table[j + 1].comm;
							buffer += "var_function";

							break;
						}

						if (lextable.table[j].lexema == LEX_MAINFUNC)
						{
							buffer += "var_main";

							break;
						}
					}

					// Добавление типа и имени в контекст
					buffer += type;
					buffer += buff;

					type.clear();
					buff.clear();

					break;
				}
				else if (entry.lexema == LEX_LEFTHESIS)
				{
					// Проверка наличия условий для игнорирования
					if (lextable.table[i - 1].lexema == LEX_RAV || lextable.table[i - 1].lexema == 'v' || lextable.table[i + 1].lexema == 'v')
					{
						// Пропуск
						;
					}

					for (int j = i; j >= 0 && j != LEX_RIGHTBRACE; j--)
					{
						// Проверка, что буфер пуст и текущая лексема - это идентификатор (LEX_ID)
						if (buffer.empty())
						{
							if (lextable.table[j].lexema == LEX_ID && lextable.table[j - 1].lexema == LEX_FUNCTION)
							{
								buff = lextable.table[j + 1].comm;
								buffer += "parameter_function";

								break;
							}
						}

						if (buffer.empty())
						{
							if (lextable.table[j].lexema == LEX_LEFTBRACE)
							{
								for (int k = j; k >= 0; k--)
								{
									if (lextable.table[k].lexema == LEX_FUNCTION)
									{
										if (lextable.table[k].comm == LIB_FUNCTION1 ||
											lextable.table[k].comm == LIB_FUNCTION2)
										{
											continue;
										}

										buff = lextable.table[k + 1].comm;
										buffer += "var_function";

										break;
									}

									if (lextable.table[k].lexema == LEX_MAINFUNC)
									{
										buffer += "var_main";

										break;
									}
								}
							}
						}
					}

					buffer += type;
					buffer += buff;

					type.clear();
					buff.clear();

					break;
				}
			}
		}

		// Присвоение полученного контекста переменной context
		context = buffer;

		return context;
	}


	// Функция fillFields принимает запись idEntry и контекст и заполняет поля записи на основе контекста
	void fillFields(IT::Entry& idEntry, std::string context)
	{
		if (context.find("function_string_") == 0)
		{
			idEntry.functionname = idEntry.id;
			idEntry.idtype = IT::IDTYPE::F;
			idEntry.iddatatype = IT::IDDATATYPE::STR;
		}
		else if (context.find("function_integer_") == 0)
		{
			idEntry.functionname = idEntry.id;
			idEntry.idtype = IT::IDTYPE::F;
			idEntry.iddatatype = IT::IDDATATYPE::INT;
		}
		else if (context.find("parameter_function") == 0)
		{
			idEntry.idtype = IT::IDTYPE::P;
			if (context.find("string") != std::string::npos)
			{
				idEntry.iddatatype = IT::IDDATATYPE::STR;
				idEntry.value.setString("");
			}
			else if (context.find("integer") != std::string::npos)
			{
				idEntry.iddatatype = IT::IDDATATYPE::INT;
				idEntry.value.setInteger(0);
			}
			// Извлечение имени функции из контекста
			std::string buff;
			size_t lastUnderscorePos = context.rfind("_");
			if (lastUnderscorePos != std::string::npos)
			{
				buff = context.substr(lastUnderscorePos + 1);
			}
			idEntry.fun = buff;
		}
		else if (context.find("var_main") == 0)
		{
			idEntry.idtype = IT::IDTYPE::V;
			if (context.find("string") != std::string::npos)
			{
				idEntry.iddatatype = IT::IDDATATYPE::STR;
				idEntry.value.setString("");
			}
			if (context.find("integer") != std::string::npos)
			{
				idEntry.iddatatype = IT::IDDATATYPE::INT;
				idEntry.value.setInteger(0);
			}
			idEntry.fun = "main";
		}
		else if (context.find("var_function") == 0)
		{
			idEntry.idtype = IT::IDTYPE::V;
			if (context.find("string") != std::string::npos)
			{
				idEntry.iddatatype = IT::IDDATATYPE::STR;
				idEntry.value.setString("");
			}
			if (context.find("integer") != std::string::npos)
			{
				idEntry.iddatatype = IT::IDDATATYPE::INT;
				idEntry.value.setInteger(0);
			}
			// Извлечение имени функции из контекста
			std::string buff;
			size_t lastUnderscorePos = context.rfind("_");
			if (lastUnderscorePos != std::string::npos)
			{
				buff = context.substr(lastUnderscorePos + 1);
			}
			idEntry.fun = buff;
		}
	}


	// Функция addIdentificator добавляет идентификатор в таблицу лексем (lextable) и таблицу идентификаторов (idtable)
	void addIdentificator(LT::LexTable& lextable, IT::IdTable& idtable, std::string word, LT::Entry& lexEntry, int line)
	{
		// Устанавливаем лексему как LEX_ID и устанавливаем номер строки
		lexEntry.lexema = LEX_ID;
		lexEntry.sn = line;

		// Если идентификатор слишком длинный, укорачиваем его до максимальной длины ID_MAXSIZE
		if (word.length() > ID_MAXSIZE && (word != LIB_FUNCTION1 && word != LIB_FUNCTION2))
		{
			word = word.substr(0, ID_MAXSIZE);
		}

		// Устанавливаем значение комментария в лексеме
		lexEntry.comm = word;

		LT::Add(lextable, lexEntry);
		
		int size = lextable.size - 1;

		IT::Entry idEntry;

		// Получаем контекст для идентификатора
		std::string context = getIdentifierContext(lextable, lextable.size);

		// Ищем идентификатор в таблице идентификаторов
		int idIndex = IsId(idtable, word, idEntry.functionname, idEntry.fun);

		if (idIndex != TI_NULLIDX)
		{
			idEntry.id = word;

			// Заполняем поля записи идентификатора на основе контекста
			fillFields(idEntry, context);

			if (idtable.table[idIndex].idtype == 4)
			{
				// Проверяем условия для присваивания значения в зависимости от контекста
				bool condition1 = ((lextable.table[size - 1].lexema == LEX_LEFTHESIS && lextable.table[size - 2].lexema == LEX_ID)
					|| (lextable.table[size - 1].lexema == LEX_COMMA && lextable.table[size - 2].lexema == LEX_ID) || lextable.table[size - 1].lexema == LEX_SEMICOLON
					|| lextable.table[size - 1].lexema == LEX_PRINT || lextable.table[size - 1].lexema == LEX_RETURN);
				bool condition2 = (idtable.table[idIndex].iddatatype == idEntry.iddatatype &&
					idtable.table[idIndex].idtype == idEntry.idtype && idtable.table[idIndex].fun == idEntry.fun);

				// Если не выполняются оба условия, добавляем новую запись в таблицу идентификаторов
				if (!condition1)
				{
					if (!condition2)
					{
						idEntry.idxfirstLE = size;
						lextable.table[size].idxTI = idtable.size;

						IT::Add(idtable, idEntry);
					}
				}
				// Иначе устанавливаем индекс из существующей записи в таблице идентификаторов
				else
				{
					int bufIndex = idtable.table[idIndex].idxfirstLE;
					lextable.table[size].idxTI = lextable.table[bufIndex].idxTI;
				}
			}
			else if (idtable.table[idIndex].idtype == 3)
			{
				if (idtable.table[idIndex].fun != idEntry.fun)
				{
					idEntry.idxfirstLE = size;
					lextable.table[size].idxTI = idtable.size;

					IT::Add(idtable, idEntry);
				}
				else
				{
					int bufIndex = idtable.table[idIndex].idxfirstLE;
					lextable.table[size].idxTI = lextable.table[bufIndex].idxTI;
				}
			}
			else
			{
				int bufIndex = idtable.table[idIndex].idxfirstLE;
				lextable.table[size].idxTI = lextable.table[bufIndex].idxTI;
			}
		}
		// Если идентификатор не существует в таблице идентификаторов
		else
		{
			idEntry.id = word;
			fillFields(idEntry, context);

			// Проверяем тип идентификатора и добавляем запись в таблицу идентификаторов
			if (idEntry.iddatatype == IT::IDDATATYPE::STR || idEntry.iddatatype == IT::IDDATATYPE::INT)
			{
				idEntry.idxfirstLE = size;
				lextable.table[size].idxTI = idtable.size;

				IT::Add(idtable, idEntry);
			}
		}
	}



	void parse(In::IN in, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		// Создаем переменные для лексемы и идентификатора
		LT::Entry lex;
		IT::Entry idn;

		// Строка, содержащая разделители
		std::string separators = "();{}=*/+-|, ";
		
		std::string buff = "";// Буфер для хранения текущей строки из входных данных
		
		bool insideString = false;// Флаг, указывающий, находится ли парсер внутри строки (внутри одинарных кавычек)
		
		int numbMarks = 0;// Счетчик открывающихся кавычек
		int line = 0;
		int pos = 0;
		
		int count = 0;// Счетчик для литералов

		// Проходим по строкам во входных данных
		for (line = 0; line < in.text.size(); line++)
		{
			// Получаем текущую строку
			buff = in.text[line];
			// Переменная для хранения текущего слова (лексемы)
			std::string word = "";
			// Проходим по символам в текущей строке
			for (pos = 0; pos < buff.size(); pos++)
			{
				if (insideString)
				{
					if (buff[pos] == '\'')
					{
						insideString = false;
						// Убираем кавычки из строки
						word += (unsigned char)buff[pos];
						word.erase(0, 1);
						word.erase(word.length() - 1, 1);
						// Проверяем, существует ли такая строка в таблице идентификаторов
						int i = IT::findLexStr(idtable, word);
						// Заполняем лексему и добавляем ее в таблицу лексем
						lex.lexema = LEX_LITERAL;
						lex.sn = line;
						lex.comm = word;
						if (i != LT_TI_NULLIDX)
						{
							lex.idxTI = i;
						}

						else
						{
							idn.id = "L" + std::to_string(count);
							idn.iddatatype = IT::STR;
							idn.idtype = IT::L;
							idn.idxfirstLE = lextable.size;
							lex.idxTI = idtable.size;
							idn.value.setString(word);
							count++;
							IT::Add(idtable, idn);
						}

						LT::Add(lextable, lex);
					}
					else
					{
						word += (unsigned char)buff[pos];
					}
				}
				else if (buff[pos] == '\'')
				{
					insideString = true;
					word += (unsigned char)buff[pos];
				}
				// Если текущий символ - разделитель и мы не внутри строки
				else if (separators.find(buff[pos]) != std::string::npos && !insideString)
				{
					std::string buffer = "";
					buffer += buff[pos];
					// Обрабатываем разные типы разделителей
					switch (buff[pos]) 
					{
					case ' ':
					{
						break; 
					}
					case '|':
					{
						break; 
					}
					case '-':
					{
						if (check(buffer, typeMinus)) // Проверка, соответствует ли буфер типу "минус"
						{
							lex.lexema = LEX_MINUS; 
							lex.sn = line; 
							lex.idxTI = -1; 
							LT::Add(lextable, lex); 
						}
						else
						{
							throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text);
						}
						break; 
					}
					case '+':
					{
						if (check(buffer, typePlus)) 
						{
							lex.lexema = LEX_PLUS; 
							lex.sn = line; 
							lex.idxTI = -1; 
							LT::Add(lextable, lex);
						}
						else
						{
							throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text);
						}
						break; 
					}

					case '*':
					{
						if (check(buffer, typeMulti)) 
						{
							lex.lexema = LEX_STAR;
							lex.sn = line;
							lex.idxTI = -1; 
							LT::Add(lextable, lex);
						}
						else
						{
							throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text);
						}
						break; 
					}

					case '/':
					{
						if (check(buffer, typeDel))
						{
							lex.lexema = LEX_DIRSLASH; 
							lex.sn = line;
							lex.idxTI = -1; 
							LT::Add(lextable, lex); 
						}
						else
						{
							throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text);
						}
						break; 
					}

					case '(':
					{
						if (check(buffer, typeLeftThesis)) 
						{
							lex.lexema = LEX_LEFTHESIS; 
							lex.sn = line; 
							lex.idxTI = -1; 
							LT::Add(lextable, lex); 
						}
						else
						{
							throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text);
						}
						break; 
					}

					case ')':
					{
						if (check(buffer, typeRightThesis)) 
						{
							lex.lexema = LEX_RIGHTHESIS;
							lex.sn = line; 
							lex.idxTI = -1;
							LT::Add(lextable, lex);
						}
						else
						{
							throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text);
						}
						break; 
					}

					case '{':
					{
						if (check(buffer, typeLeftBrace)) 
						{
							lex.lexema = LEX_LEFTBRACE; 
							lex.sn = line; 
							lex.idxTI = -1; 
							LT::Add(lextable, lex); 
						}
						else
						{
							throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text);
						}
						break; 
					}

					case '}':
					{
						if (check(buffer, typeRightBrace)) 
						{
							lex.lexema = LEX_RIGHTBRACE;
							lex.sn = line; 
							lex.idxTI = -1; 
							LT::Add(lextable, lex); 
						}
						else
						{
							throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text);
						}
						break; 
					}

					case ',':
					{
						if (check(buffer, typeComma)) 
						{
							lex.lexema = LEX_COMMA;
							lex.sn = line; 
							lex.idxTI = -1; 
							LT::Add(lextable, lex);
						}
						else
						{
							throw ERROR_THROW_IN(117, ++line, ++pos, buff, in.text);
						}
						break; 
					}

					case ';':
					{
						if (check(buffer, typeSemicolon)) 
						{
							lex.lexema = LEX_SEMICOLON;
							lex.sn = line; 
							lex.idxTI = -1;
							LT::Add(lextable, lex); 
						}
						else
						{
							throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text);
						}
						break; 
					}

					case '=':
					{
						if (check(buffer, typeEquals)) 
						{
							lex.lexema = LEX_RAV; 
							lex.sn = line; 
							lex.idxTI = -1;
							LT::Add(lextable, lex); 
						}
						else
						{
							throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text);
						}
						break; 
					}

					default:
					{
						throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text);
						break; 
					}
					} 
					buffer.clear(); 
				}
				else
				{
					// Если текущий символ не разделитель, добавляем его к текущей лексеме (слову)
					word += (unsigned char)buff[pos];
					char k = buff[pos + 1];
					if (separators.find(buff[pos + 1]) != std::string::npos)
					{
						switch ((unsigned char)word[0]) // Переключение по первому символу слова
						{
						case 'i':
						{
							if (check(word, typeInteger)) 
							{
								lex.lexema = LEX_INTEGER;
								lex.sn = line; 
								lex.idxTI = -1; 
								lex.comm = word;
								LT::Add(lextable, lex); 

								word.clear(); 
							}
							else if (check(word, typeIdentificator)) // Проверка, является ли слово идентификатором
							{
								addIdentificator(lextable, idtable, word, lex, line); // Добавление идентификатора в таблицы

								word.clear(); 
							}
							else
							{
								throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text); 
							}

							break;
						}
						case 'd':
						{
							if (check(word, typeDeclare)) 
							{
								lex.lexema = LEX_DECLARE;
								lex.sn = line;
								lex.idxTI = -1;
								LT::Add(lextable, lex); 

								word.clear(); 
							}
							else if (check(word, typeIdentificator))
							{
								addIdentificator(lextable, idtable, word, lex, line); 

								word.clear(); 
							}
							else
							{
								throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text); 
							}
							break; 
						}

						case 's':
						{
							if (check(word, typeString)) 
							{
								lex.lexema = LEX_STRING;
								lex.sn = line; 
								lex.idxTI = -1; 
								lex.comm = word;
								LT::Add(lextable, lex); 

								word.clear(); 
							}
							else if (check(word, typeIdentificator)) 
							{
								addIdentificator(lextable, idtable, word, lex, line); 

								word.clear(); 
							}
							else
							{
								throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text); 
							}
							break;
						}
						case 'f':
						{
							if (check(word, typeFunction)) 
							{
								lex.lexema = LEX_FUNCTION;
								lex.sn = line; 
								lex.idxTI = -1; 
								lex.comm = word;
								LT::Add(lextable, lex);

								word.clear(); 
							}
							else if (check(word, typeIdentificator)) 
							{
								addIdentificator(lextable, idtable, word, lex, line);

								word.clear(); 
							}
							else
							{
								throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text); 
							}
							break; 
						}

						case 'm':
						{
							if (check(word, typeMain))
							{
								lex.lexema = LEX_MAINFUNC;
								lex.sn = line; 
								lex.idxTI = -1; 
								lex.comm = word;
								LT::Add(lextable, lex);

								word.clear(); 
							}
							else if (check(word, typeIdentificator)) 
							{
								addIdentificator(lextable, idtable, word, lex, line); 

								word.clear(); 
							}
							else
							{
								throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text); 
							}
							break; 
						}
						case 'r':
						{
							if (check(word, typeReturn)) 
							{
								lex.lexema = LEX_RETURN;
								lex.sn = line; 
								lex.idxTI = -1; 
								lex.comm = word;
								LT::Add(lextable, lex); 

								word.clear(); 
							}
							else if (check(word, typeIdentificator)) 
							{
								addIdentificator(lextable, idtable, word, lex, line); 

								word.clear(); 
							}
							else
							{
								throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text); 
							}
							break; 
						}

						case 'p':
						{
							if (check(word, typePrint)) 
							{
								lex.lexema = LEX_PRINT;
								lex.sn = line; 
								lex.idxTI = -1; 
								lex.comm = word;
								LT::Add(lextable, lex);

								word.clear();
							}
							else if (check(word, typeIdentificator)) 
							{
								addIdentificator(lextable, idtable, word, lex, line); 

								word.clear(); 
							}
							else
							{
								throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text); 
							}
							break; 
						}

						default:
						{
							if (check(word, typeNumbLiteral)) // Проверка, является ли слово числовым литералом
							{
								int i = IT::findLexInteger(idtable, stoi(word)); // Поиск целочисленного значения в таблице идентификаторов
								lex.lexema = LEX_LITERAL; 
								lex.sn = line; 
								lex.comm = word; 
								if (i != LT_TI_NULLIDX) 
								{
									lex.idxTI = i; 
								}
								else
								{
									
									idn.id = "L" + std::to_string(count); // Формирование нового имени идентификатора
									idn.iddatatype = IT::INT; 
									idn.idtype = IT::L; // Установка типа идентификатора как локальная переменная

									idn.idxfirstLE = lextable.size; 

									lex.idxTI = idtable.size; 

									idn.value.setInteger(stoi(word)); // Установка значения идентификатора
									count++; // Инкремент счетчика идентификаторов
									IT::Add(idtable, idn); 
								}
								LT::Add(lextable, lex); 

								word.clear(); 
							}
							else if (check(word, typeIdentificator)) 
							{
								addIdentificator(lextable, idtable, word, lex, line);
								word.clear();
							}
							else
							{
								throw ERROR_THROW_IN(105, ++line, ++pos, buff, in.text); 
							}

							break; 
						}
						}

					}

				}
			}

		}


		std::cout << "\n";

		std::cout << "\n\nТаблица идентификаторов:\n";
		std::cout << std::setfill(' ');

		for (int i = 0; i < lextable.size; i++) 
		{
			(std::cout) << i << '\t' << "SN: " << lextable.table[i].sn << ", "
				<< "IdxTI: " << lextable.table[i].idxTI << ", " 
				<< "Lexema: " << lextable.table[i].lexema << std::endl;
		}

		std::cout << "\n"; 

		std::cout << "\n\nТаблица идентификаторов:\n";

		std::cout << std::setfill(' '); // Установка заполнителя пробелами для форматирования

		for (int i = 0; i < idtable.size; i++) { 
			std::string type;
			switch (idtable.table[i].iddatatype) { 
			case IT::INT:
				type = "INT"; 
				break;
			case IT::STR:
				type = "STR"; 
				break;
			default:
				type = "Unknown"; 
			}

			std::string idtype; 
			switch (idtable.table[i].idtype) { 
			case IT::V:
				idtype = "Variable"; //переменная
				break;
			case IT::F:
				idtype = "Function"; 
				break;
			case IT::P:
				idtype = "Parameter"; 
				break;
			case IT::L:
				idtype = "Local Variable"; //локальная переменная
				break;
			default:
				idtype = "Unknown"; 
			}
			
			// Вывод заголовков столбцов для текущей строки 
			std::cout << std::left << std::setw(20) << "Index"
				<< std::setw(20) << "IdxfirstLE"
				<< std::setw(15) << "ID"
				<< std::setw(15) << "Fun"
				<< std::setw(10) << "Type";

			if (idtable.table[i].idtype != IT::F) {
				if (idtable.table[i].iddatatype == IT::INT) {
					std::cout << std::setw(15) << "IntValue"; 
				}
				else if (idtable.table[i].iddatatype == IT::STR) {
					std::cout << std::setw(15) << "StrValue"; 
				}
			}
			else {
				std::cout << std::setw(15) << "-"; 
			}

			std::cout << std::setw(30) << "Function Name" << "\n"; 

			// Вывод данных текущей строки таблицы идентификаторов с выравниванием и заданной шириной
			std::cout << std::left << std::setw(20) << i 
				<< std::setw(20) << idtable.table[i].idxfirstLE 
				<< std::setw(15) << idtable.table[i].id 
				<< std::setw(15) << idtable.table[i].fun
				<< std::setw(10) << type; 

			if (idtable.table[i].idtype != IT::F) { 
				if (idtable.table[i].iddatatype == IT::INT) {
					std::cout << std::setw(15) << idtable.table[i].value.vint; 
				}
				else if (idtable.table[i].iddatatype == IT::STR) {
					std::cout << std::setw(15) << idtable.table[i].value.vstr; 
				}
			}
			else {
				std::cout << std::setw(15) << "-"; 
			}

			std::cout << std::setw(30) << idtable.table[i].functionname << "\n"; 
		}

		std::cout << '\n'; 

	} 
} 

