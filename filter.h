#ifndef FILTER_H
#define FILTER_H

#include <string>
#include <unordered_set>
#include <iostream>

#include "my_define.h"

class StopWordsFilter{
private:
    std::unordered_set<std::string> stop_words_;

public:
    StopWordsFilter(const std::string &stop_word = stop_word_path);
    bool IsVaild(const std::string &word) const{
        return stop_words_.find(word) == stop_words_.end();
    }

    ~StopWordsFilter() = default;
};

#endif