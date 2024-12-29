#pragma once
#define LEXEMA_FIXSIZE		1				//фиксированный размер лексемы
#define LT_MAXSIZE			4096			//максимальное количество строк в “Ћ
#define LT_TI_NULLIDX		0xffffffff		//нет элемента таблицы идентификаторов
#define MAX_NUMBER_OF_PARAM	9
#define MAX_SCOPE			10
#define LEX_NUMB			't'				//integer
#define LEX_STROKE			't'				//string
#define LEX_TYPE			't'
#define LEX_SYMBOL			't'
#define LEX_BOOL			't'
#define LEX_ID				'i'					//идентификатор
#define LEX_LITERAL			'l'				//литерал
#define LEX_MAIN			'm'
#define LEX_FUNCTION		'f'				//function
#define LEX_NEW				'n'				//declare
#define LEX_RETURN			'r'				//return
#define LEX_WRITE			'p'	            //write
#define LEX_WRITELINE		'd'             //writeline
#define LEX_SEMICOLON		';'
#define LEX_COMMA			','
#define LEX_LEFTBRACE		'['
#define LEX_BRACELET		']'
#define LEX_LEFTHESIS		'('
#define LEX_RIGHTHESIS		')'
#define LEX_PLUS			'+'
#define LEX_MINUS			'-'
#define LEX_STAR			'*'
#define LEX_DIRSLASH		'/'
#define LEX_EQUAL			'='
#define LEX_GREATER			'>'
#define LEX_SMALLER			'<'
#define LEX_GREATEROREQUAL	'}'
#define LEX_SMALLEROREQUAL	'{'
#define LEX_COLON			':'
#define LEX_STATE			'?'
#define LEX_CORRECTLY		'c'
#define LEX_WRONG			'w'
#define LEX_DOL				'$'
#define LEX_REMAINDER		'%'
#define LEX_STRLEN			'S'
#define LEX_INPUT			'U'
#define LEX_RAND			'R'
#define LEX_CYCLE			'v'
#define LEX_OR				'|'
#define LEX_AND				'&'
#define LEX_INVERSION		'!'
#define LEX_IF				'I'
#define LEX_ELSE			'E'
#define LEX_SQRT			'C'
#define LEX_ABS				'A'



namespace LT {				//таблица лексем
	struct Entry			//строка таблицы лексем
	{
		char lexema;					//лексема
		int sn;							//номер строки в исходном тексте
		int idxTI;						//индекс в таблице идентификаторов или LT_TI_NULLIDX
		int priority;
	};

	struct LexTable						//экземпл€р таблицы лексем
	{
		char scope[MAX_SCOPE];
		int maxsize;					//емкость таблицы лексем < LT_MAXSIZE
		int size;						//текущий размер таблицы лексем < maxsize
		Entry* table;					//массив строк таблицы лексем
	};
	LexTable Create(					//создать таблицу лексем
		int size						//емкость таблицы лексем < LT_MAXSIZE
	);
	void Add(							//добавить строку в таблицу лексем
		LexTable& lextable,				//экземпл€р таблицы лексем
		Entry entry						//строка таблицы лексем
	);
	Entry GetEntry(						//получить строку таблицы лексем
		LexTable& lextable,				//экземпл€р таблицы лексем
		int n							//номер получаемой строки
	);
	void Delete(LexTable& lextable);	//удалить таблицу лексем (освободить пам€ть)
	void showTable(LexTable lextable);
	void WriteTable(LexTable lextable, wchar_t* name);
};