#ifndef ASK_H
#define ASK_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>

#define QUESTION "Question:"
#define ANSWER "Answer:"
#define NOT_FOUND "__Not Found__"

//const string QUESTION = "Question:", ANSWER = "Answer:", NOT_FOUND = "__Not Found__";

class Ask
{
private:
	static std::vector<std::vector<std::string> > Keys;
	static std::vector<std::pair<std::string, std::string> > QA;
	static std::vector<int> QA_level;
	std::vector<std::string> key;
	int search();
public:
	Ask() = default;
	Ask(std::string que);
	std::string getResult();
	static void initialize(std::ifstream &finKeys, std::ifstream &finQA);
};

#endif
