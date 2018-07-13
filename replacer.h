#ifndef REPLACER_H
#define REPLACER_H

#include <string>
#include <vector>

#include "my_define.h"

class Replacer{
private:
    Paragraph source;
    Paragraph dest;

public:
    Replacer(const std::string &replace_file = replace_file_path);

    void Process(Sentence & vec) const;

    ~Replacer() = default;
};

#endif
