#include "multiqa.h"

#include "ltp.h"

using namespace std;

void Multiqa::process_conj(const string &str)
{

//	cerr << "OK3" << endl;
	string strq;

	ltp_.Segment(str, cws, LTP::MODIFY);
	ltp_.Postag(cws, pos);

	/*engine_cws = segmentor_create_segmentor("/home/cys/ltp/ltp_data/cws.model");
	segmentor_segment(engine_cws, str, cws);
	segmentor_release_segmentor(engine_cws);

	engine_pos = postagger_create_postagger("/home/cys/ltp/ltp_data/pos.model");
	postagger_postag(engine_pos, cws, pos);
 	postagger_release_postagger(engine_pos);*/

 	for(int i = 0; i < (int)pos.size(); i++)
 	{
 		//cout << cws[i] << "_" << pos[i] << " ";
 		if(pos[i] == "c" || pos[i] == "wp")
 		{
 			if(strq != "")
 			{
 				question.push_back(strq);
 				strq = "";
 			}
		}
		else
			strq += cws[i];
	}
	//cout << endl;
	if(strq != "")
		question.push_back(strq);
}

void Multiqa::process_space()
{
	string::size_type last = 0, now = 0;

	while(last < query.size() && (now = query.find(' ', last)) != string::npos)
	{
		if(now == last)
		{
			last = now + 1;
			continue;
		}
		process_conj(query.substr(last, now - last));
		last = now + 1;
	}
	if(last < query.size())
		process_conj(query.substr(last, query.size() - last));
}

Multiqa::Multiqa(const string &que, const LTP &ltp) : query(que), ltp_(ltp)
{
//	cerr << "OK1" << endl;
	process_space();
}

vector<string> & Multiqa::getResult()
{
	return question;
}
