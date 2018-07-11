#ifndef ANSWER_EXTRACT_H
#define ANSWER_EXTRACT_H

#include <string>
#include <vector>
#include <map>

class SearchResult;

class AnswerExtract{
private:
    static const double alpha_;
    std::map<std::string, double> answer_map_;
    double score_;
    std::string answer_;

    static void calc(const std::string &type, const double &rate,
                     const std::vector<std::string> &words,
                     const std::vector<int> &flag,
                     std::map<std::string, double> &answer_map,
                     void *postagger);

public:
    AnswerExtract(const std::vector<double> &scores,
                  const SearchResult &result,
                  const std::vector<std::string> &query_type,
                  void *postagger);

    std::string answer() const{return answer_;}
    double score() const{return score_;}

    ~AnswerExtract() = default;
};

#endif
