#include "replacer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

Replacer::Replacer(const string &replace_file){
    ifstream fin(replace_file.c_str());
    string line;
    int idx = 0;
    while (getline(fin, line)){
        stringstream in(line);
        istream_iterator<string> iter(in), eof;
        if (idx % 2 == 0){
            source.push_back(vector<string>(iter, eof));
        }
        else{
            dest.push_back(vector<string>(iter, eof));
        }
        idx ++;
    }
}

void Replacer::Process(vector<string> & vec){
    vector<string> tmp;
    for (size_t j = 0; j < source.size(); j ++){
        auto & s = source[j];
        auto & d = dest[j];
        for (size_t i = 0; i + s.size() - 1 < vec.size() ; i ++){
            bool flag = true;
            for (size_t k = 0; k < s.size(); k ++){
                if (vec[i + k] != s[k]){
                    flag = false;
                    break;
                }
            }
            if (flag){
                tmp = vector<string>(vec.begin(), vec.begin() + i);
                for (size_t k = 0; k < d.size(); k ++)
                    tmp.push_back(d[k]);
                for (size_t k = i + s.size(); k < vec.size(); k ++)
                    tmp.push_back(vec[k]);
                vec = tmp;
            }
        }
    }
}
