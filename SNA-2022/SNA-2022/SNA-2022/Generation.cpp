#include "Libraries.h"
#include "Generation.h"
#include <stack>

namespace Generation
{
	void Generation(Lex::LEX lex, Log::LOG log)
	{
		*(log.stream) << ".586\n\t.model flat, stdcall\n\tincludelib libucrt.lib\n\tincludelib kernel32.lib\n\tincludelib ../Debug/Lib.lib\n\tExitProcess PROTO :DWORD\n\n";
		*(log.stream) << "\treadw PROTO: DWORD\n\treadr PROTO: DWORD\n\tstrln PROTO: DWORD\n\tstrtonum PROTO: BYTE\n";	
		*(log.stream) << "\n.stack 4096\n";

		*(log.stream) << ".const\n";
		*(log.stream) << "\tnum_err byte \'Error: Переполнение в результате арифметической операции!\', 0\n";
		for (int i = 0; i < lex.idtable.size; i++)			
		{
			if (lex.idtable.table[i].idType == IT::IDTYPE_LIT)
			{
				*(log.stream) << "\t" << lex.idtable.table[i].id;
				if (lex.idtable.table[i].idDataType == IT::IDDATATYPE_STR)
				{
					*(log.stream) << " BYTE " << lex.idtable.table[i].value.strValue.str << ", 0\n";
				}
				if (lex.idtable.table[i].idDataType == IT::IDDATATYPE_INT)
				{
					*(log.stream) << " DWORD " << lex.idtable.table[i].value.intValue << endl;
				}
			}
		}

		*(log.stream) << ".data\n";
		for (int i = 0; i < lex.lextable.size; i++)
		{
			if (lex.lextable.table[i].lexema == LEX_DC)
			{
				*(log.stream) << "\t" << lex.idtable.table[lex.lextable.table[i + 2].indexTI].id;
				*(log.stream) << " DWORD ?\n";
				i += 3;
			}
		}

		stack<string> stk; 
		string func_name = "";					
		bool isHesisForBlockFunctionion = false,			
			isEntry = false,			
			isCycle = false;

		*(log.stream) << "\n.code\n\n";
		for (int i = 0; i < lex.lextable.size; i++)
		{
			switch (lex.lextable.table[i].lexema)
			{
			case LEX_FUNC:
			{
				*(log.stream) << (func_name = lex.idtable.table[lex.lextable.table[++i].indexTI].id) << " PROC ";
				while (lex.lextable.table[i].lexema != LEX_RIGHTHESIS)
				{
					if (lex.idtable.table[lex.lextable.table[i].indexTI].idType == IT::IDTYPE_PAR)
					{
						*(log.stream) << lex.idtable.table[lex.lextable.table[i].indexTI].id << " : ";
						if (lex.idtable.table[lex.lextable.table[i].indexTI].idDataType == IT::IDDATATYPE_INT)
						{
							*(log.stream) << "SDWORD";
						}
						else
						{
							*(log.stream) << "DWORD";
						}
					}
					if (lex.lextable.table[i].lexema == LEX_COMMA)
					{
						*(log.stream) << ", ";
					}
					i++;
				}
				isHesisForBlockFunctionion = true;
				*(log.stream) << endl;
				break;
			}
			case LEX_ENTRY:
			{
				isEntry = true;
				*(log.stream) << "main PROC\n";
				break;
			}
			case LEX_EQUAL:
			{
				int result_position = i - 1;
				while (lex.lextable.table[i].lexema != LEX_SEMICOLON)
				{
					switch (lex.lextable.table[i].lexema)
					{
					case LEX_ID:
					case LEX_NUM:
					{
						*(log.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i].indexTI].id << endl;
						stk.push(lex.idtable.table[lex.lextable.table[i].indexTI].id);
						break;
					}
					case LEX_STR:
					{
						*(log.stream) << "\tpush offset " << lex.idtable.table[lex.lextable.table[i].indexTI].id << endl;
						stk.push("offset " + lex.idtable.table[lex.lextable.table[i].indexTI].id);
						break;
					}
					case LEX_SUBST:
					{
						for (int j = 1; j <= lex.lextable.table[i].priority; j++)
						{
							*(log.stream) << "\tpop edx\n";
						}
						for (int j = 1; j <= lex.lextable.table[i].priority; j++)
						{
							*(log.stream) << "\tpush " << stk.top() << endl;
							stk.pop();
						}
						*(log.stream) << "\tcall " << lex.idtable.table[lex.lextable.table[i].indexTI].id << "\n\tpush eax\n";
						break;
					}
					case LEX_ACTION:
					{
						switch (lex.idtable.table[lex.lextable.table[i].indexTI].id[0])
						{
						case LEX_MULTIPLY:
						{
							*(log.stream) << "\tpop eax\n\tpop ebx\n";
							*(log.stream) << "\tmul eax\n";
							*(log.stream) << "\tcmp eax, 127\n";
							*(log.stream) << "\tjg num_error\n";
							*(log.stream) << "\tcmp eax, -128\n";
							*(log.stream) << "\tjl num_error\n";
							*(log.stream) << "\tpush eax\n";
							break;
						}
						case LEX_PLUS:
						{
							*(log.stream) << "\tpop eax\n\tpop ebx\n";
							*(log.stream) << "\tadd eax, ebx\n";
							*(log.stream) << "\tcmp eax, 127\n";
							*(log.stream) << "\tjg num_error\n";
							*(log.stream) << "\tcmp eax, -128\n";
							*(log.stream) << "\tjl num_error\n";
							*(log.stream) << "\tpush eax\n";
							break;
						}
						case LEX_AND:
						{
							*(log.stream) << "\tpop eax\n\tpop ebx\n";
							*(log.stream) << "\tand eax, ebx\n";
							*(log.stream) << "\tcmp eax, 127\n";
							*(log.stream) << "\tjg num_error\n";
							*(log.stream) << "\tcmp eax, -128\n";
							*(log.stream) << "\tjl num_error\n";
							*(log.stream) << "\tpush eax\n";
							*(log.stream) << "\tpush ebx\n";
							break;
						}
						case LEX_OR:
						{
							*(log.stream) << "\tpop eax\n\tpop ebx\n";
							*(log.stream) << "\tor eax, ebx\n";
							*(log.stream) << "\tcmp eax, 127\n";
							*(log.stream) << "\tjg num_error\n";
							*(log.stream) << "\tcmp ebx, -128\n";
							*(log.stream) << "\tjl num_error\n";
							*(log.stream) << "\tpush eax\n";
							*(log.stream) << "\tpush ebx\n";
							break;
						}
						case LEX_MINUS:
						{
							*(log.stream) << "\tpop ebx\n\tpop eax\n";
							*(log.stream) << "\tsub eax, ebx\n";
							*(log.stream) << "cmp eax, 127\n";
							*(log.stream) << "jg num_error\n";
							*(log.stream) << "cmp eax, -128\n";
							*(log.stream) << "jl num_error\n";
							*(log.stream) << "\tpush eax\n";
							break;
						}
						}
						break;
					case LEX_NOT:
					{
						*(log.stream) << "\tpop eax\n";
						*(log.stream) << "\tnot eax\n";;
						*(log.stream) << "\tcmp eax, 127\n";
						*(log.stream) << "\tjg num_error\n";
						*(log.stream) << "\tcmp eax, -128\n";
						*(log.stream) << "\tjl num_error\n";
						*(log.stream) << "\tpush eax\n";
						break;
					}
						break;
					}
					}
					i++;
				}
				*(log.stream) << "\tpop " << lex.idtable.table[lex.lextable.table[result_position].indexTI].id << "\n";
				if (isCycle)
				{
					*(log.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[result_position].indexTI].id << "\n";
				}
				break;
			}
			case LEX_COMEBACK:
			{
				*(log.stream) << "\tpush ";
				i++;
				if (lex.idtable.table[lex.lextable.table[i].indexTI].idType == IT::IDTYPE_LIT)
				{
					*(log.stream) << lex.idtable.table[lex.lextable.table[i++].indexTI].value.intValue;
					*(log.stream) << "\n\tpop eax\n";
				}
				else
				{
					*(log.stream) << lex.idtable.table[lex.lextable.table[i++].indexTI].id;
				}
				*(log.stream) << "\n\tret\n";
				break;
			}
			case LEX_RIGHTBRACE:
			{
				if (isEntry && !isHesisForBlockFunctionion && !isCycle)
				{
					*(log.stream) << "\tpush 0\n\tcall ExitProcess\nmain ENDP\nend main";
				}
				if (isHesisForBlockFunctionion && !isCycle)
				{
					*(log.stream) << func_name << " ENDP\n\n";
					isHesisForBlockFunctionion = false;
				}
				if (isCycle)
				{
					*(log.stream) << "loop Mark\n";
					isCycle = false;
				}
				break;
			}
			case LEX_READ:
			{
				if (lex.idtable.table[lex.lextable.table[i + 2].indexTI].idDataType == IT::IDDATATYPE_INT)
				{
					*(log.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 2].indexTI].id << "\n\t\tcall readr\n";
				}
				else
				{
					*(log.stream) << "\tpush offset " << lex.idtable.table[lex.lextable.table[i + 2].indexTI].id << "\n\t\tcall readw\n";
				}
				break;
			}
			case LEX_CYCLE:
			{
				while (lex.lextable.table[i].lexema != LEX_RIGHTHESIS)
				{
					if (lex.idtable.table[lex.lextable.table[i].indexTI].idType == IT::IDTYPE_LIT || lex.idtable.table[lex.lextable.table[i].indexTI].idType == IT::IDTYPE_FUN)
					{
						if (lex.idtable.table[lex.lextable.table[i].indexTI].idDataType == IT::IDDATATYPE_INT)
						{
							*(log.stream) << "\tmov ecx, " << lex.idtable.table[lex.lextable.table[i].indexTI].id << "\n";
						}
					}
					i++;
				}
				*(log.stream) << "Mark:\n";
				isCycle = true;
				break;
			}

			}
			
		}
		*(log.stream) << "push 0\ncall ExitProcess\nnum_error::\npush offset num_err\ncall readw\ncall ExitProcess\nmain ENDP\nend main";
	}
}