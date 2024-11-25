#include "stdafx.h"

namespace LT
{
    LexTable Create(int size)
    {
        LexTable* tabl = new LexTable;

        if (size > LT_MAXSIZE)
        {
            throw ERROR_THROW(2); 
        }

        tabl->maxsize = size; 
        tabl->size = 0;
        tabl->table = new Entry[size]; // —оздать новую запись в таблице

        return *tabl;
    }

    void Add(LexTable& lextable, Entry entry)
    {
        // ѕроверить, не превышено ли максимальное количество лексем в таблице
        if (lextable.size + 1 > lextable.maxsize)
        {
            throw ERROR_THROW(114); 
        }

        lextable.table[lextable.size] = entry; 
        lextable.size += 1; 
    }

    // ѕолучить запись из таблицы лексем по индексу
    Entry GetEntry(LexTable& lextable, int n)
    {
        return lextable.table[n]; 
    }

    void Delete(LexTable& lextable)
    {
        delete[] lextable.table; 
    }
};
