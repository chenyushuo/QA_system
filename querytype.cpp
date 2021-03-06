#include "querytype.h"

#include "ltp.h"

using namespace std;

vector<pair<string, vector<string> > > Querytype::Que;
pair<string, vector<string> > Querytype::pr;

void Querytype::initialize()
{
	string str, str2;
	ifstream fin(querytype_path);
	while(getline(fin, str))
	{
		vector<string> word;

		getline(fin, str2);
		stringstream ss;
		ss << str2;

		while(ss >> str2)
			word.push_back(str2);

		pr = make_pair(str, word);
		Que.push_back(pr);
	}
	fin.close();
}

Querytype::Querytype(const string &que, const LTP &ltp) : _query(que), ltp_(ltp)
{
	ltp_.Segment(_query, cws);
	ltp_.Postag(cws, pos);
}

pair<vector<string>, string> Querytype::getResult()
{
	int maxitimes = 0;
	bool tele = false;

	string type = "n";

	for(int i = 0; i < (int)Que[0].second.size(); i++)
		if(_query.find(Que[0].second[i]) != string::npos)
		{
			type = Que[0].first;
			tele = true;
			break;
		}

	for (auto iter = Que.begin(); iter != Que.end(); iter++)
	{
		int times = 0;
		for(int i = 0; i < (int)iter->second.size(); i++){
		    if (find(cws.begin(), cws.end(), iter -> second[i]) != cws.end())
				times++;
		}

		if(!tele && times > maxitimes)
		{
			maxitimes = times;
			type = iter->first;
		}
	}

    ltp_.Filter(cws);
	return make_pair(cws, type);
}

