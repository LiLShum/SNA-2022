#include "Libraries.h"
#include "LT.h"
namespace LT
{
	LexTable Create(int size)
	{
		LexTable* newTable = new LexTable;					
		newTable->maxize = size;							
		newTable->size = 0;									
		newTable->table = new Entry[size];					
		memset(newTable->table, 255, 255);					
		return *newTable;
	}

	Entry Add(LexTable& lextable, Entry entry)
	{
		return(lextable.table[lextable.size++] = entry);
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		return (lextable.table[n]);
	}

	void Write(Log::LOG log, LexTable& lexTable)
	{
		int k = 1;
		*(log.stream) << "\n\n1: ";
		for (int i = 0; i < lexTable.size; i++) {
			if (lexTable.table[i].lineNo != k)
			{
				*(log.stream) << "\n" << lexTable.table[i].lineNo << ": ";
				k = lexTable.table[i].lineNo;
				if (lexTable.table[i].indexTI != (int)LT_TI_NULLIDX)
				{
					*(log.stream) << lexTable.table[i].lexema;
				}
				else
				{
					*(log.stream) << lexTable.table[i].lexema;
				}
			}
			else
			{
				if (lexTable.table[i].indexTI != (int)LT_TI_NULLIDX)
				{
					*(log.stream) << lexTable.table[i].lexema;
				}
				else
				{
					*(log.stream) << lexTable.table[i].lexema;
				}
			}
		}
		*(log.stream) << "\nКоличество лексем - " << lexTable.size << endl;
	}

	void ShowTable(LexTable& lexTable)
	{
		int k = 0;
		cout << endl << "Таблица лексем: " << endl;
		for (int i = 0; i < lexTable.size; i++) {
			if (lexTable.table[i].lineNo != k) {
				cout << "\n" << lexTable.table[i].lineNo << ": ";
				k = lexTable.table[i].lineNo;
				if (lexTable.table[i].indexTI != (int)LT_TI_NULLIDX) {
					cout << lexTable.table[i].lexema;
				}
				else {
					cout << lexTable.table[i].lexema;
				}
			}
			else {
				if (lexTable.table[i].indexTI != (int)LT_TI_NULLIDX) {
					cout << lexTable.table[i].lexema;
				}
				else {
					cout << lexTable.table[i].lexema;
				}
			}
		}
		cout << "\nКоличество лексем - " << lexTable.size;
	}
}