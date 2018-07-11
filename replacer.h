#ifndef REPLACER_H
#define REPLACER_H

#include <string>
#include <vector>

class Replacer{
private:
    std::vector<std::vector<std::string> > source;
    std::vector<std::vector<std::string> > dest;

public:
    Replacer(const std::string &replace_file = "replace.txt");
    
    void Process(std::vector<std::string> & vec);
    
    ~Replacer() = default;
};

#endif
