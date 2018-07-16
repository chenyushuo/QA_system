#include <cstdlib>
#include <cctype>
#include <cstring>
#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>

#include "chat.h"

using namespace std;

static string api = "api=nli";
static string AppKey = "appkey=2f3efbfb6df54f9dba37fd01f73f4cd1";
static string AppSecret = "02e2365deded41029be83bf5cd88c41b";
static string url_pre = "https://cn.olami.ai/cloudservice/api?";
static string rq_pre = "rq={\"data\":{\"input_type\":1,\"text\":\"";
static string rq_suf = "\"},\"data_type\":\"stt\"}";

string Chat(const string & question){
	srand(rand() + time(0) + (long long)(&question));
	long long curtime = time(0) * 1000LL;
	int echo;
	string file_suf = to_string(rand());
	string timestamp = "timestamp=" + to_string(curtime);

	string origin_key = AppSecret + api + AppKey + timestamp + AppSecret;
	string md5_file = "md5" + file_suf + ".txt";
	string command = "echo -n '" + origin_key + "' | md5sum > " + md5_file;
	echo = system(command.c_str());

	ifstream mdin;
	mdin.open(md5_file.c_str());
	string sign;
	mdin >> sign;
	sign = "sign=" + sign;

	string rq = rq_pre + question + rq_suf;
	string temp;
	string answer = "";
    string request = url_pre + AppKey + "&" + api + "&" + timestamp +
                    "&" + sign + "&" + rq;
	//cout<<request<<endl;
	string answer_file = "answer" + file_suf + ".txt";
	command = "wget '" + request + "' -O " + answer_file + " 2> wget.txt";
	echo = system(command.c_str());
	ifstream anin;
	anin.open(answer_file.c_str());
	while (anin >> temp) {
		answer = answer + temp;
	}
	//cerr << "answer = " << answer << endl;
	if (answer.find("对不起，你说的我还不懂，能换个说法吗") != string::npos) {
	    echo = system(("rm " + answer_file).c_str());
	    echo = system(("rm " + md5_file).c_str());
	    echo = system("rm wget.txt");
		return "RUC小图灵想呀想还是没想出来ww，求轻虐_(:3 」∠)_\n";
	}
	int b;
	while ((b = answer.find("欧拉蜜")) != string::npos) {
		answer.replace(b, strlen("欧拉蜜"), "RUC小图灵");
		b = b + strlen("欧拉蜜");
	}
	while ((b = answer.find("主人，")) != string::npos) {
		answer.replace(b, strlen("主人，"), "");
	}
	//cout<<answer<<endl;
	string ret_str = "";
	b = answer.find("result\":\"");
	if (b != string::npos) {
		int i = b + strlen("result\":\"");
		while (answer[i] != '"') {
			if (answer[i] == '\\' && answer[i + 1] == 'n') {
				//cout << endl;
				ret_str = ret_str + '\n';
				i++;
			}
			else
				ret_str = ret_str + answer[i];
			i++;
		}
		ret_str = ret_str + '\n';
	}
	b = answer.find("content\":\"");
	if (b != string::npos) {
		int i = b + 10;
		while (answer[i] != '"') {
			if (answer[i] == '\\' && answer[i + 1] == 'n') {
				ret_str = ret_str + '\n';
				i++;
			}
			else
				ret_str = ret_str + answer[i];
			i++;
		}
		ret_str = ret_str + '\n';
	}
	b = answer.find("description\":\"");
	if (b != string::npos) {
		int i = b + 14;
		while (answer[i] != '"') {
			if (answer[i] == '\\' && answer[i + 1] == 'n') {
				ret_str = ret_str + '\n';
				i++;
			}
			else
				ret_str = ret_str + answer[i];
			i++;
		}
		ret_str = ret_str + '\n';
	}
	b = 0;
	int cnt = 0;
	while ((b = answer.find("detail\":\"", b)) != string::npos) {
		cnt++;
		//cout << cnt << endl;
		ret_str = ret_str + (char)(cnt + '0') + '\n';
		int i = b + 9;
		while (answer[i] != '"') {
			if (answer[i] == '\\' && answer[i + 1] == 'n') {
				ret_str = ret_str + '\n';
				i++;
			}
			else
				if (isspace(answer[i]) == false)
					ret_str = ret_str + answer[i];
			i++;
		}
		ret_str = ret_str + '\n';
		b = i;
		b = answer.find("ref_url\":\"", b);
		if (b != string::npos) {
			i = b + 10;
			while (answer[i] != '"') {
				if (answer[i] == '\\' && answer[i + 1] == 'n') {
					ret_str = ret_str + '\n';
					i++;
				}
				else
					if (isspace(answer[i]) == false)
						ret_str = ret_str + answer[i];
				i++;
			}
			ret_str = ret_str + '\n';
			b = i;
		}
	}
	b = answer.find("url\":\"");
	if (b != string::npos) {
		int i = b + 6;
		while (answer[i] != '"') {
			if (answer[i] == '\\' && answer[i + 1] == 'n') {
				ret_str = ret_str + '\n';
				i++;
			}
			else
				ret_str = ret_str + answer[i];
			i++;
		}
		ret_str = ret_str + '\n';
	}
	echo = system(("rm " + answer_file).c_str());
	echo = system(("rm " + md5_file).c_str());
	echo = system("rm wget.txt");
	return ret_str;
}
