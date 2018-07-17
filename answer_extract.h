#ifndef ANSWER_EXTRACT_H
#define ANSWER_EXTRACT_H

#include <string>
#include <vector>
#include <unordered_map>

#include "my_define.h"

class SearchResult;
class LTP;

class AnswerExtract{
private:
    static const double alpha_;
    std::unordered_map<std::string, double> answer_map_;
    double score_;
    std::string answer_;

    static void calc(const std::string &type, const double &rate,
                     const Sentence &words,
                     const SentenceFlag &flag,
                     std::unordered_map<std::string, double> &answer_map,
                     const LTP &ltp);

public:
    AnswerExtract(const std::vector<double> &scores,
                  const SearchResult &result,
                  const std::string &query_type,
                  const LTP &ltp);

    std::string answer() const{return answer_;}
    double score() const{return score_;}
    void Fix(){answer_ += "\n";}
    
    void SetAnswer(const Paragraph &para);
    void SetScore(const double &value){score_ = value;}

    ~AnswerExtract() = default;
};

#endif
