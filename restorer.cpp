#include "restorer.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

TagRestorer::TagRestorer(const std::string &restore_file){
    ifstream fin(restore_file.c_str());
    int idx = 0;
    string line;
    while (getline(fin, line)){
        if (idx % 2 == 0)
            keywords_.push_back(line);
        else
            new_tags_.push_back(line);
        idx ++;
    }
    fin.close();
}

inline static bool istelephone(const string &str){
    for (auto & ch : str){
        if ((isdigit(ch) || ch == '-') == false)
            return false;
    }
    return str.length() >= 8;
}

void TagRestorer::Process(const Sentence &words, Sentence &tags) const{
    for (size_t j = 0; j < keywords_.size(); j ++)
        for (size_t i = 0; i < words.size(); i ++){
            if (words[i] == keywords_[j])
                tags[i] = new_tags_[j];
        }
    for (size_t i = 0; i < words.size(); i ++){
        if (istelephone(words[i]))
            tags[i] = "te";
    }
}
