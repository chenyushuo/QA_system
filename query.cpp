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
#include "segment_dll.h"
#include "filter.h"

using namespace std;

Query::Query(void *segmentor, Filter filter, Replacer replacer) :
    accurate_(false)
{
    SetColor(stderr, green);
    SetColor(stderr, highlight);
    fprintf(stderr, "请输入关键字：");
    SetColor(stderr, white);
    SetColor(stderr, closed);
    string line;
    getline(cin, line);
    stringstream in(line);
    string term;
    auto func = [&filter](const string &str){return filter.IsVaild(str);};
    while (in >> term){
        if (term == "+" || term == "-" || term == "&" || term == "|" || term == "^"){
            accurate_ = true;
            query_.push_back(vector<string>{term});
            continue;
        }
        vector<string> words;
        segmentor_segment(segmentor, term, words);
        replacer.Process(words);
        auto ptr = stable_partition(words.begin(), words.end(), func);
        words.erase(ptr, words.end());
        if (!words.empty())
            query_.push_back(words);
    }
}


Query::Query(void *segmentor, Filter filter, Replacer replacer,
             const vector<string> &keyword) :
    accurate_(false)
{
    auto func = [&filter](const string &str){return filter.IsVaild(str);};
    for (auto & term : keyword){
        vector<string> words;
        segmentor_segment(segmentor, term, words);
        replacer.Process(words);
        auto ptr = stable_partition(words.begin(), words.end(), func);
        words.erase(ptr, words.end());
        if (!words.empty())
            query_.push_back(words);
    }
}
