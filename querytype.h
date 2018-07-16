#ifndef QUERYTYPE_H
#define QUERYTYPE_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <sstream>

#include "my_define.h"

class LTP;

class Querytype
{
private:
	static std::vector<std::pair<std::string, Sentence> > Que;
	static std::pair<std::string, Sentence> pr;
	Sentence cws, pos;
	std::string _query;
	const LTP &ltp_;
public:
	static void initialize();
	// Querytype() = default;
	Querytype(const std::string &que, const LTP &ltp);
	std::pair<Sentence, std::string> getResult();
};

#endif
