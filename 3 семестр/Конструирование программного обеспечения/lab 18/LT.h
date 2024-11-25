#pragma once
#pragma once
#include "IT.h"
#define LEXEMA_FIXSIZE  1			//фиксированный размер лексемы
#define	LT_MAXSIZE		4096		// максимальное количество строк в таблице лексем	
#define	LT_TI_NULLXDX	-1			// нет элемента таблицы идентификаторов				
#define	LEX_INTEGER		't'			// лексема дл€ integer
#define	LEX_STRING		't'			// лексема дл€ string
#define	LEX_ID			'i'			// лексема дл€ идентификатора
#define	LEX_LITERAL		'l'			// лексема дл€ литерала
#define	LEX_FUNCTION	'f'			// лексема дл€ function
#define	LEX_DECLARE		'd'			// лексема дл€ declare
#define	LEX_RETURN		'r'			// лексема дл€ return
#define	LEX_PRINT		'p'			// лексема дл€ print
#define	LEX_STRLEN		'e'			// лексема дл€ strlen
#define	LEX_SEMICOLON	';'			// лексема дл€ ;
#define	LEX_COMMA		','			// лексема дл€ ,
#define	LEX_LEFTBRACE	'{'			// лексема дл€ {
#define	LEX_RIGHTBRACE	'}'			// лексема дл€ }
#define	LEX_LEFTHESIS	'('			// лексема дл€ (
#define	LEX_RIGHTHESIS	')'			// лексема дл€ )
#define	LEX_PLUS		'v'			// лексема дл€ +
#define	LEX_MINUS		'v'			// лексема дл€ -
#define	LEX_STAR		'v'			// лексема дл€ *
#define	LEX_DIRSLASH	'v'			// лексема дл€ /
#define	LEX_EQUAL_SIGN	'='			// лексема дл€ =

#define PLUS '+'
#define MINUS '-'
#define STAR '*'
#define DIRSLASH '/'


namespace LT									// таблица лексем
{
	struct Entry								// строка таблицы лексем
	{
		char lexema;							// лексема
		int sn;									// номер строки в исходном тексте
		int idxTI;								// индекс в таблице идентификаторов или LT_TI_NULLIDX
		Entry();
		Entry(const char lex, int str_n, int idxTI);
	};

	struct LexTable								// экзепл€р таблицы лексем
	{
		int maxsize;							// Ємкость таблицы лексем < LT_MAXSIZE
		int size;								// текущий размер таблицы лексем < maxsize
		Entry* table;							// массив строк таблицы лексем


		Entry GetEntry(								// получить строку таблицы лексем
			int n									// номер получаемой строки
		);

		void PrintLexTable(const wchar_t* in);
		LexTable();
	};

	void Add(									// добавить строку в таблицу лексем
		LexTable& lextable,						// экземпл€р таблицы лексем
		Entry entry								// строка таблицы лексем
	);

	void Delete(LexTable& lextable);			// удалить таблицу лексем (освободить пам€ть)
};