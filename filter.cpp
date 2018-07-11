#include "filter.h"

#include <iostream>
#include <fstream>

using namespace std;

Filter::Filter(const std::string &stop_word){
    ifstream fin(stop_word.c_str());
    if (!fin.is_open()){
        fprintf(stderr, "can't open stop word!\n");
    }
    else{
        string line;
        while (getline(fin, line)){
            stop_words_.insert(line);
        }
        fin.close();
    }
}