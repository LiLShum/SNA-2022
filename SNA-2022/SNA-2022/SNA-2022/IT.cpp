#include "Libraries.h"
#include <fstream>
#include "IT.h"
using namespace std;

namespace IT
{
	IdTable Create(int size)
	{
		IdTable* p = new IdTable;
		p->maxsize = size;
		p->size = 0;
		p->table = new Entry[size];
		return (*p);
	}


	void Add(IdTable& idtable, Entry entry)
	{
		idtable.table[idtable.size++] = entry;
	}


	Entry GetEntry(IdTable& idtable, int n)
	{
		return (idtable.table[n]);
	}


	int IsId(IdTable& idtable, string id)
	{
		for (int i = 0; i < idtable.size; i++) {
			if (idtable.table[i].id == id) {
				return i;
			}
		}

		return TI_NULLIDX;
	}


	int IsDublId(IdTable& idtable, string id)
	{
		for (int i = 0; i < idtable.size - 1; i++) {
			if (idtable.table[i].id == id) {
				return i;
			}
		}

		return TI_NULLIDX;
	}


	int CheckId(IdTable& idtable, string value)
	{
		bool rc = false;
		int i;

		for (i = 0; i < idtable.size; i++) {
			if (idtable.table[i].value.strValue.str == value) {
				rc = true;
				break;
			}
		}

		return rc ? i : TI_NULLIDX;
	}


	int CheckId(IdTable& idtable, int value)
	{
		bool rc = false;
		int i;

		for (i = 0; i < idtable.size; i++) {
			if (idtable.table[i].value.intValue == value)
			{
				if (idtable.table[i].idType == IDTYPE_LIT && idtable.table[i].idDataType == IDDATATYPE_INT)
				{
					rc = true;
					break;
				}
			}
		}

		return rc ? i : TI_NULLIDX;
	}

	void WriteTable(Log::LOG log, IdTable& idtable)
	{
		int i, numberOP = 0;

		*(log.stream) << setfill('-') << setw(87) << '-' << endl;
		*(log.stream) << "   №" << " | " << "Идентификатор" << " | " << "Тип данных" << " | " << "Тип идентификатора" << " | " << "Индекс в ТЛ" << " | " << "Значение    " << endl;
		*(log.stream) << setw(87) << '-' << endl;

		for (i = 0; i < idtable.size; i++) {
			*(log.stream) << setfill(' ') << setw(4) << std::right << i << " | ";
			*(log.stream) << setw(13) << left << idtable.table[i].id << " | ";
			switch (idtable.table[i].idDataType) {
			case  IDDATATYPE_INT:
				*(log.stream) << setw(10) << left;
				if (idtable.table[i].idType == IDTYPE_ACT) {
					*(log.stream) << "-" << " | ";
				}
				else {
					*(log.stream) << "num" << " | ";
				}
				break;

			case  IDDATATYPE_STR:
				*(log.stream) << setw(10) << left;
				if (idtable.table[i].idType == IDTYPE_ACT) {
					*(log.stream) << "-" << " | ";
				}
				else {
					*(log.stream) << "str" << " | ";
				}
				break;

			default:
				*(log.stream) << setw(10) << left << "unknown" << " | "; break;
			}

			switch (idtable.table[i].idType) {
			case IDTYPE_VAR:
				*(log.stream) << setw(18) << left << "переменная" << " | ";
				break;
			case IDTYPE_FUN:
				*(log.stream) << setw(18) << left << "функция" << " | ";
				break;
			case IDTYPE_PAR:
				*(log.stream) << setw(18) << left << "параметр" << " | ";
				break;
			case IDTYPE_LIT:
				*(log.stream) << setw(18) << left << "литерал" << " | ";
				break;
			case IDTYPE_ACT:
				*(log.stream) << setw(18) << left << "оператор" << " | ";
				numberOP++;
				break;
			default:
				*(log.stream) << setw(18) << left << "unknown" << " | ";
				break;
			}
			*(log.stream) << setw(11) << left << idtable.table[i].idFirstInLT << " | ";
			if (idtable.table[i].idDataType == IDDATATYPE_INT && (idtable.table[i].idType == IDTYPE_VAR || idtable.table[i].idType == IDTYPE_LIT)) {
				*(log.stream) << setw(18) << left << idtable.table[i].value.intValue;
			}
			else if (idtable.table[i].idDataType == IDDATATYPE_STR && (idtable.table[i].idType == IDTYPE_VAR || idtable.table[i].idType == IDTYPE_LIT)) {
				*(log.stream) << "[" << idtable.table[i].value.strValue.lenghtstr << "]" << idtable.table[i].value.strValue.str;
			}
			else {
				*(log.stream) << "-";
			}
			*(log.stream) << endl;
		}
		*(log.stream) << setfill('-') << setw(87) << '-' << endl;
		*(log.stream) << "Количество идентификаторов: " << i - numberOP << endl;
		*(log.stream) << setw(87) << '-' << endl;
	}

	void ShowTable(IdTable& idtable)
	{
		int i, numberOP = 0;
		cout << setfill('-') << setw(87) << '-' << endl;
		cout << "   №" << " | " << "Идентификатор" << " | " << "Тип данных" << " | " << "Тип идентификатора" << " | " << "Индекс в ТЛ" << " | " << "Значение    " << endl;
		cout << setw(87) << '-' << endl;
		for (i = 0; i < idtable.size; i++) {
			cout << setfill(' ') << setw(4) << std::right << i << " | ";
			cout << setw(13) << left << idtable.table[i].id << " | ";
			switch (idtable.table[i].idDataType) {
			case  IDDATATYPE_INT:
				cout << setw(10) << left;
				if (idtable.table[i].idType == IDTYPE_ACT) {
					cout << "-" << " | ";
				}
				else {
					cout << "num" << " | ";
				}
				break;

			case  IDDATATYPE_STR:
				cout << setw(10) << left;
				if (idtable.table[i].idType == IDTYPE_ACT) {
					cout << "-" << " | ";
				}
				else {
					cout << "str" << " | ";
				}
				break;

			default:
				cout << setw(10) << left << "unknown" << " | "; break;
			}

			switch (idtable.table[i].idType) {
			case IDTYPE_VAR:
				cout << setw(18) << left << "переменная" << " | ";
				break;

			case IDTYPE_FUN:
				cout << setw(18) << left << "функция" << " | ";
				break;

			case IDTYPE_PAR:
				cout << setw(18) << left << "параметр" << " | ";
				break;

			case IDTYPE_LIT:
				cout << setw(18) << left << "литерал" << " | ";
				break;

			case IDTYPE_ACT:
				cout << setw(18) << left << "оператор" << " | ";
				numberOP++;
				break;

			default:
				cout << setw(18) << left << "unknown" << " | ";
				break;
			}
			cout << setw(11) << left << idtable.table[i].idFirstInLT << " | ";
			if (idtable.table[i].idDataType == IDDATATYPE_INT && (idtable.table[i].idType == IDTYPE_VAR || idtable.table[i].idType == IDTYPE_LIT)) {
				cout << setw(18) << left << idtable.table[i].value.intValue;
			}
			else if (idtable.table[i].idDataType == IDDATATYPE_STR && (idtable.table[i].idType == IDTYPE_VAR || idtable.table[i].idType == IDTYPE_LIT)) {
				cout << "[" << idtable.table[i].value.strValue.lenghtstr << "]" << idtable.table[i].value.strValue.str;
			}
			else {
				cout << "-";
			}
			cout << endl;
		}
		cout << setfill('-') << setw(87) << '-' << endl;
		cout << "Количество идентификаторов: " << i - numberOP << endl;
		cout << setw(87) << '-' << endl;
	}
}