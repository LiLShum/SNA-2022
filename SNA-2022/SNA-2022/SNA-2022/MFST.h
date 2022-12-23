#pragma once
#include "Libraries.h"
#include "GRB.h"
#include "Log.h"
#include "LexicalAnalysis.h"
#include <stack>
using namespace std;

typedef stack<short, deque<short>> MFSTSTACK;

#define NS(n)GRB::Rule::Chain::N(n)
#define TS(n)GRB::Rule::Chain::T(n)
#define ISNS(n) GRB::Rule::Chain::isN(n)

#define MFST_DIAGN_MAXSIZE 2 * ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

#define MFST_TRACE_START(LOG)*log.stream <<"\n\n----------Синтаксический Анализ----------\n"<< endl << setw(4)<<left<<"Шаг"<<":" \
	<< setw(20) << left << "Правило"  \
	<< setw(30) << left << "Входная лента" \
	<< setw(20) << left << "Стек" \
	<< endl;\
			/*	cout << "\n\n----------Синтаксический Анализ----------\n" << endl << setw(4) << left << "Шаг" << ":" \
	<< setw(20) << left << "Правило"  \
	<< setw(30) << left << "Входная лента" \
	<< setw(20) << left << "Стек" \
	<< endl;*/

#define MFST_TRACE1(LOG) *log.stream <<setw(4)<<left<<++FST_TRACE_n<<":" \
	<< setw(20) << left << rule.getCRule(rbuf, nrulechain)  \
	<< setw(30) << left << getCLenta(lbuf, lenta_position) \
	<< setw(20) << left << getCSt(sbuf) \
	<< endl; \
	/*		cout<<setw(4) << left << FST_TRACE_n << ":" \
	<< setw(20) << left << rule.getCRule(rbuf, nrulechain)  \
	<< setw(30) << left << getCLenta(lbuf, lenta_position) \
	<< setw(20) << left << getCSt(sbuf) \
	<< endl;*/

#define MFST_TRACE2(LOG) *log.stream <<setw(4)<<left<<++FST_TRACE_n<<":" \
	<< setw(20) << left << " "  \
	<< setw(30) << left << getCLenta(lbuf, lenta_position) \
	<< setw(20) << left << getCSt(sbuf) \
	<< endl;\
			/*cout <<setw(4)<<left<<FST_TRACE_n<<":" \
	<< setw(20) << left << " "  \
	<< setw(30) << left << getCLenta(lbuf, lenta_position) \
	<< setw(20) << left << getCSt(sbuf) \
	<< endl;*/

#define MFST_TRACE3(LOG) *log.stream<<setw(4)<<left<<++FST_TRACE_n<<":" \
	<< setw(20) << left << " "  \
	<< setw(30) << left << getCLenta(lbuf, lenta_position) \
	<< setw(20) << left << getCSt(sbuf) \
	<< endl; \
	/*	cout<<setw(4)<<left<<FST_TRACE_n<<":" \
	<< setw(20) << left << " "  \
	<< setw(30) << left << getCLenta(lbuf, lenta_position) \
	<< setw(20) << left << getCSt(sbuf) \
	<< endl;*/

#define MFST_TRACE4(LOG, c) *log.stream<<setw(4)<<left<< ++FST_TRACE_n<<": "<<setw(20)<<left<<(c)<<endl;\
								/*cout<<setw(4)<<left<< FST_TRACE_n<<": "<<setw(20)<<left<<(c)<<endl;*/

#define MFST_TRACE5(LOG, c) *log.stream<<setw(4)<<left<< ++FST_TRACE_n<<": "<<setw(20)<<left<<(c)<<endl;\
								/*cout<<setw(4)<<left<<  FST_TRACE_n<<": "<<setw(20)<<left<<(c)<<endl;*/

#define MFST_TRACE6(LOG,c,k) *log.stream<<setw(4)<<left<< ++FST_TRACE_n<<": "<<setw(20)<<left<<(c)<<(k)<<endl;\
								/*cout<<setw(4)<<left<< FST_TRACE_n<<": "<<setw(20)<<left<<(c)<<(k)<<endl;*/

#define MFST_TRACE7(LOG)  *log.stream<<setw(4)<<left<<state.lenta_position<<": "\
	<< setw(20) << left << rule.getCRule(rbuf, state.nrulechain) \
	<< endl; \
	/*	cout<<setw(4)<<left<<state.lenta_position<<": "\
	<< setw(20) << left << rule.getCRule(rbuf, state.nrulechain) \
	<< endl;*/


namespace MFST
{
	struct MfstState				
	{
		short lenta_position;		
		short nrule;				
		short nrulechain;			
		MFSTSTACK st;				
		MfstState();
		MfstState(
			short pposition,		
			MFSTSTACK pst,			
			short pnrulechain		
		);
		MfstState(
			short pposition,		
			MFSTSTACK pst,			
			short pnrule,			
			short pnrulechain		
		);
	};

	struct Mfst						
	{
		enum RC_STEP			
		{
			NS_OK,				
			NS_NORULE,			
			NS_NORULECHAIN,		
			NS_ERROR,			
			TS_OK,				
			TS_NOK,				
			LENTA_END,			
			SURPRISE			
		};
		struct MfstDiagnosis		
		{
			short lenta_position;		
			RC_STEP rc_step;			
			short nrule;				
			short nrule_chain;			
			MfstDiagnosis();
			MfstDiagnosis(				
				short plenta_position,	
				RC_STEP prc_step,		
				short pnrule,			
				short pnrule_chain		
			);
		}
		diagnosis[MFST_DIAGN_NUMBER];		
		GRBALPHABET* lenta;					
		short lenta_position;				
		short nrule{};						
		short nrulechain{};					
		short lenta_size;					
		GRB::Greibach grebach;				
		Lex::LEX lex{};						
		MFSTSTACK st;								
		stack<MfstState> storestate;	
		stack<MfstState> bufstack;
		deque<MfstState> my_deque;
		Mfst();
		Mfst(
			Lex::LEX plex,			    
			GRB::Greibach pgrebach			
		);
		char* getCSt(char* buf);									
		char* getCLenta(char* buf, short pos, short n = 25);	
		char* getDiagnosis(short n, char* buf);					
		bool savestate(const Log::LOG& log);					
		bool reststate(const Log::LOG& log);					
		bool push_chain(				
			GRB::Rule::Chain chain		
		);
		RC_STEP step(const Log::LOG& log);		
		bool start(const Log::LOG& log);		
		bool savediagnosis(
			RC_STEP pprc_step					
		);
		bool printrules(const Log::LOG& log);	
		struct Deducation	
		{
			short size;				
			short* nrules;			
			short* nrulechains;		
			Deducation() { size = 0; nrules = 0; nrulechains = 0; };
		}deducation;
		bool savededucation();		
	};
};
