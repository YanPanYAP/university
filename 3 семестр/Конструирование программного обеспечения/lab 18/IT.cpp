#include "stdafx.h"


namespace IT
{
	IdTable::IdTable()
	{
		this->noname_lexema_count = 0;
		this->maxsize = TI_MAXSIZE;
		this->size = 0;
		this->table = new Entry[TI_MAXSIZE];
	}
	Entry::Entry()
	{
		this->parrent_function[0] = '\0';	//строку родительской функции
		this->id[0] = '\0';		//строку идентификатора
		this->firstApi = 0;		//индекс первого по€влени€
		this->iddatatype = IT::IDDATATYPE::DEF;		//тип данных
		this->idtype = IT::IDTYPE::D;	//тип идентификатора
		this->parmQuantity = 0;		//количество параметров

	}

	Entry::Entry(const char* parrent_function, const char* id, IDDATATYPE iddatatype, IDTYPE idtype, int first)
	{
		int i = 0;
		if (parrent_function)
			for (i = 0; parrent_function[i] != '\0'; i++)
				this->parrent_function[i] = parrent_function[i];
		this->parrent_function[i] = '\0';
		i = 0;
		if (id)
			for (i = 0; id[i] != '\0'; i++)
				this->id[i] = id[i];

		this->firstApi = first;
		this->id[i] = '\0';
		this->iddatatype = iddatatype;
		this->idtype = idtype;
		this->parmQuantity = 0;
	}
	Entry::Entry(const char* parrent_function, const char* id, IDDATATYPE iddatatype, IDTYPE idtype, int first, int it)
	{
		int i = 0;
		if (parrent_function)
			for (i = 0; parrent_function[i] != '\0'; i++)
				this->parrent_function[i] = parrent_function[i];
		this->parrent_function[i] = '\0';
		i = 0;
		if (id)
			for (i = 0; id[i] != '\0'; i++)
				this->id[i] = id[i];

		this->firstApi = first;
		this->id[i] = '\0';
		this->iddatatype = iddatatype;
		this->idtype = idtype;
		this->parmQuantity = 0;
		this->value.vint = it;	// »нициализирует целочисленное значение
	}
	Entry::Entry(const char* parrent_function, const char* id, IDDATATYPE iddatatype, IDTYPE idtype, int first, char* ch)
	{
		int i = 0;
		if (parrent_function)
			for (i = 0; parrent_function[i] != '\0'; i++)
				this->parrent_function[i] = parrent_function[i];
		this->parrent_function[i] = '\0';
		i = 0;
		if (id)
			for (i = 0; id[i] != '\0'; i++)
				this->id[i] = id[i];

		this->firstApi = first;
		this->id[i] = '\0';
		this->iddatatype = iddatatype;
		this->idtype = idtype;
		this->parmQuantity = 0;
		strcpy_s(this->value.vstr.str, 255, ch);	//  опирует строковое значение в vstr
		this->value.vstr.len = strlen(ch);	// ”станавливает длину строки в vstr
	}
	Entry::Entry(const char* parrent_function, const char* id, IDDATATYPE iddatatype, IDTYPE idtype, int first, const char* ch)
	{
		int i = 0;
		if (parrent_function)
			for (i = 0; parrent_function[i] != '\0'; i++)
				this->parrent_function[i] = parrent_function[i];
		this->parrent_function[i] = '\0';
		i = 0;
		if (id)
			for (i = 0; id[i] != '\0'; i++)
				this->id[i] = id[i];

		this->firstApi = first;
		this->id[i] = '\0';
		this->iddatatype = iddatatype;
		this->idtype = idtype;
		this->parmQuantity = 0;
		strcpy_s(this->value.vstr.str, 255, ch);
		this->value.vstr.len = strlen(ch);
	}
	Entry::Entry(char* parrent_function, char* id, IDDATATYPE iddatatype, IDTYPE idtype)
	{
		int i = 0;
		if (parrent_function)
			for (i = 0; parrent_function[i] != '\0'; i++)
				this->parrent_function[i] = parrent_function[i];
		this->parrent_function[i] = '\0';
		i = 0;
		if (id)
			for (i = 0; id[i] != '\0'; i++)
				this->id[i] = id[i];

		this->id[i] = '\0';
		this->iddatatype = iddatatype;
		this->idtype = idtype;
		this->parmQuantity = 0;
	}

	// —оздает и инициализирует объект IdTable с заданным размером
	IdTable Create(int size)
	{
		IdTable id_table;
		id_table.size = size;
		id_table.maxsize = TI_MAXSIZE;
		id_table.table = new Entry[TI_MAXSIZE];
		return id_table;
	}

	// ƒобавл€ет новую запись в IdTable
	void IdTable::Add(Entry entry)
	{
		if (strlen(entry.id) > ID_MAXSIZE && entry.idtype != IDTYPE::F)
			throw ERROR_THROW(121);

		if (this->size < this->maxsize)
		{
			if (entry.idtype != IDTYPE::F)
				entry.id[5] = '\0';
			this->table[this->size] = entry;

			switch (entry.iddatatype)
			{
			case IDDATATYPE::INT:
			{
				this->table[this->size].value.vint = TI_INT_DEFAULT;
			}
			case IDDATATYPE::STR:
			{
				this->table[this->size].value.vstr.str[0] = TI_STR_DEFAULT;
				this->table[this->size].value.vstr.len = 0;
			}
			}
			this->size++;
		}
		else
			throw ERROR_THROW(122);

	}

	// ѕолучает запись по индексу
	Entry IdTable::GetEntry(int n)
	{
		if (n < this->size && n >= 0)
			return this->table[n];
	}

	// ѕровер€ет, существует ли идентификатор в таблице, и возвращает его индекс, если он найден
	int IdTable::IsId(const char id[ID_MAXSIZE])
	{
		for (int iter = 0; iter < this->size; iter++)
		{
			if (strcmp(this->table[iter].id, id) == 0)	// ѕровер€ет на совпадение
				return iter;
		}
		return TI_NULLIDX;
	}

	// ѕровер€ет, существует ли идентификатор с определенной родительской функцией, и возвращает его индекс, если найден
	int IdTable::IsId(const char id[ID_MAXSIZE], const char parrent_function[ID_MAXSIZE + 5])
	{
		for (int i = 0; i < this->size; i++)
		{
			if ((strcmp(this->table[i].id, id) == 0) &&
				(strcmp(this->table[i].parrent_function, parrent_function) == 0))
				return i;
		}
		return TI_NULLIDX;
	}

	// ‘ункци€ дл€ удалени€ таблицы идентификаторов
	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
		idtable.table = nullptr;
	}

	// √енерирует уникальное им€ дл€ лексемы
	char* IdTable::GetLexemaName()
	{
		char buffer[3];		// Ѕуфер дл€ хранени€ числового значени€
		char tempBuf[5];	// ¬ременный буфер дл€ итогового имени лексемы
		_itoa_s(this->noname_lexema_count, buffer, 10);		// ѕреобразует число в строку и сохран€ет его в buffer
		tempBuf[0] = 'L';
		tempBuf[1] = '_';
		tempBuf[2] = buffer[0];
		tempBuf[3] = buffer[1];
		tempBuf[4] = buffer[2];
		return tempBuf;
	}

	// ћетод дл€ вывода таблицы идентификаторов на экран
	void IdTable::PrintIdTable(const wchar_t* in)
	{

		bool flagForFirst = false;
		char buffer[3];
		char tempBuf[5];
		int lexema_count = 0;	// —четчик уникальных лексем
		bool exist = true;		// ‘лаг дл€ проверки существовани€ лексемы
		tempBuf[0] = 'L';
		tempBuf[1] = '_';

		cout << "\n“аблица идентификаторов" << std::endl;

		for (int i = 0; i < this->size; i++)
		{
			switch (this->table[i].idtype)
			{
				//переменна€
			case IT::IDTYPE::V:
				switch (this->table[i].iddatatype)
				{
				case 1:
					cout << "TypeOfId: " << "ѕеременна€" << "\t" << "id: " << this->table[i].id << " " << "\t" << "dataType: " << "INT " << "value: " << 0 << " " << "idxFirstLE: " << this->table[i].firstApi << std::endl;
					break;
				case 2:
					cout << "TypeOfId: " << "ѕеременна€" << "\t" << "id: " << this->table[i].id << " " << "\t" << "dataType: " << "STR " << "value: " << "\"\" " << "idxFirstLE: " << this->table[i].firstApi << std::endl;
					break;
				}
				break;

				//параметр
			case IT::IDTYPE::P:
				switch (this->table[i].iddatatype)
				{
				case 1:
					cout << "TypeOfId: " << "ѕараметр" << "\t" << "id: " << this->table[i].id << " " << "\t" << "dataType: " << "INT " << "value: " << 0 << " " << "idxFirstLE: " << this->table[i].firstApi << std::endl;
					break;
				case 2:
					cout << "TypeOfId: " << "ѕараметр" << "\t" << "id: " << this->table[i].id << " " << "\t" << "dataType: " << "STR " << "value: " << "\"\" " << "idxFirstLE: " << this->table[i].firstApi << std::endl;
					break;
				}
				break;

				//литерал
			case IT::IDTYPE::L:
				_itoa_s(lexema_count, buffer, 10);	// ѕреобразует счетчик лексем в строку и сохран€ет в buffer

				exist = false;

				tempBuf[2] = buffer[0];
				tempBuf[3] = buffer[1];
				tempBuf[4] = buffer[2];
				switch (this->table[i].iddatatype)
				{
				case 1:
					for (int k = 0; k < i; k++) {
						if (this->table[k].idtype == IT::IDTYPE::L)		// ѕровер€ет наличие литерала среди уже добавленных
							if (this->table[k].value.vint == this->table[i].value.vint) {
								exist = true;	 // ”станавливает флаг, если найден дубликат
								break;
							}
					}
					if (!exist) {	//если уникален
						cout << "TypeOfId: " << "Ћитерал" << "\t" << tempBuf << " " << "\t" << "dataType: " << "INT " << "value: " << this->table[i].value.vint << " " << "idxFirstLE: " << this->table[i].firstApi << std::endl;
						lexema_count++;
					}
					break;
				case 2:
					for (int k = 0; k < i; k++) {
						for (int j = 0; j < 10; j++) {
							if (this->table[k].idtype == IT::IDTYPE::L)
								if (this->table[k].value.vstr.str[j] != this->table[i].value.vstr.str[j]) {
									exist = false;
									break;
								}
							exist = true;
						}
					}
					if (!exist) {
						cout << "TypeOfId: " << "Ћитерал" << "\t" << tempBuf << " " << "\t" << "dataType: " << "STR " << "value: " << this->table[i].value.vstr.str << " " << "idxFirstLE: " << this->table[i].firstApi << std::endl;
						lexema_count++;
					}
					break;
				}
				break;

				//функци€
			case IT::IDTYPE::F:
				switch (this->table[i].iddatatype)
				{
				case 1:
					cout << "TypeOfId: " << "‘ункци€" << "\t" << "id: " << this->table[i].id << "\t" << "dataType: " << "INT " << "value: " << "- " << "idxFirstLE: " << this->table[i].firstApi << " " << "Parms:" << this->table[i].parmQuantity << std::endl;
					break;
				case 2:
					cout << "TypeOfId: " << "‘ункци€" << "\t" << "id: " << this->table[i].id << "\t" << "dataType: " << "STR " << "value: " << "- " << "idxFirstLE: " << this->table[i].firstApi << " " << "Parms:" << this->table[i].parmQuantity << std::endl;
					break;
				}
				break;


			}
		}

	}
}