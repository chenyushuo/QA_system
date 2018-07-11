#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include <iostream>
#include <vector>
#include <string>

#include "index.h"
#include "document.h"
#include "query.h"

typedef std::vector<std::string> Section;
typedef std::vector<Section> Passage;
typedef std::vector<int> Section_flag;
typedef std::vector<Section_flag> Passage_flag;

class SearchResult{
public:
    enum FlagState{none, highlight};

private:
    size_t saved_number_;
    size_t total_number_;
    const size_t kDisplayLimits_ = 0;
    std::vector<std::string> doc_;
    std::vector<Passage> title_;
    std::vector<Passage> snippets_;
    std::vector<Passage_flag> title_flag_;
    std::vector<Passage_flag> snippets_flag_;

    static void Read(const std::string &file_name, const Query &query,
                     std::vector<Passage> &str_vec,
                     std::vector<Passage_flag> &flag_vec, bool inital);
    static void Write(std::ostream &os, const std::string &type,
                      const Passage &str_vec,
                      const Passage_flag &flag_vec);

public:
    SearchResult() = default;
    SearchResult(const PostingList &post, const Query &query,
                 const Document &doc_list, size_t limits = 10);

    size_t size() const{return saved_number_;}
    
    friend class AnswerExtract;

    friend std::ostream & operator << (std::ostream &os, const SearchResult &result);
    /*friend AnswerExtract::AnswerExtract(const std::vector<double> &scores,
                                        const SearchResult &result,
                                        const std::vector<std::string> &query_type,
                                        void *postagger);*/
    // friend const SearchResult & Search

    ~SearchResult() = default;
};
#endif
