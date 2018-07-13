#include "index.h"

#include <cmath>
#include <cassert>

#include <vector>
#include <string>

using namespace std;

inline static double sqr(const double &x){return x * x;}

const DocPosition DocPosition::operator + (const DocPosition &pos) const{
    if (docID_ != pos.docID_)
        return DocPosition();
    DocPosition result(docID_);
    auto p1 = position_.begin();
    auto p2 = pos.position_.begin();
    while (p1 != position_.end() && p2 != pos.position_.end()){
        if (p1 -> begin < p2 -> begin){
            if (p1 -> end == p2 -> begin)
                result.append(*p1 + *p2);
            p1 ++;
        }
        else{
            if (p2 -> end == p1 -> begin)
                result.append(*p2 + *p1);
            p2 ++;
        }
    }
    return result;
}

void PostingList::append(size_t docID, size_t pos){
    if (docID == Document::npos)
        return;
    if (posting_.empty() || docID != posting_.rbegin() -> docID())
        posting_.push_back(DocPosition(docID));
    auto ptr = posting_.rbegin();
    ptr -> append(pos);
}

const PostingList PostingList::operator & (const PostingList &post) const{
    PostingList result;
    auto p1 = posting_.begin();
    auto p2 = post.posting_.begin();
    while (p1 != posting_.end() && p2 != post.posting_.end()){
        if (p1 -> docID() < p2 -> docID()){
            p1 ++;
        }
        else if (p1 -> docID() > p2 -> docID()){
            p2 ++;
        }
        else{
            result.append(p1 -> docID());
            p1 ++, p2 ++;
        }
    }
    return result;
}

const PostingList PostingList::operator | (const PostingList &post) const{
    PostingList result;
    auto p1 = posting_.begin();
    auto p2 = post.posting_.begin();
    while (p1 != posting_.end() && p2 != post.posting_.end()){
        if (p1 -> docID() < p2 -> docID()){
            result.append(p1 -> docID());
            p1 ++;
        }
        else if (p1 -> docID() > p2 -> docID()){
            result.append(p2 -> docID());
            p2 ++;
        }
        else{
            result.append(p1 -> docID());
            p1 ++, p2 ++;
        }
    }
    while (p1 != posting_.end()){
        result.append(p1 -> docID());
        p1 ++;
    }
    while (p2 != post.posting_.end()){
        result.append(p2 -> docID());
        p2 ++;
    }
    return result;
}

const PostingList PostingList::operator ^ (const PostingList &post) const{
    PostingList result;
    auto p1 = posting_.begin();
    auto p2 = post.posting_.begin();
    while (p1 != posting_.end() && p2 != post.posting_.end()){
        if (p1 -> docID() < p2 -> docID()){
            result.append(p1 -> docID());
            p1 ++;
        }
        else if (p1 -> docID() > p2 -> docID()){
            result.append(p2 -> docID());
            p2 ++;
        }
        else{
            p1 ++, p2 ++;
        }
    }
    while (p1 != posting_.end()){
        result.append(p1 -> docID());
        p1 ++;
    }
    while (p2 != post.posting_.end()){
        result.append(p2 -> docID());
        p2 ++;
    }
    return result;
}

const PostingList PostingList::operator - (const PostingList &post) const{
    PostingList result;
    auto p1 = posting_.begin();
    auto p2 = post.posting_.begin();
    while (p1 != posting_.end() && p2 != post.posting_.end()){
        if (p1 -> docID() < p2 -> docID()){
            result.append(p1 -> docID());
            p1 ++;
        }
        else if (p1 -> docID() > p2 -> docID()){
            p2 ++;
        }
        else{
            p1 ++, p2 ++;
        }
    }
    while (p1 != posting_.end()){
        result.append(p1 -> docID());
        p1 ++;
    }
    return result;
}

void PostingList::Combine(const PostingList &post){
    if (empty())
        return;
    PostingList result;
    auto p1 = posting_.begin();
    auto p2 = post.posting_.begin();
    while (p1 != posting_.end() && p2 != post.posting_.end()){
        if (p1 -> docID() < p2 -> docID()){
            p1 ++;
        }
        else if (p1 -> docID() > p2 -> docID()){
            p2 ++;
        }
        else{
            result.append(*p1 + *p2);
            p1 ++, p2 ++;
        }
    }
    *this = result;
}

void Index::append(const size_t docID, const vector<string> &content){
    for (size_t i = 0; i < content.size(); i ++){
        auto & term = content[i];
        auto iter = term_map_.find(term);
        size_t idx;
        if (iter == term_map_.end()){
            term_map_.insert(make_pair(term, term_number_));
            idx = term_number_;
            term_number_ ++;
            term_.push_back(term);
            dictionary_.push_back(PostingList());
        }
        else{
            idx = iter -> second;
        }
        dictionary_[idx].append(docID, i);
    }
}

const PostingList Index::operator [] (const string &term) const{
    auto iter = term_map_.find(term);
    if (iter == term_map_.end())
        return PostingList();
    return dictionary_[iter -> second];
}

void Index::SetLength(const size_t doc_number){
    length_.resize(doc_number, 0);
    for (auto & post : dictionary_){
        post.SetWeight(doc_number);
        for (auto & doc : post.posting())
            length_[doc.docID()] += sqr(doc.weight());
    }
    for (size_t i = 0; i < doc_number; i ++)
        length_[i] = sqrt(length_[i]);
}

void DocPosition::Load(istream & in){
    in >> docID_;
    size_t number;
    in >> number;
    position_.resize(number);
    for (size_t i = 0; i < number; i ++)
        position_[i].Load(in);
}

void DocPosition::Save(ostream & out){
    out << docID_ << endl;
    out << position_.size() << endl;
    for (size_t i = 0; i < position_.size(); i ++)
        position_[i].Save(out);
}

void PostingList::Load(ifstream &in){
    size_t number;
    in >> number;
    posting_.resize(number);
    for (size_t i = 0; i < number; i ++)
        posting_[i].Load(in);
}

void PostingList::Save(ofstream &out){
    out << posting_.size() << endl;
    for (size_t i = 0; i < posting_.size(); i ++)
        posting_[i].Save(out);
}

void Index::Load(ifstream &in){
    in >> term_number_;
    string word;
    for (size_t i = 0; i < term_number_; i ++){
        in >> word;
        term_.push_back(word);
        term_map_.insert(make_pair(word, i));
    }
    dictionary_.resize(term_number_);
    for (size_t i = 0; i < term_number_; i ++)
        dictionary_[i].Load(in);
    size_t doc_number;
    in >> doc_number;
    length_.resize(doc_number);
    for (size_t i = 0; i < doc_number; i ++)
        in >> length_[i];
    for (size_t i = 0; i < term_number_; i ++)
        dictionary_[i].SetWeight(doc_number);
}

void Index::Save(ofstream &out){
    out << term_number_ << endl;
    for (size_t i = 0; i < term_number_; i ++)
        out << term_[i] << endl;
    for (size_t i = 0; i < term_number_; i ++)
        dictionary_[i].Save(out);
    out << length_.size() << endl;
    for (size_t i = 0; i < length_.size(); i ++)
        out << length_[i] << endl;
}
