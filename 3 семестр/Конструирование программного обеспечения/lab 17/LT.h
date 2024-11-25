#pragma once

#define LEXEMA_FIXSIZE 1	// Фиксированный размер лексемы
#define LT_MAXSIZE 4096		// Максимальный размер таблицы лексем
#define LT_TI_NULLIDX 0xffffffff	// Нет записи в таблице идентификаторов
#define LEX_INTEGER		't'	// Лексема для целых чисел
#define LEX_STRING		't'	// Лексема для строк
#define LEX_LITERAL		'l'	// Лексема для литералов
#define LEX_ID			'i'	// Лексема для идентификаторов
#define LEX_FUNCTION	'f'	// Лексема для функций
#define LEX_MAINFUNC	'm'	// Лексема для главной функции
#define LEX_DECLARE		'd'	// Лексема для объявлений
#define LEX_RETURN		'r'	// Ключевое слово return
#define LEX_PRINT		'p'	// Ключевое слово print
#define LEX_SEMICOLON	';'	// ;
#define LEX_COMMA		','	// ,
#define LEX_LEFTBRACE	'{'	// {
#define LEX_RIGHTBRACE	'}'	// }
#define LEX_LEFTHESIS	'('	// (
#define LEX_RIGHTHESIS	')'	// )
#define LEX_PLUS		'v'	// +
#define LEX_MINUS		'v'	// -
#define LEX_STAR		'v'	// *
#define LEX_DIRSLASH	'v'	// /
#define LEX_RAV			'='	// =
#define LIB_FUNCTION1 "substr"
#define LIB_FUNCTION2 "strlen"


namespace LT	
{
	struct Entry	// Структура для представления записи в таблице лексем
	{
		unsigned char lexema;	
		int sn;			// Номер строки в исходном коде
		int idxTI;		
		std::string comm{};
	};

	struct LexTable		// Структура для таблицы лексем
	{
		int maxsize;	
		int size;		
		Entry* table;	/// Массив записей таблицы лексем
	};

	LexTable Create( 
		int size	
	);
	void Add(				
		LexTable& lextable, /// Ссылка на таблицу лексем
		Entry entry			/// Запись для добавления
	);

	Entry GetEntry(			
		LexTable& lextable,	/// Ссылка на таблицу лексем
		int n				/// Индекс записи
	);

	void Delete(LexTable& lextable); 
};
