#pragma once
#define ERROR_THROW(id) Error::geterror(id);
#define ERROR_THROW_IN(id, l, c) Error::geterrorin(id, l, c);
#define ERROR_ENTRY(id, m) {id, m, {-1, -1}}
#define ERROR_MAXSIZE_MESSAGE 200
#define ERROR_ENRTY_NODEF(id)	ERROR_ENTRY(-id, "Неопределённая ошибка")

#define ERROR_ENRTY_NODEF10(id)  ERROR_ENRTY_NODEF(id+0), ERROR_ENRTY_NODEF(id+1), ERROR_ENRTY_NODEF(id+2), ERROR_ENRTY_NODEF(id+3), \
								 ERROR_ENRTY_NODEF(id+4), ERROR_ENRTY_NODEF(id+5), ERROR_ENRTY_NODEF(id+6), ERROR_ENRTY_NODEF(id+7), \
								 ERROR_ENRTY_NODEF(id+8), ERROR_ENRTY_NODEF(id+9)

#define ERROR_ENRTY_NODEF100(id)  ERROR_ENRTY_NODEF(id+0), ERROR_ENRTY_NODEF(id+10), ERROR_ENRTY_NODEF(id+20), ERROR_ENRTY_NODEF(id+30), \
								 ERROR_ENRTY_NODEF(id+40), ERROR_ENRTY_NODEF(id+50), ERROR_ENRTY_NODEF(id+60), ERROR_ENRTY_NODEF(id+70), \
								 ERROR_ENRTY_NODEF(id+80), ERROR_ENRTY_NODEF(id+90)
#define ERROR_MAX_ENTRY 1000

namespace Error
{
	struct ERROR
	{
		int id;
		char message[ERROR_MAXSIZE_MESSAGE];
		struct IN
		{
			short line;
			short col;
		} inext;
	};

	ERROR geterror(int id);
	ERROR geterrorin(int id, int line, int col);
};
