#include "chat.h"

#include <cstdlib>
#include <ctime>
#include <cstring>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const string Chat::appkey_ = "appkey=1f600a9faea74393bf79dcf66e40ed74";
const string Chat::api_ = "api=nli";
const string Chat::timestamp_pre_= "timestamp=";
const string Chat::sign_pre_ = "sign=";
const string Chat::app_secret_ = "640bdcde8b2f42f7bb25671afe897c67";
const string Chat::rq_left_ = "rq={\"data\":{\"input_type\":1,\"text\":\"";
const string Chat::rq_right_ = "\"},\"data_type\":\"stt\"}";
const string Chat::url_pre_ = "https://cn.olami.ai/cloudservice/api?";

void Chat::FixAnswer(std::string &answer){
    auto begin = answer.find("\"result\":\"") + strlen("\"result\":\"");
    auto end = answer.find("\",", begin);
    answer = answer.substr(begin, end - begin);
}

string Chat::Answer(const string &query){
    auto cur_time = time(0) * 1000;
    timestamp_ = timestamp_pre_ + to_string(cur_time);
    
    srand(time(0));
    string suf = to_string((unsigned long)rand() + (unsigned long)(&query));
    string file = "chat" + suf + ".txt";
    
    string sum = app_secret_ + api_ + appkey_ + timestamp_ + app_secret_;
    string command = "echo -n '" + sum + "' | md5sum > " + file;
    auto state = system(command.c_str());
    if (state){
        cerr << "error!" << endl;
        exit(1);
    }
    ifstream fin(file.c_str());
    fin >> sign_;
    sign_ = sign_pre_ + sign_;
    fin.close();
    
    rq_ = rq_left_ + query + rq_right_;
    
    url_ = url_pre_ + appkey_ + "&" + api_ + "&" + timestamp_ + "&" + sign_ + "&" + rq_;
    
    command = "wget -O " + file + " '" + url_ + "' 2> chat.log";
    state = system(command.c_str());
    if (state){
        cerr << "error!" << endl;
        exit(1);
    }
    
    fin.open(file.c_str());
    fin >> answer_;
    FixAnswer(answer_);
    
    command = "rm -rf " + file;
    state = system(command.c_str());
    if (state){
        cerr << "error!" << endl;
        exit(1);
    }
    
    return answer_;
}
