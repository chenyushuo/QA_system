#include "answer_extract.h"

#include <cctype>

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "search_result.h"
#include "ltp.h"
#include "my_define.h"

using namespace std;

const double AnswerExtract::alpha_ = 1.0;

static inline double sqr(const double &x){return x * x;}

static inline bool empty(const string &str){
    for (auto & ch : str)
        if (!isspace(ch))
            return false;
    return true;
}

static inline bool check_type(const string &type, const string &tag){
    if (type == "n")
        return tag.find("n") == 0;
    return type == tag;
}

void AnswerExtract::calc(const string &type, const double &rate,
                         const Sentence &words,
                         const SentenceFlag &flag,
                         unordered_map<string, double> &answer_map,
                         const LTP &ltp)
{
    Sentence tags;
    ltp.Postag(words, tags);

    vector<size_t> mark;
    for (size_t i = 0; i < flag.size(); i ++)
        if (flag[i] == SearchResult::highlight){
            mark.push_back(i);
        }

    for (size_t i = 0; i < words.size(); i ++){
        const string &word = words[i];
        if (check_type(type, tags[i]) && !empty(word)){
            double value = 0;
            for (size_t j = 0; j < mark.size(); j ++){
                size_t idx = j;
                if (mark[idx] < i){
                    while (mark[idx + 1] <= i && mark[idx] + 1 == mark[idx + 1])
                        idx ++;
                }
                else{
                    while (mark[idx - 1] >= i && mark[idx] - 1 == mark[idx - 1])
                        idx --;
                }
                double dist = (mark[idx] < i) ? i - mark[idx] : mark[idx] - i;
                value += sqr(alpha_ / (dist + alpha_));
            }

            auto iter = answer_map.find(word);
            if (iter == answer_map.end())
                iter = answer_map.insert(make_pair(word, 0)).first;
            iter -> second += rate * value;
        }
    }
}

AnswerExtract::AnswerExtract(const vector<double> &scores,
                             const SearchResult &result,
                             const string &query_type,
                             const LTP &ltp) :
    score_(0)
{
    for (size_t i = 0; i < result.size(); i ++){
        double score = scores[i] / sqrt(i + 1);
        for (size_t j = 0; j < result.title_[i].size(); j ++){
            calc(query_type, score * 0.6, result.title_[i][j],
                result.title_flag_[i][j], answer_map_, ltp);
        }
        for (size_t j = 0; j < result.snippets_[i].size(); j ++){
            calc(query_type, score * 0.4, result.snippets_[i][j],
                result.snippets_flag_[i][j], answer_map_, ltp);
        }
    }

    vector<pair<double, string> > vec;
    for (auto & ele : answer_map_){
        string str = ele.first;
        double value = ele.second;
        if (ltp.IsVaildAnswer(str)){
            if (score_ < ele.second){
                answer_ = str;
                score_ = value;
            }
            vec.push_back(make_pair(value, str));
        }
    }
    sort(vec.rbegin(), vec.rend());
    /*for (size_t i = 0; i < 10 && i < vec.size(); i ++){
        cout << vec[i].first << ' ' << vec[i].second << endl;
    }*/

    if (query_type != "ns" && query_type != "nh" && query_type != "te")
        score_ = 0;
    answer_ += "\n";
}

void AnswerExtract::SetAnswer(const Paragraph &para){
    answer_.clear();
    for (auto & term : para){
        for (auto & word : term){
            if (isalpha(word[0]))
                answer_ += " ";
            answer_ += word;
        }
        answer_ += "\n";
    }
}
