#include "stdafx.h"
#include "Log.h" 
#include <iostream> 
#include <ctime> 

namespace Log
{
	LOG getlog(wchar_t logfile[])
	{
		LOG temp; 
		char* outFile = new char[wcslen(logfile) + 1]; 
		wcstombs_s(NULL, outFile, wcslen(logfile) + 1, logfile, wcslen(logfile) + 1); 
		temp.stream = new std::ofstream; 
		temp.stream->open(outFile); 
		if (!temp.stream->is_open()) 
		{
			ERROR_THROW(112); 
		}
		wcscpy_s(temp.logfile, logfile);
		return temp; 
	};

	void WriteLine(LOG log, char* c, ...)
	{
		char** p = &c; 

		while (*p != "") 
		{

			(*log.stream) << *p;
			p += 1; 
		}
	};

	void WriteLine(LOG log, wchar_t* c, ...)
	{
		wchar_t** p = &c; 
		char buffer[50]; // Буфер для преобразования строки

		while (*p != L"") 
		{
			wcstombs(buffer, *p, sizeof(buffer)); 
			(*log.stream) << buffer; 
			p += 1;
		}
	};

	// Функция для записи времени и заголовка лога
	void WriteLog(LOG log)
	{
		char buffer[PARM_MAX_SIZE]; // Буфер для хранения времени

		time_t rawtime; // Переменная для хранения текущего времени
		struct tm* timeinfo; // Структура для хранения времени в формате человеко-читаемого вида

		time(&rawtime); // Получаем текущее время
		timeinfo = localtime(&rawtime); // Преобразуем время в локальное время

		strftime(buffer, PARM_MAX_SIZE, "%d.%m.%Y %H:%M:%S", timeinfo); // Преобразуем время в строковый формат
		(*log.stream) << "\n--— Протокол —— \n" << buffer << " —— \n"; // Записываем время и заголовок в лог
	};

	void WriteParm(LOG log, Parm::PARM parm)
	{
		char inInfo[PARM_MAX_SIZE]; 
		char outInfo[PARM_MAX_SIZE];
		char logInfo[PARM_MAX_SIZE];

		wcstombs(inInfo, parm.in, sizeof(inInfo)); 
		wcstombs(outInfo, parm.out, sizeof(outInfo)); 
		wcstombs(logInfo, parm.log, sizeof(logInfo)); 

		(*log.stream) << "--— Аргументы —— \n" 
			<< " -in: " << inInfo << "\n" 
			<< " -out: " << outInfo << "\n" 
			<< " -log: " << logInfo << "\n";
	};

	void WriteIn(LOG log, In::IN in)
	{
		(*log.stream) << "--— Параметры —— \n" 
			<< "Количество символов: " << in.size << "\n" 
			<< "Строк: " << in.lines << "\n" 
			<< "Пропущено: " << in.ignor << "\n"; 
	};

	void WriteError(LOG log, Error::ERROR error)
	{
		if (error.inext.line != -1) // Если строка ошибки известна
		{
			*log.stream << "Ошибка в строке:\t" << error.inext.buff << std::endl; 
			*log.stream << "Строка: " << error.inext.line << " Позиция: " << error.inext.col << std::endl; 
		}
		*log.stream << "Ошибка: " << error.id << ":" << error.message << std::endl; 
	}

	void Close(LOG log)
	{
		log.stream->close(); 
	}

	// Функция для записи таблицы лексем, переданной через структуру LT::LexTable
	void WriteLexTable(LOG log, LT::LexTable& lextable)
	{
		int currentLine = 0;  //для хранения текущей строки
		bool currentLineChange = true;
		*log.stream << "\n\nТаблица Лексем:\n"; 
		for (int i = 0; i < lextable.size; i++) 
		{
			if (lextable.table[i].sn != currentLine) // Если строка лексемы изменилась
			{
				*log.stream << '\n';  
				currentLine = lextable.table[i].sn; // Обновляем текущую строку
				currentLineChange = true;
			}
			if (currentLine + 1 < 10 && currentLineChange == true) {
				*log.stream << "0" << currentLine + 1 << ' ';
				currentLineChange = false;
			}
			else if (currentLineChange == true) {
				*log.stream << currentLine + 1 << ' ';
				currentLineChange = false;
			}
			*log.stream << lextable.table[i].lexema; // Записываем лексему в лог
		}
	}

};
