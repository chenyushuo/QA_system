#include "query.h"

#include <cstdio>
#include <climits>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

#include "set_color.h"
#include "ltp.h"

using namespace std;

Query::Query(const LTP &ltp) :
    accurate_(false)
{
    SetColor(cerr, green);
    SetColor(cerr, highlight);
    cerr << "请输入关键字：";
    SetColor(cerr, white);
    SetColor(cerr, closed);
    string line;
    getline(cin, line);
    stringstream in(line);
    string term;
    while (in >> term){
        if (term == "+" || term == "-" || term == "&" || term == "|" || term == "^"){
            accurate_ = true;
            query_.push_back(vector<string>{term});
            continue;
        }
        vector<string> words;
        ltp.Segment(term, words, LTP::MODIFY);
        if (!words.empty())
            query_.push_back(words);
    }
}


Query::Query(const LTP &ltp, const Sentence &keyword) :
    accurate_(false)
{
    Sentence words = keyword;
    ltp.Filter(words);
    for (auto & term : words){
        query_.push_back(vector<string>{term});
    }
}

bool Query::in(const std::string &str) const{
    for (auto & term : query_){
        for (auto & word : term){
            //cerr << "word = '" << word << "' '" << str << "'" << endl;
            if (word == str)
                return true;
        }
    }
    return false;
}
