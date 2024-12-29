#pragma once
#define ID_MAXSIZE		8
#define TI_MAXSIZE		4096
#define TI_INT_DEFAULT	0x00000000
#define TI_STR_DEFAULT	0x00
#define TI_NULLIDX		0xffffffff
#define TI_STR_MAXSIZE	255

namespace IT {
	enum IDDATATYPE { INT = 1, STR = 2, UNDEF = 3, CHAR = 4, BOOL = 5, BYTE = 6 }; // Добавлен BYTE
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4 };

	struct Entry {
		int			idxfirstLE;
		char		id[ID_MAXSIZE];
		IDDATATYPE	iddatatype = UNDEF;
		IDTYPE		idtype;
		char		scope[10] = "";
		int			numberOfParam;
		union {
			int vint; // Используется также для BYTE
			struct {
				char len;
				char str[TI_STR_MAXSIZE - 1];
			} vstr;
		} value;
	};

	struct IdTable {
		int maxsize;
		int size;
		Entry* table;
	};

	IdTable Create(int size);
	void Add(IdTable& idtable, Entry entry);
	Entry GetEntry(IdTable& idtable, int n);
	int IsId(IdTable& idtable, char id[ID_MAXSIZE]);
	void Delete(IdTable& idtable);
	void Replace(IdTable& idtable, Entry entry, int index);
	void showTable(IdTable& idtable);
	void WriteTable(IdTable& idtable, wchar_t* name);
}
