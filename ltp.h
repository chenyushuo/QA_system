#ifndef LTP_H
#define LTP_H

#include <vector>
#include <string>

#include "filter.h"
#include "replacer.h"
#include "restorer.h"
#include "my_define.h"

class LTP{
public:
    enum MODE{ORIGIN, MODIFY};

private:
    void *segmentor_;
    void *postagger_;
    StopWordsFilter filter_;
    Replacer replacer_;
    Restorer restorer_;

public:
    LTP(const std::string &stop_words_file = stop_word_path,
        const std::string &replace_file = replace_file_path,
        const std::string &restorer_file = restorer_file_path);

    void Replace(Sentence &words) const;
    void Filter(Sentence &words) const;
    void Restore(const Sentence &words, Sentence &tags) const;
    void Segment(const std::string &term, Sentence &words, MODE mode = ORIGIN) const;
    void Postag(const Sentence &words, Sentence &tags, MODE mode = MODIFY) const;

    ~LTP();
};

#endif