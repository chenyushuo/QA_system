#include "ask.h"

using namespace std;

vector<string> Ask::Keys;
vector<pair<string, string> > Ask::QA;

void Ask::initialize(ifstream &finKeys, ifstream &finQA)
{
	string str, str2, str3;
	pair<string, string> pr;

	Keys.clear();
	QA.clear();

	while(getline(finKeys, str))
	{
		for(int i = 0; i < (int)str.size(); i++)
			if(str[i] >= 'a' && str[i] <= 'z')
				str[i] -= 'a' - 'A';
		Keys.push_back(str);
	}

	int state = -1;
	while(getline(finQA, str))
	{
		if(str == QUESTION)
		{
			if(state == 1)
			{
				pr = make_pair(str2, str3);
				QA.push_back(pr);
			}
			str2 = "";
			state = 0;
		}
		else if(str == ANSWER)
		{
			str3 = "";
			state = 1;
		}
		else
		{
			if(state == 0)
			{
				for(int i = 0; i < (int)str.size(); i++)
					if(str[i] >= 'a' && str[i] <= 'z')
						str[i] -= 'a' - 'A';
				str2 += str;
				str2 += "\n";
			}
			else if(state == 1)
			{
				str3 += str;
				str3 += "\n";
			}
		}
	}
	if(state == 1)
	{
		pr = make_pair(str2, str3);
		QA.push_back(pr);
	}

//	cerr << QA.size() << endl << Keys.size() << endl;
}

int Ask::search()
{
	if(key.size() == 0)
		return -1;
	else
	{
		int maxi = 0, maxloc = 0;
		double maxi2 = 0.0;

		for(int i = 0; i < (int)QA.size(); i++)
		{
			int num = 0;
			for(int j = 0; j < (int)key.size(); j++)
				if(QA[i].first.find(key[j]) != string::npos)
					num++;

			if(num > maxi || (num == maxi && (double)num / QA[i].first.size() > maxi2))
			{
				maxi = num;
				maxi2 = (double)num / QA[i].first.size();
				maxloc = i;
			}
		}

		return maxloc;
	}
}

Ask::Ask(string que)
{
	for(int i = 0; i < (int)que.size(); i++)
		if(que[i] >= 'a' && que[i] <= 'z')
			que[i] -= 'a' - 'A';

	for(int i = 0; i < (int)Keys.size(); i++)
		if(que.find(Keys[i]) != string::npos)
			key.push_back(Keys[i]);
}

string Ask::getResult()
{
	int res_int = search();
	string res_str;

	if(res_int < 0)
		res_str = NOT_FOUND;
	else
		res_str = QA[res_int].second;

	return res_str;
}

