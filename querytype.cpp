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

	vector<string>::iterator iter_cws = cws.begin();
	vector<string>::iterator iter_pos = pos.begin();

	while(iter_cws != cws.end())
	{
		if(!((*iter_pos).find('n') != string::npos || *iter_pos == "v"))
		{
			iter_cws = cws.erase(iter_cws);
			iter_pos = pos.erase(iter_pos);
		}
		else
		{
			iter_cws++;
			iter_pos++;
		}
	}
}

pair<vector<string>, vector<string> > Querytype::getResult()
{
	int maxitimes = 0;

	string type = "n";

	for (auto iter = Que.begin(); iter != Que.end(); iter++)
	{
		int times = 0;
		for(int i = 0; i < (int)iter->second.size(); i++)
			if(_query.find(iter->second[i]) != string::npos)
				times++;

		if(times > maxitimes)
		{
			maxitimes = times;
			type = iter->first;
		}
	}

	return make_pair(cws, vector<string>{type});
}

