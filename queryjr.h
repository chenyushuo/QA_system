#ifndef QUERYJR_H
#define QUERYJR_H

#include <iostream>
#include <string>

#include "search_engine.h"
#include "ltp.h"
#include "chat.h"

class QueryJR
{
private:
	LTP ltp_;
	SearchEngine search_engine_;

public:
	QueryJR();
	void answer(const std::string &que, std::ostream &out);

};

#endif
