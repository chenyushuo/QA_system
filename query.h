#ifndef QUERY_H
#define QUERY_H

#include <string>
#include <vector>

#include "my_define.h"

class LTP;

class Query{
private:
    bool accurate_;
    Paragraph query_;

public:
    Query(const LTP &ltp);
    Query(const LTP &ltp, const Sentence &keyword);

    bool accurate() const{return accurate_;}
    const Paragraph & query() const{return query_;}

    ~Query() = default;
};

#endif
