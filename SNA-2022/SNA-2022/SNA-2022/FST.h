#pragma once
#include "Libraries.h"
#define ARR_OF_FST_WORD_SIZE 13
#define ARR_OF_FST_OP_SIZE 11

namespace FST
{
	enum LEX_TYPE { SYMB_NOT_FOUND = -1, SYMB_TABLE_ID = 0, SYMB_NEED_PRIORITY, SYMB_LEXEMS, SYMB_ACTION, SYMB_STATIC_LIB, SYMB_LITERAL };
	// тип лексемы : -1 - ошибка, 0 - для таблицы идентификаторов, 1 - для присвоения приоритета, 2 - "обычные лексемы"

	struct RELATION		 // ребро: символ -> вершина графа перехода КА
	{
		char symbol;			// символ перехода
		short nnode;			// номер смежной вершины
		RELATION(
			char c = 0x00,			// символ перехода
			short ns = NULL			// новое состояние
		);
	};

	
	struct NODE			// вершина графа переходов
	{
		short n_relation;		// количество инцидентных рёбер
		RELATION* relations;	// инцидентные рёбра
		NODE();
		NODE(
			short n,				// количество инцидентных рёбер
			RELATION rel, ...		// список рёбер
		);
	};

	struct KA
	{
		LEX_TYPE lex_type;
		char lexema;
	};

	struct FST			// недетерминированный конечный автомат
	{
		char lexema;			// лексема
		LEX_TYPE lex_type;		// тип лексемы
		string str;				// цепочка (строка, завершается 0х00)
		short position;			// текущая позиция в цепочке
		short nstates;			// количество состояний автомата
		NODE* nodes;			// граф переходов: [0] - начальное состояние, [nstates - 1] - конечное
		short* rstates;			// возможные состояния автомата на данной позиции
		FST();
		FST(
			char c,					// лексема
			LEX_TYPE l_t,			// тип лексемы
			string s,				// цепочка(строка, завершается 0х00)
			short ns,				// количество состояний автомата
			NODE n, ...				// список состояний (граф переходов)
		);
	};

	bool execute(		// выполнить распознавание цепочки
		FST& fst			// недетерминированный конечный автомат
	);
	FST* arr_of_fst_op();		// массив автоматов лексем-одиночных символов
	FST* arr_of_fst_word();		// массив автоматов лексем-слов
}