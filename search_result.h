#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include <iostream>
#include <vector>
#include <string>

#include "my_define.h"

class PostingList;
class Index;
class Document;
class Query;

class SearchResult{
public:
    enum FlagState{none, highlight};

private:
    size_t saved_number_;
    size_t total_number_;
    const size_t kDisplayLimits_ = 0;
    Sentence doc_;
    Passage title_;
    Passage snippets_;
    PassageFlag title_flag_;
    PassageFlag snippets_flag_;

    static void Read(const std::string &file_name, const Query &query,
                     Passage &str_vec,
                     PassageFlag &flag_vec, bool inital);
    static void Write(std::ostream &os, const std::string &type,
                      const Paragraph &str_vec,
                      const ParagraphFlag &flag_vec);

public:
    SearchResult() = default;
    SearchResult(const PostingList &post, const Query &query,
                 const Document &doc_list, size_t limits = 10);

    size_t size() const{return saved_number_;}
    const Paragraph & first() const{return snippets_[0];}
    const Paragraph ReloadFirst() const;

    friend class AnswerExtract;

    friend std::ostream & operator << (std::ostream &os, const SearchResult &result);

    ~SearchResult() = default;
};
#endif
