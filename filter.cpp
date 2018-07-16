#include "filter.h"

#include <iostream>
#include <fstream>

using namespace std;

StopWordsFilter::StopWordsFilter(const std::string &stop_word){
    ifstream fin(stop_word.c_str());
    if (!fin.is_open()){
        cerr << "can't open stop word!" << endl;
    }
    else{
        string line;
        while (getline(fin, line)){
            stop_words_.insert(line);
        }
        fin.close();
    }
}

SensitiveWordsFilter::SensitiveWordsFilter(const std::string &sensitive_words){
    ifstream fin(sensitive_words.c_str());
    if (!fin.is_open()){
        cerr << "can't open sensitive word!" << endl;
    }
    else{
        string line;
        while (getline(fin, line)){
            sensitive_words_.insert(line);
        }
        fin.close();
    }
}
