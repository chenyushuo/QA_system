#include "ask.h"

using namespace std;

vector<vector<string> > Ask::Keys;
vector<pair<string, string> > Ask::QA;
vector<int> Ask::QA_level;

const int TOOLARGE = 2100000000;

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
		
		stringstream ss;
		ss << str;
		
		vector<string> similar;
		while(ss >> str)
			similar.push_back(str);
	
		Keys.push_back(similar);
	}
	
	int state = -1, level;
	while(getline(finQA, str))
	{
		if(str == QUESTION)
		{
			if(state == 1)
			{
				pr = make_pair(str2, str3);
				QA.push_back(pr);
				QA_level.push_back(level);
			}
			getline(finQA, str);
			level = str[0] - '0';
		//	cout << level << endl;
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
		QA_level.push_back(level);
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
		int mini = TOOLARGE;
		
		for(int i = 0; i < (int)QA.size(); i++)
		{
			int num = 0;
			for(int j = 0; j < (int)key.size(); j++)
				if(QA[i].first.find(key[j]) != string::npos)
					num++;

			if(num > maxi || (num == maxi && mini > QA_level[i]))
			{
				maxi = num;
				mini = QA_level[i];
				maxloc = i;
			}
		}
		
		//cout << QA_level[maxloc] << endl;
		return maxloc;
	}
}

Ask::Ask(string que)
{
	for(int i = 0; i < (int)que.size(); i++)
		if(que[i] >= 'a' && que[i] <= 'z')
			que[i] -= 'a' - 'A';
	
	for(int i = 0; i < (int)Keys.size(); i++)
		for(int j = 0; j < (int)Keys[i].size(); j++)
			if(que.find(Keys[i][j]) != string::npos)
			{
				key.push_back(Keys[i][0]);
				break;
			}
			
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

