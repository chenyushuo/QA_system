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
	Ask ask(que);
	if(ask.getResult() == NOT_FOUND)
	{
		Querytype qt(que, ltp_);
		pair<string, double> result = search_engine_.Search(qt.getResult());
		if (result.second > SearchEngine::kLimit){
			out << result.first;
		}
		else{
		    out << Chat(que);
		}
	}
	else{
		out << ask.getResult();
	}
}
