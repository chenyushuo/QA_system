#include "queryjr.h"

#include <iostream>

#include "ask.h"
#include "multiqa.h"
#include "querytype.h"
#include "search_engine.h"
#include "ltp.h"
#include "chat.h"

using namespace std;

QueryJR::QueryJR() :
	ltp_(), search_engine_(ltp_)
{
	ifstream finKeys(speckey_path), finQA(qa_path);
	Ask::initialize(finKeys, finQA);
	finKeys.close();
	finQA.close();
	Querytype::initialize();
}

void QueryJR::answer(const string &que, ostream &out)
{
	string query = que;
	Multiqa multiqa = Multiqa(query, ltp_);
	vector<string> question = multiqa.getResult();

	//out << endl;
	if(question.size() > 0)
	{
		if(question.size() > 1){
		    out << "聪明的RUC小图灵感觉您问了" << question.size() << "个问题"
		        << endl << endl;
	    }
		for(int i = 0; i < (int)question.size(); i++)
		{
			if (question.size() > 1)
			    out << question[i] << ":" << endl;
			Ask ask(question[i]);
			if(ask.getResult() == NOT_FOUND)
			{
				Querytype qt(question[i], ltp_);
				pair<string, double> result = search_engine_.Search(qt.getResult());
				if (result.second > SearchEngine::kLimit){
					out << result.first << endl << endl;
				}
				else{
				    out << chat_.Answer(query) << endl;
					//out << "闲聊" << endl;
				}
			}
			else{
				out << ask.getResult() << endl << endl;
			}
		}
	}
	else
	{
		Querytype qt(query, ltp_);
		pair<string, double> result = search_engine_.Search(qt.getResult());
		if (result.second > SearchEngine::kLimit){
		    out << result.first << endl << endl;
		}
		else{
		    out << chat_.Answer(query) << endl;
		    //out << "闲聊" << endl;
		}
	}
}
