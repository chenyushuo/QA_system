#include <bits/stdc++.h>

#include "queryjr.h"

using namespace std;

int main(int argc, char *args[])
{
	if (argc == 3){
		QueryJR query;
//		ifstream fin(args[2]);
//		if (!fin.is_open()){
//			cout << "服务器出现了一些问题o(╯□╰)o" << endl;
//		}
//		else{
//			string que;
//			string str;
//			while (fin >> str)
//				que += str + " ";
			query.answer(args[2], cout);
//		}
	}
	/*while (true){
		cerr << "请输入您的问题：" << endl;
		string line;
		getline(cin, line);
		query.answer(line, cout);
	}*/
	return 0;
}
