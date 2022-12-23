#pragma once
#include "Libraries.h"
#include "LT.h"
#include "IT.h"
#define FUNC_MAX_SIZE 5

namespace Lex
{
	struct LEX
	{
		LT::LexTable lextable;
		IT::IdTable idtable;
	};
	LEX Lexer(IN::IN, Log::LOG log);
}
