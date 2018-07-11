#ifndef FILTER_H
#define FILTER_H

#include <string>
#include <unordered_set>
#include <iostream>

class Filter{
private:
    std::unordered_set<std::string> stop_words_;

public:
    Filter(const std::string &stop_word = "stop_words.txt");
    bool IsVaild(const std::string &word) const{
        return stop_words_.find(word) == stop_words_.end();
    }

    ~Filter() = default;
};

#endif