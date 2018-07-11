#include "search_engine.h"

#include <cmath>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <functional>

#include "search_result.h"
#include "segment_dll.h"
#include "postag_dll.h"
#include "set_color.h"
#include "kbhit.h"
#include "answer_extract.h"

using namespace std;

const size_t SearchEngine::npos = -1;

void SearchEngine::LoadFile(const size_t idx, const string &file_name, Index &index,
                            const Filter &filter)
{
    ifstream in(file_name.c_str());
    if (!in.is_open()){
        fprintf(stderr, "can't open %s !\n", file_name.c_str());
    }
    else{
        istream_iterator<string> iter(in), eof;
        vector<string> words(iter, eof);
        auto func = [&filter](const string &str){return filter.IsVaild(str);};
        auto ptr = stable_partition(words.begin(), words.end(), func);
        words.erase(ptr, words.end());
        index.append(idx, words);
    }
    in.close();
}

SearchEngine::SearchEngine(const string &file_list,
                           const string &weight_file,
                           const string &stop_word,
                           const string &replace_file) :
    file_list_(file_list),
    doc_list_(weight_file),
    segmentor_(segmentor_create_segmentor("/home/cys/ltp/ltp_data/cws.model")),
    postagger_(postagger_create_postagger("/home/cys/ltp/ltp_data/pos.model")),
    filter_(stop_word),
    replacer_(replace_file)
{
    ifstream fin(file_list_.c_str());
    if (!fin.is_open()){
        fprintf(stderr, "can't open file_list!\n");
        exit(1);
    }

    string doc;
    while (getline(fin, doc)){
        size_t idx = doc_list_.append(doc);
        LoadFile(idx, doc + ".title", title_index_, filter_);
        LoadFile(idx, doc + ".body", body_index_, filter_);
    }

    title_index_.SetLength(doc_list_.doc_number());
    body_index_.SetLength(doc_list_.doc_number());

    fin.close();
    /*fprintf(stderr, "title :\n");
    title_index_.display();

    fprintf(stderr, "body :\n");
    body_index_.display();*/
    /*auto vec = body_index_.tot_number();
    sort(vec.rbegin(), vec.rend());
    FILE *fp = fopen("terms.txt", "w");
    // for (size_t i = 0; i < 100; i ++)
        // fprintf(fp, "%s\n", vec[i].second.c_str());
    for (auto ele : vec)
        fprintf(fp, "times = %lu, term = %s\n", ele.first, ele.second.c_str());
    fclose(fp);*/
}

PostingList SearchEngine::BasicAccurateSearch(const vector<Words> &query,
                                              const Index &idx)
{
    PostingList post;
    FlagStatus flag = none;
    for (auto & term : query){
        if (term.size() == 1){
            if (term[0] == "+"){
                flag = plus_flag;
                continue;
            }
            else if (term[0] == "-"){
                flag = minus_flag;
                continue;
            }
            else if (term[0] == "&"){
                flag = and_flag;
                continue;
            }
            else if (term[0] == "|"){
                flag = or_flag;
                continue;
            }
            else if (term[0] == "^"){
                flag = xor_flag;
                continue;
            }
        }
        PostingList cur;
        bool isfirst = true;
        for (auto & word : term){
            if (isfirst){
                cur = idx[word];
                isfirst = false;
            }
            else
                cur.Combine(idx[word]);
        }
        switch (flag){
            case none:
                post |= cur;
                break;
            case plus_flag:
                post &= cur;
                break;
            case minus_flag:
                post -= cur;
                break;
            case and_flag:
                post &= cur;
                break;
            case or_flag:
                post |= cur;
                break;
            case xor_flag:
                post ^= cur;
                break;
        }
    }
    return post;
}

PostingList SearchEngine::AccurateSearch(const vector<Words> & query){
    return BasicAccurateSearch(query, title_index_)
         | BasicAccurateSearch(query, body_index_);
}

void SearchEngine::BasicRankedSearch(const vector<Words> & query,
                                     const vector<double> &term_frequency,
                                     const Index &idx,
                                     vector<double> &final_scores, const double rate)
{
    vector<double> scores(final_scores.size(), 0);
    for (size_t i = 0; i < query.size(); i ++){
        const Words & term = query[i];
        PostingList cur;
        bool isfirst = true;
        for (auto & word : term){
            cerr << word << ' ';
            if (isfirst){
                cur = idx[word];
                isfirst = false;
            }
            else{
                cur.Combine(idx[word]);
            }
        }
        cerr << endl;
        cur.SetWeight(final_scores.size());
        double weight_of_query = term_frequency[i] * cur.idf();
        for (auto & doc : cur.posting()){
            scores[doc.docID()] += weight_of_query * doc.weight();
        }

        if (term.size() > 1){
            string total;
            for (auto & word : term)
                total += word;
            cur = idx[total];
            cur.display();
            cur.SetWeight(final_scores.size());
            weight_of_query = term_frequency[i] * cur.idf() * term.size();
            for (auto & doc : cur.posting()){
                scores[doc.docID()] += weight_of_query * doc.weight();
            }
        }
    }
    /*for (size_t i = 0; i < words.size(); i ++){
        const string & word = words[i];
        PostingList cur = idx[word];
        double weight_of_query = term_frequency[i] * cur.idf();
        for (auto & doc : cur.posting()){
            scores[doc.docID()] += weight_of_query * doc.weight();
        }
    }*/

    for (size_t i = 0; i < scores.size(); i ++){
        double len = idx.length(i);
        if (len != 0)
            scores[i] /= len;
    }

    for (size_t i = 0; i < final_scores.size(); i ++)
        final_scores[i] += scores[i] * rate;
}

PostingList SearchEngine::RankedSearch(const vector<Words> & query,
                                       vector<double> &scores){
    size_t doc_number = doc_list_.doc_number();
    vector<string> words;
    vector<double> term_frequency;
    for (auto & term : query){
        string total;
        for (auto & word : term)
            total += word;

        bool flag = false;
        for (size_t i = 0; i < words.size(); i ++){
            if (words[i] == total){
                flag = true;
                term_frequency[i] ++;
            }
        }
        if (flag == false){
            words.push_back(total);
            term_frequency.push_back(1);
        }
    }

    for (size_t i = 0; i < term_frequency.size(); i ++)
        term_frequency[i] = 1 + log(term_frequency[i]);

    BasicRankedSearch(query, term_frequency, title_index_, scores, 0.6);
    BasicRankedSearch(query, term_frequency, body_index_, scores, 0.4);
    
    for (size_t i = 0; i < doc_number; i ++)
        scores[i] *= doc_list_.weight(i);

    vector<int> rank(doc_number, 0);
    for (size_t i = 0; i < doc_number; i ++)
        rank[i] = i;
    auto func = [&scores](const int &a, const int &b){return scores[a] > scores[b];};
    sort(rank.begin(), rank.end(), func);

    /*for (size_t i = 0; i < doc_number; i ++)
        if (scores[rank[i]] != 0)
            cerr << "i = " << i << " id = " << rank[i] <<
            " scores = " << scores[rank[i]] << endl;*/

    PostingList post;
    for (size_t i = 0; i < doc_number; i ++){
        if (scores[rank[i]] != 0){
            post.append(rank[i]);
        }
        else
            break;
    }

    return post;
}

SearchResult SearchEngine::Search(const Query &query, vector<double> &scores,
                                  const size_t limits)
{
    return (query.accurate() ?
        SearchResult(AccurateSearch(query.query()), query, doc_list_, limits)
        :
        SearchResult(RankedSearch(query.query(), scores), query, doc_list_, limits));
}

pair<string, double> SearchEngine::Search(const vector<string> &keyword,
                                          const vector<string> &query_type)
{
    cerr << "test" << endl;
    Query query(segmentor_, filter_, replacer_, keyword);
    vector<double> scores(doc_list_.doc_number(), 0);
    SearchResult result = Search(query, scores, 20);
    sort(scores.rbegin(), scores.rend());
    AnswerExtract answer_extract(scores, result, query_type, postagger_);
    cerr << "答案为：" << answer_extract.answer() << endl;
    cerr << result << endl;
    return make_pair(answer_extract.answer(), answer_extract.score());
}

void SearchEngine::Run(){
    ResetScreen(cerr);
    Query query(segmentor_, filter_, replacer_);
    SetColor(cerr, green);
    SetColor(cerr, highlight);
    cerr << "搜索结果显示条数：";
    SetColor(cerr, white);
    SetColor(cerr, closed);
    size_t limits = 10;
    string line;
    getline(cin, line);
    stringstream in(line);
    in >> limits;
    vector<double> scores(doc_list_.doc_number(), 0);
    cerr << Search(query, scores, limits) << endl;
    // for (size_t i = 0; i < scores.size(); i ++)
        // cerr << "i = " << " scores[i] = " << scores[i] << endl;
    InitKeyboard();
    CheckKeyboard();
    CloseKeyboard();
}

SearchEngine::~SearchEngine(){
    segmentor_release_segmentor(segmentor_);
    postagger_release_postagger(postagger_);
}
