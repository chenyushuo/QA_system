#ifndef RESTORER_H
#define RESTORER_H

#include <string>

#include "my_define.h"

class Restorer{
private:
    Sentence keywords_;
    Sentence new_tags_;

public:
    Restorer(const std::string &restore_file = restorer_file_path);

    void Process(const Sentence &words, Sentence &tags) const;

    ~Restorer() = default;
};

#endif