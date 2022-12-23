#pragma once
#include "Libraries.h"
#include "In.h"

namespace Log
{
	struct LOG
	{
		wchar_t logfile[PARM_MAX_SIZE];		// Имя файла протокола
		ofstream* stream;					// Выходной поток
	};

	static const LOG INITLOG = { L"", NULL };		// структура для начальной инициализации LOG

	LOG  GetLog(wchar_t logfile[]);					// сформировать структуру LOG
	void WriteLine(LOG log, char* c, ...);			// вывести в протокол конкатенацию строк
	void WriteLine(LOG log, wchar_t* c, ...);		// вывести в протокол конкатенацию строк
	void WriteLog(LOG log);							// вывести в протокол заголовок
	void WriteParm(LOG log, Parm::PARM parm);		// вывести в протокол информацию о входных параметрах
	void WriteIn(LOG log, IN::IN in);				// вывести в протокол информациб о входном потоке
	void WriteError(LOG log, Error::ERROR error);	// вывести в прокол информацию об ошибке
	void Close(LOG log);							// закрыть протокол
}
