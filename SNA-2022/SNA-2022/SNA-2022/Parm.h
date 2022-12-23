#pragma once

#define PARM_MAX_SIZE 100					// Максимальная длина строки параметра

#define PARM_IN  L"-in:"					// Ключ для файла исходного кода
#define PARM_OUT L"-out:"					// Ключ для файла объектного кода	
#define PARM_LOG L"-log:"					// Ключ для файла журнала 

#define PARM_LEX L"-lex:"					// Ключ для файла-протокола лексического и семантического анализа
#define PARM_SIN L"-sin:"					// Ключ для файла-протокола синтаксического анализа

#define PARM_OUT_DEFAULT_EXT L".asm"		// Расширение файла объектного кода по умолчанию
#define PARM_LOG_DEFAULT_EXT L".log"		// Расширение файла протокола по умолчанию
#define PARM_LEX_DEFAULT_EXT L".lx" 		// Расширение файла протокола лексического анализа
#define PARM_SIN_DEFAULT_EXT L".sin" 		// Расширение файла протокола синтаксического анализа
#include <tchar.h>

namespace Parm
{
	struct PARM
	{
		wchar_t in[PARM_MAX_SIZE];			// -in:     имя файла исходного кода
		wchar_t out[PARM_MAX_SIZE];			// -out:    имя файла объектного кода
		wchar_t log[PARM_MAX_SIZE];			// -log:    имя файла протокола
		wchar_t lex[PARM_MAX_SIZE];			// -lex:	имя файла протокола лексического анализа
		wchar_t sin[PARM_MAX_SIZE];			// -sin:	имя файла протокола синтаксического анализа
	};

	PARM getparm(int argc, _TCHAR* argv[]);
}
