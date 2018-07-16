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

class SensitiveWordsFilter{
private:
    std::unordered_set<std::string> sensitive_words_;
public:
    SensitiveWordsFilter(const std::string &sensitive_words = sensitive_words_path);
    bool IsVaild(const std::string &word) const{
        return sensitive_words_.find(word) == sensitive_words_.end();
    }

    ~SensitiveWordsFilter() = default;
};

#endif
