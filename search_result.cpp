#include "search_result.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "index.h"
#include "document.h"
#include "query.h"
#include "set_color.h"
#include "my_define.h"

using namespace std;

void SearchResult::Read(const string &file_name, const Query &query,
                        Passage &str_vec,
                        PassageFlag &flag_vec, bool inital)
{
    ifstream fin(file_name.c_str());
    string line;
    Paragraph selected_words;
    ParagraphFlag selected_flag;
    while (getline(fin, line)){
        stringstream in(line);
        istream_iterator<string> iter(in), eof;
        Sentence words(iter, eof);
        SentenceFlag flag(words.size(), none);
        bool marked = inital;
        for (auto & keywords : query.query()){
            if (keywords.size() == 1){
                auto & key = keywords[0];
                if (key == "+" || key == "-" || key == "&" || key == "|" || key == "^")
                    continue;
            }

            for (auto p = keywords.begin(); p < keywords.end(); p ++){
                auto ptr = search(words.begin(), words.end(), p, p + 1);
                auto length = 1;
                while (ptr != words.end()){
                    marked = true;
                    fill_n(flag.begin() + (ptr - words.begin()), length, highlight);
                    ptr = search(ptr + length, words.end(), p, p + 1);
                }
            }
        }
        if (marked){
            selected_words.push_back(words);
            selected_flag.push_back(flag);
        }
    }
    str_vec.push_back(selected_words);
    flag_vec.push_back(selected_flag);
    fin.close();
}

SearchResult::SearchResult(const PostingList &post, const Query &query,
                           const Document &doc_list, size_t limits) :
    saved_number_(0), total_number_(post.posting().size()), kDisplayLimits_(limits)
{
    for (auto & doc : post.posting()){
        if (saved_number_ == limits)
            break;
        saved_number_ ++;
        string doc_name = doc_list[doc.docID()];
        Read(doc_name + ".title", query, title_, title_flag_, true);
        Read(doc_name + ".body", query, snippets_, snippets_flag_, false);
        doc_.push_back(doc_name);
    }
}

void SearchResult::Write(ostream &os, const string &type,
                         const Paragraph &str_vec, const ParagraphFlag &flag_vec)
{
    SetColor(os, Color::highlight);
    SetColor(os, blue);
    os << type;
    SetColor(os, closed);
    for (size_t i = 0; i < str_vec.size(); i ++){
        for (size_t j = 0; j < str_vec[i].size(); j ++){
            if (flag_vec[i][j]){
                SetColor(os, red);
                SetColor(os, Color::highlight);
            }
            else{
                SetColor(os, white);
                SetColor(os, closed);
            }
            if (j != 0 && isalpha(str_vec[i][j][0]))
                os << ' ';
            os << str_vec[i][j];
        }
        SetColor(os, white);
        SetColor(os, closed);
        os << endl;
    }
    SetColor(os, white);
    os << endl;
}

ostream & operator << (ostream &os, const SearchResult &result){
    ResetScreen(os);
    SetColor(os, yellow);
    os << "总计" << result.total_number_ << "个文件符合条件" << endl;
    for (size_t i = 0; i < result.saved_number_; i ++){
        SetColor(os, Color::highlight);
        SetColor(os, blue);
        os << "文件名:";
        SetColor(os, closed);
        SetColor(os, white);
        os << result.doc_[i] << endl << endl;
        SearchResult::Write(os, "标题：", result.title_[i], result.title_flag_[i]);
        SearchResult::Write(os, "正文：", result.snippets_[i], result.snippets_flag_[i]);
        for (size_t j = 0; j < kScreenWidth; j ++)
            os << '-';
        os << endl;
    }
    SetColor(os, white);
    SetColor(os, closed);
    return os;
}
