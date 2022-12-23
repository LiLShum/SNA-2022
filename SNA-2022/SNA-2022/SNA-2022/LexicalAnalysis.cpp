#include "Libraries.h"
#include "LexicalAnalysis.h"
#include "FST.h"
namespace Lex
{
	LT::LexTable lexTable = LT::Create(LT_MAXSIZE);
	IT::IdTable idTable = IT::Create(TI_MAXSIZE);

	struct Function
	{
		int indexTI;
		int size = 0;
		string name;
		IT::IDDATATYPE arrayOfDataType[FUNC_MAX_SIZE];	// массив типов параметров
	};

	struct FunctionName
	{
		Function arrayOfFunction[FUNC_MAX_SIZE];
		int size = -1;
	}Func_Han;

	FST::KA SearchFST(string src)
	{
		int size;
		FST::KA result;
		FST::FST* arr_of_fst = new FST::FST;
		if (src.size() == 1)
		{
			arr_of_fst = FST::arr_of_fst_op();
			size = ARR_OF_FST_OP_SIZE;
		}
		else
		{
			arr_of_fst = FST::arr_of_fst_word();
			size = ARR_OF_FST_WORD_SIZE;
		}
		for (int i = 0; i < size; i++) {
			arr_of_fst[i].str = src;
			if (execute(arr_of_fst[i]) == true)
			{
				result.lexema = arr_of_fst[i].lexema;
				result.lex_type = arr_of_fst[i].lex_type;
				return result;
			}
		}
		return { FST::SYMB_NOT_FOUND, 0 };
	}


	LT::Entry CreateLex(char lex, int line, int id_in_TI, int prior)
	{
		LT::Entry result;
		result.priority = prior;
		result.lexema = lex;
		result.lineNo = line;
		result.indexTI = id_in_TI;
		return result;
	}

	IT::Entry CreateId(int line, string id, IT::IDDATATYPE id_data_type, IT::IDTYPE id_type, int vint)
	{
		IT::Entry result;
		result.id = id;
		result.idFirstInLT = line;
		result.idDataType = id_data_type;
		result.idType = id_type;
		result.value.intValue = vint;
		return result;
	}
	IT::Entry CreateId(int line, string id, IT::IDDATATYPE id_data_type, IT::IDTYPE id_type)
	{
		IT::Entry result;
		result.id = id;
		result.idFirstInLT = line;
		result.idDataType = id_data_type;
		result.idType = id_type;
		return result;
	}

	IT::Entry CreateId(int line, string id, IT::IDDATATYPE id_data_type, IT::IDTYPE id_type, string vstr)
	{
		IT::Entry result;
		result.id = id;
		result.idFirstInLT = line;
		result.idDataType = id_data_type;
		result.idType = id_type;
		result.value.strValue.str = vstr;
		result.value.strValue.lenghtstr = vstr.size();
		return result;
	}

	string MakeString(char symb, int num_of_lit)
	{
		string result = "1";
		result[0] = symb;
		return result + to_string(num_of_lit);
	}

	bool Check(IT::IDDATATYPE* arr_of_it_entry, string name)
	{
		for (int i = 0; i < Func_Han.size + 1; i++)
		{
			if (Func_Han.arrayOfFunction[i].name == name)
			{
				for (int j = 0; j < Func_Han.arrayOfFunction[i].size; j++)
				{
					if (Func_Han.arrayOfFunction[i].arrayOfDataType[j] != arr_of_it_entry[j])
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	LEX Lexer(IN::IN in, Log::LOG log)
	{
		bool isHesisForBlockFunction = false,	
			isParametrsInTheFunction = false,	
			isDim = false,			// можно ли объявлять переменные
			isAction = false,		
			isComeback = false,		
			isEntry = false,		
			isStrlen = false,
			isStrToN = false,
			isProcedure = false,
			isParametr = false,
			isEqual = false,
			isCycle = false,
			isBlockInTheCycle = false;




		LEX tables;					
		string func_name = "", funcHesisName = "", func_vision_name = ""; 
		IT::IDDATATYPE arrayOfITEntry[FUNC_MAX_SIZE]; 
		IN::Word queue_word;
		LT::Entry elementLT;  
		FST::KA lexFromFst;
		IT::IDDATATYPE data_type = IT::IDDATATYPE_OFF;
		int nLit = 0, //кол литералов
			nParam = 0,  //кол параметров
			size = in.Words.size(), //кол лексем
			counterMain = 0,
			counterOfLeftBrace = 0;
		for (int i = 0; i < size; i++) {
			queue_word = in.Words.front();
			in.Words.pop();
			lexFromFst = SearchFST(queue_word.word);
			elementLT = CreateLex(lexFromFst.lexema, queue_word.line, LT_TI_NULLIDX, LT_NOPRIORITY);
			switch (lexFromFst.lex_type) {
			case FST::SYMB_NOT_FOUND:
			{
				throw ERROR_THROW_IN(120, elementLT.lineNo, -1);
				break;
			}
			case FST::SYMB_TABLE_ID://идентификаторы
			{
				int index = 0;
				if (isProcedure)
				{
					isParametrsInTheFunction = true;
					func_name = queue_word.word;
					func_vision_name = func_name;
					isProcedure = false;
					if (IT::IsDublId(idTable, queue_word.word.substr(0, 6)) != TI_NULLIDX)
					{
						throw ERROR_THROW_IN(700, elementLT.lineNo, -1);
					}
					else
					{
						if (data_type == IT::IDDATATYPE_INT || data_type == IT::IDDATATYPE_STR)
						{
							idTable.table[idTable.size] = CreateId(lexTable.size, queue_word.word.substr(0, 6), data_type, IT::IDTYPE_FUN);
							elementLT.indexTI = idTable.size++;
							Func_Han.size++;
						}
						else
						{
							throw ERROR_THROW_IN(701, elementLT.lineNo, -1);
						}
					}
					break;
				}
				if (isCycle)
				{
					isCycle = false;
					if (IT::IsDublId(idTable, queue_word.word.substr(0, 6)) != TI_NULLIDX)
					{
						throw ERROR_THROW_IN(700, elementLT.lineNo, -1);
					}
					else
					{
						if (data_type == IT::IDDATATYPE_INT || data_type == IT::IDDATATYPE_STR)
						{
							idTable.table[idTable.size] = CreateId(lexTable.size, func_name + queue_word.word.substr(0, 6), data_type, IT::IDTYPE_FUN);
							elementLT.indexTI = idTable.size++;
							Func_Han.size++;
						}
						else
						{
							throw ERROR_THROW_IN(701, elementLT.lineNo, -1);
						}
					}
					break;
				}
				if (isParametrsInTheFunction)
				{
					if ((index = IT::IsId(idTable, func_name + queue_word.word.substr(0, 6))) == TI_NULLIDX)
					{
						idTable.table[idTable.size] = CreateId(lexTable.size, func_name + queue_word.word.substr(0, 6), data_type, IT::IDTYPE_PAR);
						elementLT.indexTI = idTable.size++;
					}
					else
					{
						if (isEntry)
						{
							arrayOfITEntry[nParam++] = idTable.table[index].idDataType;
						}
						if (isCycle)
						{
							arrayOfITEntry[nParam++] = idTable.table[index].idDataType;
						}
						elementLT.indexTI = index;
					}
				}
				if (isHesisForBlockFunction)
				{
					if ((index = IT::IsId(idTable, func_name + queue_word.word.substr(0, 6))) == TI_NULLIDX)
					{
						if ((index = IT::IsId(idTable, queue_word.word.substr(0, 6))) != TI_NULLIDX)
						{
							elementLT.indexTI = index;
							if (idTable.table[index].idType == IT::IDTYPE_FUN)
							{
								funcHesisName = idTable.table[index].id;
								isParametrsInTheFunction = true;
							}
						}

						if (isDim)
						{
							if (data_type == IT::IDDATATYPE_STR)
							{
								idTable.table[idTable.size] = CreateId(lexTable.size, func_name + queue_word.word.substr(0, 6), data_type, IT::IDTYPE_VAR, "");
							}
							else
							{
								idTable.table[idTable.size] = CreateId(lexTable.size, func_name + queue_word.word.substr(0, 6), data_type, IT::IDTYPE_VAR, 0);
							}
							elementLT.indexTI = idTable.size++;
							////
						}
						////СЕМАНТИКА/
						else if (index == TI_NULLIDX)
						{
							throw ERROR_THROW_IN(706, queue_word.line, -1);
						}
					}
					else//если есть индекс вхождения в табл идентификаторов
					{
						elementLT.indexTI = index;
						if (isDim)
						{
							throw ERROR_THROW_IN(700, elementLT.lineNo, -1);
						}
						if (isEqual && !isParametrsInTheFunction)
						{
							if (idTable.table[index].idDataType != data_type)
							{
								throw ERROR_THROW_IN(707, queue_word.line, -1);
							}
						}
					}
				}
				if (isBlockInTheCycle)
				{
					if ((index = IT::IsId(idTable, func_name + queue_word.word.substr(0, 6))) == TI_NULLIDX)
					{
						if ((index = IT::IsId(idTable, func_vision_name + queue_word.word.substr(0, 6))) != TI_NULLIDX)
						{
							elementLT.indexTI = index;
							if (idTable.table[index].idType == IT::IDTYPE_FUN)
							{
								funcHesisName = idTable.table[index].id;
								isParametrsInTheFunction = true;
							}
						}

						if (isDim)
						{
							if (data_type == IT::IDDATATYPE_STR)
							{
								idTable.table[idTable.size] = CreateId(lexTable.size, func_name + queue_word.word.substr(0, 6), data_type, IT::IDTYPE_VAR, "");
							}
							else
							{
								idTable.table[idTable.size] = CreateId(lexTable.size, func_name + queue_word.word.substr(0, 6), data_type, IT::IDTYPE_VAR, 0);
							}
							elementLT.indexTI = idTable.size++;
						}
						
						else if (index == TI_NULLIDX)
						{
							throw ERROR_THROW_IN(706, queue_word.line, -1);
						}

					}
					else
					{
						elementLT.indexTI = index;
						if (isDim)
						{
							throw ERROR_THROW_IN(700, elementLT.lineNo, -1);
						}
						if (isEqual && !isParametrsInTheFunction)
						{
							if (idTable.table[index].idDataType != data_type)
							{
								throw ERROR_THROW_IN(707, queue_word.line, -1);
							}
						}
					}
				}
				isAction = false;


				if (isComeback)
				{
					if (idTable.table[elementLT.indexTI].idDataType != IT::IDDATATYPE_INT)
					{
						throw ERROR_THROW_IN(701, queue_word.line, -1);
					}
					isComeback = false;
				}
				if (isStrToN)
				{
					if (idTable.table[elementLT.indexTI].idDataType != IT::IDDATATYPE_STR)
					{
						throw ERROR_THROW_IN(705, queue_word.line, -1);
					}
					isStrToN = false;
				}

				if (isStrlen)
				{
					if (idTable.table[elementLT.indexTI].idDataType != IT::IDDATATYPE_STR)
					{
						throw ERROR_THROW_IN(705, queue_word.line, -1);
					}
					isStrlen = false;
				}
				break;
			}

			case FST::SYMB_LITERAL:
			{
				switch (lexFromFst.lexema) {
				case LEX_NUM:
				{
					if (queue_word.word[0] == '0' && queue_word.word.length() > 1)
					{
						int result8to10 = 0;
						for (size_t i = 0; i < queue_word.word.length(); i++)
						{
							result8to10 += (queue_word.word[i] - '0') * pow(8, queue_word.word.length() - i - 1);
						}
						queue_word.word = std::to_string(result8to10);
					}
					int sizeOfInt = 0;
					sizeOfInt = atoi(queue_word.word.c_str());
					if (sizeOfInt > 127 || sizeOfInt < -128)
					{
						throw ERROR_THROW(125);
					}
					int index = IT::CheckId(idTable, atoi(queue_word.word.c_str()));
					if (index == TI_NULLIDX)
					{
						idTable.table[idTable.size] = CreateId(lexTable.size, MakeString(LEX_L, nLit++), IT::IDDATATYPE_INT, IT::IDTYPE_LIT, atoi(queue_word.word.c_str()));
						elementLT.indexTI = idTable.size++;
					}
					else
					{
						elementLT.indexTI = index;
					}

					isAction = false;
					break;
				}
				case LEX_STR:
				{
					int index = IT::CheckId(idTable, queue_word.word);
					if (index == TI_NULLIDX)
					{
						idTable.table[idTable.size] = CreateId(lexTable.size, MakeString(LEX_L, nLit++), IT::IDDATATYPE_STR, IT::IDTYPE_LIT, queue_word.word);
						elementLT.indexTI = idTable.size++;
					}
					else
					{
						elementLT.indexTI = index;
					}
					if (isStrlen)
					{
						isStrlen = false;
					}
					break;
				}
				}
				if (isEntry && isParametrsInTheFunction)
				{
					arrayOfITEntry[nParam++] = idTable.table[elementLT.indexTI].idDataType;
				}
				if (isEqual && !isParametrsInTheFunction)			///////////////////СЕМАНТИКА
				{
					if (idTable.table[elementLT.indexTI].idDataType != data_type)
					{
						throw ERROR_THROW_IN(707, queue_word.line, -1); //Несоответствие типов в операторе присваивания
					}
				}
				break;
			}

			case FST::SYMB_NEED_PRIORITY:
			{
				switch (queue_word.word[0]) {
				case LEX_COMMA:
					elementLT.priority = 1;
					break;

				case LEX_LEFTHESIS:
					if (isParametrsInTheFunction && elementLT.lexema == LEX_LEFTHESIS)
					{
						elementLT.priority = 4;
						isParametr = true; //следует, что надо закрыть скобку
					}
					else
					{
						elementLT.priority = 0;
					}
					break;

				case LEX_RIGHTHESIS:
					if (isParametrsInTheFunction && isParametr)
					{
						if (isEntry)
						{
							if (Check(arrayOfITEntry, funcHesisName))
							{
								throw ERROR_THROW_IN(702, elementLT.lineNo, -1);
							}
							funcHesisName = "";
							nParam = 0;
						}
						elementLT.priority = 4;
						isParametr = false;
						isParametrsInTheFunction = false;
						isCycle = false;
					}
					else
					{
						elementLT.priority = 0;
					}
					break;

				}
				break;
			}

			case FST::SYMB_LEXEMS:
			{
				switch (lexFromFst.lexema) {
				case LEX_ENTRY:
					isEntry = true;
					func_name = queue_word.word;
					func_vision_name = func_name;
					counterMain++;
					break;

				case LEX_FUNC:
					isProcedure = true;

					break;

				case LEX_STR:
					if (isParametrsInTheFunction && !isEntry)
					{
						Func_Han.arrayOfFunction[Func_Han.size].arrayOfDataType[Func_Han.arrayOfFunction[Func_Han.size].size++] = IT::IDDATATYPE_STR;
						Func_Han.arrayOfFunction[Func_Han.size].name = func_name;
					}
					data_type = IT::IDDATATYPE_STR;
					elementLT.lexema = LEX_T;
					break;

				case LEX_NUM:
					if (isParametrsInTheFunction && !isEntry)
					{
						Func_Han.arrayOfFunction[Func_Han.size].arrayOfDataType[Func_Han.arrayOfFunction[Func_Han.size].size++] = IT::IDDATATYPE_INT;
						Func_Han.arrayOfFunction[Func_Han.size].name = func_name;
					}
					data_type = IT::IDDATATYPE_INT; //тип данных
					elementLT.lexema = LEX_T;
					break;

				case LEX_DC:
					if (isHesisForBlockFunction || isBlockInTheCycle)
					{
						isDim = true;
					}
					else
					{
						throw ERROR_THROW_IN(609, elementLT.lineNo - 1, -1);
					}
					break;

				case LEX_COMEBACK:
					isComeback = true;
					break;

				case LEX_EQUAL:
					isEqual = true;
					data_type = idTable.table[lexTable.table[lexTable.size - 1].indexTI].idDataType;
					break;

				case LEX_SEMICOLON:
					if (isParametrsInTheFunction)
					{
						throw  ERROR_THROW_IN(602, elementLT.lineNo, -1); //Ошибка синтаксиса: Неверное выражение
					}

					isStrToN = false;
					isDim = false;
					isEqual = false;
					isStrlen = false;
					break;

				case LEX_LEFTBRACE:
					counterOfLeftBrace++;
					if (isParametrsInTheFunction)
					{
						throw  ERROR_THROW_IN(602, elementLT.lineNo - 1, -1); //Ошибка синтаксиса: Неверное выражение
					}
					isHesisForBlockFunction = true;
					if (counterOfLeftBrace >= 2)
					{
						isBlockInTheCycle = true;
						isHesisForBlockFunction = false;
					}
					break;

				case LEX_RIGHTBRACE:
					if (counterOfLeftBrace >= 2)
					{
						isBlockInTheCycle = false;
						isHesisForBlockFunction = true;
						func_name = func_vision_name;
						counterOfLeftBrace--;
						break;

					}
					if (isHesisForBlockFunction)
					{
						isHesisForBlockFunction = false;
						counterOfLeftBrace--;
					}
					break;

				case LEX_CYCLE:
					isCycle = true;
					isParametrsInTheFunction = true;
					func_name = queue_word.word;
					break;
				}
				break;

			}

			case FST::SYMB_ACTION:
			{
				switch (queue_word.word[0]) {
				case LEX_PLUS:
					elementLT.priority = 2;
					break;

				case LEX_MINUS:
					elementLT.priority = 2;
					break;

				case LEX_MULTIPLY:
					elementLT.priority = 3;
					break;
				case LEX_AND:
					elementLT.priority = 10;
					break;
				case LEX_NOT:
					elementLT.priority = 11;
					break;
				case LEX_OR:
					elementLT.priority = 12;
				}

				//Semantical analysis
				if (isAction)
				{
					throw ERROR_THROW_IN(124, elementLT.lineNo, -1);
				}
				isAction = true;
				elementLT.indexTI = idTable.size;
				idTable.table[idTable.size++] = CreateId(lexTable.size, queue_word.word, IT::IDDATATYPE_MATH, IT::IDTYPE_ACT);
				break;
			}

			case FST::SYMB_STATIC_LIB:
			{
				int index;
				if ((index = IT::IsId(idTable, queue_word.word)) == TI_NULLIDX)
				{
					idTable.table[idTable.size] = CreateId(lexTable.size, queue_word.word, IT::IDDATATYPE_INT, IT::IDTYPE_FUN);
					elementLT.indexTI = idTable.size++;
				}
				else
				{
					elementLT.indexTI = index;
				}
				if (elementLT.lexema == LEX_STRTON)		// для семантики чтобы потом показывать какие типы данных передаются в ф-ию
				{
					isStrToN = true;
				}
				if (elementLT.lexema == LEX_STRLN)		// для семантики
				{
					isStrlen = true;
				}
				isParametrsInTheFunction = true;
				break;
			}
			}
			lexTable.table[lexTable.size++] = elementLT;

			if (lexTable.size > LT_MAXSIZE)
			{
				throw ERROR_THROW(121);
			}

			if (idTable.size > TI_MAXSIZE)
			{
				throw ERROR_THROW(122);
			}

		}
		if (isEntry == false)
		{
			throw ERROR_THROW(613);
		}
		if (counterMain > 1)
		{
			throw ERROR_THROW(710);
		}

		IT::WriteTable(log, idTable);
		//IT::ShowTable(idTable);
		LT::Write(log, lexTable);
		//LT::ShowTable(lexTable);
		tables.idtable = idTable;
		tables.lextable = lexTable;
		cout << "Лексический анализ выполнен без ошибок" << endl;
		return tables;
	}
}
