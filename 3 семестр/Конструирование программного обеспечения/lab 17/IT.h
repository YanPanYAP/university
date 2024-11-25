#pragma once
#define ID_MAXSIZE 5
#define TI_MAXSIZE 4096
#define TI_INT_DEFAULT 0
#define TI_STR_DEFAULT ""
#define TI_NULLIDX 0xffffffff
#define TI_STR_MAXSIZE 255
namespace IT
{
    enum IDDATATYPE { INT = 1, STR = 2 }; // Перечисление для типов данных
    enum IDTYPE { V = 1, F = 2, P = 3, L = 4 }; // Перечисление для типов идентификаторов

    struct Entry // Структура для представления записи в таблице идентификаторов
    {
        int idxfirstLE; // Индекс первой записи в таблице лексем, связанной с этим идентификатором
        std::string id; // Идентификатор 
        std::string fun; // Название функции, к которой относится идентификатор
        IDDATATYPE iddatatype; // Тип данных идентификатора (INT или STR)
        IDTYPE idtype; // Тип идентификатора 
        std::string functionname; // Название функции 
        char operation[256]; // Операция 
        struct
        {
            int vint;
            std::string vstr;
            int len;

            void setInteger(int value) {
                vint = value;
            }

            void setString(std::string value) {
                vstr = value;
                len = value.length();
            }
        }value;
    };

    struct IdTable
    {
        int maxsize; 
        int size; 
        Entry* table; // Массив записей таблицы идентификаторов
    };

    IdTable Create(int size); 
    void Add(IdTable& idtable, Entry entry); 
    Entry GetEntry(IdTable& idtable, int n); 
    int IsId(IdTable& idtable, std::string word, std::string functionName, std::string function); // Проверить, является ли идентификатор частью таблицы
    void Delete(IdTable& idtable); 
    int findLexInteger(IdTable& idtable, int word);
    int findLexStr(IdTable& idtable, std::string word);
};


