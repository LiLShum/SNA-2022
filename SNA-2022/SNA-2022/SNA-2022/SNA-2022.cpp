#include "Libraries.h"
#include "LexicalAnalysis.h"
#include "MFST.h"
#include "Generation.h"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;										
	try																	
	{
		Parm::PARM parm = Parm::getparm(argc, argv);					
		log = Log::GetLog(parm.log);									
		Log::WriteLog(log);											
		Log::WriteParm(log, parm);

		IN::IN in = IN::GetIn(parm.in);								
		Log::WriteIn(log, in);

		log = Log::GetLog(parm.lex);
		char header[] = "Lexical analisis\n";
		Log::WriteLine(log, header, "");
		Lex::LEX lex = Lex::Lexer(in, log);								

		log = Log::GetLog(parm.sin);
		MFST_TRACE_START(log);
		MFST::Mfst mfst(lex, GRB::getGreibach());
		mfst.start(log);
		mfst.savededucation(); //��� ������ �������
		mfst.printrules(log);

		CallPolishNotation(&lex.lextable, &lex.idtable);
		LT::Write(log, lex.lextable);
		IT::WriteTable(log, lex.idtable);
		//cout << endl << "-------------------����� ������ �������� �������-------------------------- " << endl;
		//LT::ShowTable(lex.lextable);

		log = Log::GetLog(parm.out);
		Generation::Generation(lex, log);

		cout << "\n������ ������� ���������\n";
		Log::Close(log);
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
	}
}