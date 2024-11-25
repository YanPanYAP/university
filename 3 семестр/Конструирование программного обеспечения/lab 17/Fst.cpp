#include "stdafx.h" 

namespace FST
{
    RELATION::RELATION(unsigned char c, short nn)
    {
        symbol = c; 
        nnode = nn; //номер следующего узла
    }

    NODE::NODE()
    {
        n_relation = 0; 
        relations = nullptr; 
    }

    NODE::NODE(short n, RELATION rel, ...)
    {
        n_relation = n; 
        RELATION* p = &rel; // Получаем указатель на первый элемент 
        relations = new RELATION[n]; 
        for (short i = 0; i < n; i++) 
        {
            relations[i] = p[i];
        }
    }

    FST::FST(std::string s, short ns, NODE n, ...)
    {
        string = s; 
        nstates = ns; 
        nodes = new NODE[ns]; 
        NODE* p = &n; 
        for (int k = 0; k < ns; k++)
        {
            nodes[k] = p[k]; 
        }
        rstates = new short[nstates]; 
        memset(rstates, 0xff, sizeof(short) * nstates); // Заполняем массив состояний значениями 0xff
        rstates[0] = 0; 
        position = -1; 
    }

    // Функция для выполнения одного шага автомата
    bool step(FST& fst, short*& rstates)
    {
        bool rc = false;
        std::swap(rstates, fst.rstates); // Меняем местами временные состояния и состояния автомата
        for (short i = 0; i < fst.nstates; i++) 
        {
            if (rstates[i] == fst.position) 
                for (short j = 0; j < fst.nodes[i].n_relation; j++) // Цикл по всем состояниям узла
                {
                    if (fst.nodes[i].relations[j].symbol == fst.string[fst.position]) // Если символ совпадает с символом во входной строке
                    {
                        fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1; // Переходим к следующему состоянию
                        rc = true; 
                    }
                }
        }
        return rc; 
    }

    // Функция для выполнения автомата
    bool execute(FST& fst)
    {
        short* rstates = new short[fst.nstates]; 
        memset(rstates, 0xff, sizeof(short) * fst.nstates); 
        short lstring = fst.string.length(); // Получаем длину входной строки
        bool rc = true; 
        for (short i = 0; i < lstring && rc; i++) 
        {
            fst.position++; 
            rc = step(fst, rstates); 
        }
        fst.rstates[0] = 0; 
        return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc); 
    }

    void WriteError(Error::ERROR e)
    {
        std::cout << "Ошибка разбора строки" << std::endl; 
        std::cout << "Ошибка разбора в строке: " << e.inext.buff << std::endl; 
        std::cout << "Позиция: " << e.inext.fstline + 1; 
    }
}
