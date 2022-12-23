#pragma once
#include "Log.h"

#define ID_MAXSIZE		6			//максимальное кол-во символов в идентификаторе
#define TI_MAXSIZE		4096		//максимальное кол-во строк в идентификаторе
#define TI_INT_DEFAULT	0x000000	//значение по умолчанию для типа int
#define TI_STRDEFAULT	0x00		//значение по умолчанию для типа str
constexpr auto TI_NULLIDX = 0xffffff;	//нет элемента таблицы идентификаторов
#define TI_STR_MAXSIZE	255	

namespace IT
{
	enum IDDATATYPE { IDDATATYPE_OFF = 0, IDDATATYPE_INT = 1, IDDATATYPE_STR = 2, IDDATATYPE_MATH = 3 };
	enum IDTYPE { IDTYPE_N = 0, IDTYPE_VAR = 1, IDTYPE_FUN = 2, IDTYPE_PAR = 3, IDTYPE_LIT = 4, IDTYPE_ACT = 5 };   //переменная, функция, параметр функции, литерал, оператор

	struct Entry
	{
		int			idFirstInLT;		
		string		id;					
		IDDATATYPE	idDataType;		
		IDTYPE		idType;			
		struct
		{
			int intValue;				
			struct
			{
				int lenghtstr;						
				string str;					
			} strValue;								
		} value;				
	};

	struct IdTable				
	{
		int maxsize;						
		int size;								
		Entry* table;						
	};

	IdTable Create(int size);  
	void Add(IdTable& idtable, Entry entry);
	Entry GetEntry(IdTable& idtable, int n);
	int IsId(IdTable& idtable, string id); 
	int CheckId(IdTable& idtable, string value);
	int IsDublId(IdTable& idtable, string id);		
	int CheckId(IdTable& idtable, int value);
	void ShowTable(IdTable& idtable);
	void WriteTable(Log::LOG log, IdTable& idtable);
};
