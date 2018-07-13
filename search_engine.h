#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include <vector>
#include <string>

#include "document.h"
#include "index.h"
#include "ltp.h"
#include "my_define.h"

class Query;
class SearchResult;

class SearchEngine{
public:
    static const double kLimit;

private:
    enum FlagStatus{
        none = 000,
        plus_flag = 001,
        minus_flag = 002,
        and_flag = 004,
        or_flag = 010,
        xor_flag = 020
    };
    static const size_t npos;
    const std::string file_list_;
    Document doc_list_;
    Index title_index_, body_index_;
    const LTP & ltp_;

    static void LoadFile(const size_t idx, const std::string &file_name, Index &index,
                         const LTP &ltp);
    static PostingList BasicAccurateSearch(const Paragraph &query,
                                           const Index &idx);
    static void BasicRankedSearch(const Paragraph &query,
                                  const std::vector<double> &term_frequency,
                                  const Index &idx,
                                  std::vector<double> &final_scores, const double rate);

    PostingList AccurateSearch(const Paragraph &query);
    PostingList RankedSearch(const Paragraph &query,
                             std::vector<double> &scores);

public:
    SearchEngine(const LTP & ltp,
                 const std::string &file_list = file_list_path,
                 const std::string &weight_file = weight_file_path,
                 const std::string &data_file = data_file_path);

    SearchResult Search(const Query &query, std::vector<double> &scores,
                        const size_t limits = 10);

    std::pair<std::string, double> Search(const Sentence &keyword,
                                          const Sentence &query_type);

    std::pair<std::string, double> Search(const std::pair<Sentence, Sentence> &pr){
        return Search(pr.first, pr.second);
    }

    // void Run();

    ~SearchEngine() = default;
};

#endif
