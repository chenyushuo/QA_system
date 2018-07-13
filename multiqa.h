#ifndef MULTIQA_H
#define MULTIQA_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

class LTP;

class Multiqa
{
private:
	std::string query;
	std::vector<std::string> cws, pos, question;
	const LTP &ltp_;
	//void *engine_cws, *engine_pos;
	void process_conj(const std::string &str);
	void process_space();
public:
	// Multiqa() = default;
	Multiqa(const std::string &que, const LTP &ltp);
	std::vector<std::string> & getResult();
};

#endif
