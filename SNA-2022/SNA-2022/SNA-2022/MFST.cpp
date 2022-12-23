#include "Libraries.h"
#include "MFST.h"
#include <functional>

using namespace std;
int FST_TRACE_n = -1;
char rbuf[205], sbuf[205], lbuf[1024];

namespace MFST
{
	int counterOfErrors = 0;
	MFST::MfstState::MfstState() 
	{
		lenta_position = 0;		
		nrule = -1;				
		nrulechain = -1;		
	};
	MfstState::MfstState(short pposition, MFSTSTACK pst, short pnrulechain) 
	{
		lenta_position = pposition;
		st = pst;				
		nrulechain = pnrulechain;	
	};
	MfstState::MfstState(short pposition, MFSTSTACK pst, short pnrule, short pnrulechain) 
	{
		lenta_position = pposition;	
		st = pst;					
		nrule = pnrule;			
		nrulechain = pnrulechain;
	};
	
	Mfst::MfstDiagnosis::MfstDiagnosis()
	{
		lenta_position = -1;
		rc_step = SURPRISE;
		nrule = -1;
		nrule_chain = -1;
	};

	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)
	{
		lenta_position = plenta_position;	
		rc_step = prc_step;					
		nrule = pnrule;						
		nrule_chain = pnrule_chain;			
	};
	Mfst::Mfst() { lenta = 0; lenta_size = lenta_position = 0; };	
	Mfst::Mfst(Lex::LEX plex, GRB::Greibach pgrebach)				
	{
		grebach = pgrebach; 
		lex = plex; 
		lenta = new short[lenta_size = lex.lextable.size];
		for (int k = 0; k < lenta_size; k++)lenta[k] = TS(lex.lextable.table[k].lexema);
		lenta_position = 0;
		st.push(grebach.stbottomT);
		st.push(grebach.startN);
		nrulechain = -1;
	};
	Mfst::RC_STEP Mfst::step(const Log::LOG& log) 
	{
		RC_STEP rc = SURPRISE; 
		if (lenta_position < lenta_size)
		{
			if (ISNS(st.top())) 
			{
				GRB::Rule rule;
				if ((nrule = grebach.getRule(st.top(), rule)) >= 0)
				{
					GRB::Rule::Chain chain;
					if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0)
					{
						MFST_TRACE1(log)
							savestate(log);
						st.pop();
						push_chain(chain); 
						rc = NS_OK; 
						MFST_TRACE2(log)
					}
					else
					{
						MFST_TRACE4(log, "TNS_NORULECHAIN/NS_NORULE")
							savediagnosis(NS_NORULECHAIN); 
						rc = reststate(log) ? NS_NORULECHAIN : NS_NORULE; 
					};
				}
				else rc = NS_ERROR;
			}
			else if ((st.top() == lenta[lenta_position]))
			{
				lenta_position++; 
				st.pop();
				nrulechain = -1; 
				rc = TS_OK;
				MFST_TRACE3(log)
			}
			else { MFST_TRACE4(log, "TS_NOK/NS_NORULECHIN") rc = reststate(log) ? TS_NOK : NS_NORULECHAIN; };
		}
		else { rc = LENTA_END; MFST_TRACE4(log, "LENTA_END") };
		return rc;
	};

	bool Mfst::push_chain(GRB::Rule::Chain chain)
	{
		for (int k = chain.chainLenght - 1; k >= 0; k--)st.push(chain.nt[k]);
		return true;
	};
	bool Mfst::savestate(const Log::LOG& log) 
	{
		storestate.push(MfstState(lenta_position, st, nrule, nrulechain)); 
		bufstack.push(MfstState(lenta_position, st, nrule, nrulechain));
		MFST_TRACE6(log, "SAVESTATE:", storestate.size());
		return true;
	};
	bool Mfst::reststate(const Log::LOG& log)
	{
		bool rc = false;
		MfstState state;
		if (rc = (storestate.size() > 0))
		{
			state = storestate.top();
			lenta_position = state.lenta_position;
			st = state.st;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop();
			MFST_TRACE5(log, "RESSTATE")
				MFST_TRACE2(log)
		};
		return rc;
	};
	bool Mfst::savediagnosis(RC_STEP prc_step)
	{
		bool rc = false;
		short k = 0;
		while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)k++;
		if (rc = (k < MFST_DIAGN_NUMBER))
		{
			diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain);
			for (short j = k + 1; j < MFST_DIAGN_NUMBER; j++)diagnosis[j].lenta_position = -1;
		};
		return rc;
	};
	bool Mfst::start(const Log::LOG& log)
	{
		bool rc = false;
		RC_STEP rc_step = SURPRISE;
		char buf[MFST_DIAGN_MAXSIZE];
		rc_step = step(log);
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
			rc_step = step(log);
		switch (rc_step)
		{
		case LENTA_END:         MFST_TRACE4(log, "------>LENTA_END")
			* log.stream << "-------------------------------------------------------------------------------------" << endl;
			//cout << "-------------------------------------------------------------------------------------" << endl;
			//cout << "Successful completion of the syntax analyzer!" << endl;
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d:всего строк %d, синтаксический анализ выполнен без ошибок ", 0, lenta_size);
			*log.stream << setw(4) << left << 0 << ":всего строк " << lenta_size << ", синтаксический анализ выполнен без ошибок " << endl;
			cout << setw(4) << left  << "синтаксический анализ выполнен без ошибок " << endl;
			rc = true;
			break;
		case NS_NORULE:         MFST_TRACE4(log, "------>NS_NORULE")
			cout << "-------------------------------------------------------------------------------------" << endl;
			cout << getDiagnosis(0, buf) << endl;
			cout << getDiagnosis(1, buf) << endl;
			cout << getDiagnosis(2, buf) << endl;
			*log.stream << "-------------------------------------------------------------------------------------" << endl;
			*log.stream << getDiagnosis(0, buf) << endl;
			*log.stream << getDiagnosis(1, buf) << endl;
			*log.stream << getDiagnosis(2, buf) << endl;
			break;

		case NS_NORULECHAIN:       MFST_TRACE4(log, "------>NS_NORULECHAIN") break;
		case NS_ERROR:             MFST_TRACE4(log, "------>NS_ERROR") break;
		case SURPRISE:             MFST_TRACE4(log, "------>SURPRISE") break;
		};
		return rc;

	};

	char* Mfst::getCSt(char* buf)
	{
		stack<short> bufst = st;
		deque<int> my_deque;
		while (!bufst.empty()) {
			my_deque.push_back(bufst.top());
			bufst.pop();
		}
		for (int k = 0; k < static_cast<signed>(st.size()); k++)
		{
			short p = my_deque[k];
			buf[k] = GRB::Rule::Chain::alphabet_to_char(p);
		}
		buf[st.size()] = 0x00;
		return buf;
	};
	char* Mfst::getCLenta(char* buf, short pos, short n)
	{
		short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
		for (i = pos; i < k; i++)buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);
		buf[i - pos] = 0x00;
		return buf;
	};
	char* Mfst::getDiagnosis(short n, char* buf)
	{
		char* rc = new char;
		*rc = '\0';
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{

			errid = grebach.getRule(diagnosis[n].nrule).idDiagnosticError;
			Error::ERROR err = ERROR_THROW_IN(errid, lex.lextable.table[lpos].lineNo, -1);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d,%s", err.id, lex.lextable.table[lpos].lineNo, err.message);
			counterOfErrors++;
			if (counterOfErrors == 3)
			{
				throw err;
			}
			rc = buf;
		};
		return rc;
	};
	bool Mfst::printrules(const Log::LOG& log)
	{
		* log.stream << endl;
		*log.stream << endl;
		*log.stream << "-------------------Дерево разбора-------------------------- " << endl;
		//cout << "-------------------Дерево разбора-------------------------- " << endl;
		MfstState state;
		GRB::Rule rule;
		for (unsigned short k = 0; k < my_deque.size(); k++)
		{
			state = my_deque[k];
			rule = grebach.getRule(state.nrule);
			MFST_TRACE7(log)
		};
		return true;
	};
	bool Mfst::savededucation()
	{
		while (!bufstack.empty()) {
			my_deque.push_back(bufstack.top());
			bufstack.pop();
		}
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = my_deque.size()];
		deducation.nrulechains = new short[deducation.size];
		for (unsigned short k = 0; k < my_deque.size(); k++)
		{
			state = my_deque[k];
			deducation.nrules[k] = state.nrule;
			deducation.nrulechains[k] = state.nrulechain;
		};
		return true;
	};
};