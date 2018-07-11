#ifndef INDEX_H
#define INDEX_H

#include <cstdio>
#include <cmath>
#include <cstddef>

#include <string>
#include <vector>
#include <unordered_map>

#include "document.h"

struct Segment{
    static const size_t npos = -1;
    size_t begin, end;

    Segment() : begin(npos), end(npos){}
    Segment(size_t pos) : begin(pos), end(pos + 1){}
    Segment(size_t l, size_t r) : begin(l), end(r){}

    const Segment operator + (const Segment &seg) const{
        return (end == seg.begin ? Segment(begin, seg.end) : Segment());
    }

    bool operator == (const Segment &seg) const{
        return begin == seg.begin && end == seg.end;
    }
    bool operator != (const Segment &seg) const{
        return begin != seg.begin || end != seg.end;
    }

    ~Segment() = default;

    void display(FILE *fp = stderr){
        fprintf(fp, "segment = [%lu, %lu)\n", begin, end);
    }
};

class DocPosition{
private:
    size_t docID_;
    std::vector<Segment> position_;
    double weight_;

public:
    DocPosition(size_t docID = Document::npos) : docID_(docID){}

    size_t docID() const{return docID_;}
    bool empty() const{return position_.empty();}
    double weight() const{return weight_;}
    void SetWegith(double idf){
        if (position_.size() == 0)
            weight_ = 0;
        else
            weight_ = (1 + log(position_.size())) * idf;
    }

    void append(const Segment &seg){
        if (seg != Segment())
            position_.push_back(seg);
    }

    const DocPosition operator + (const DocPosition &pos) const;

    ~DocPosition() = default;

    void display(FILE *fp = stderr){
        fprintf(fp, "docID = %lu\n", docID_);
        for (auto & ele : position_)
            ele.display(fp);
        fprintf(fp, "\n");
    }
    size_t tot_number(){return position_.size();}
};

class PostingList{
private:
    std::vector<DocPosition> posting_;
    double idf_;

public:
    PostingList() : idf_(0){}

    const std::vector<DocPosition> & posting() const{return posting_;}
    bool empty() const{return posting_.empty();}
    double idf(){return idf_;}

    void append(const DocPosition &pos){if (!pos.empty()) posting_.push_back(pos);}
    void append(size_t docID = Document::npos, size_t pos = Segment::npos);
    void SetWeight(const size_t doc_number){
        idf_ = log(1.0 * doc_number / posting_.size());
        for (auto & doc : posting_)
            doc.SetWegith(idf_);
    }

    const PostingList operator & (const PostingList &post) const;
    const PostingList operator | (const PostingList &post) const;
    const PostingList operator ^ (const PostingList &post) const;
    const PostingList operator - (const PostingList &post) const;

    PostingList & operator &= (const PostingList &post){
        *this = *this & post;
        return *this;
    }
    PostingList & operator |= (const PostingList &post){
        *this = *this | post;
        return *this;
    }
    PostingList & operator ^= (const PostingList &post){
        *this = *this ^ post;
        return *this;
    }
    PostingList & operator -= (const PostingList &post){
        *this = *this - post;
        return *this;
    }

    void Combine(const PostingList &post);

    ~PostingList() = default;

    void display(FILE *fp = stderr){
        for (auto & ele : posting_)
            ele.display(fp);
        fprintf(fp, "--------------------\n");
    }
    size_t tot_number(){
        size_t res = 0;
        for (auto & ele : posting_)
            res += ele.tot_number();
        return res;
    }
};

class Index{
private:
    size_t term_number_;
    std::vector<std::string> term_;
    std::unordered_map<std::string, size_t> term_map_;

    std::vector<PostingList> dictionary_;
    std::vector<double> length_;

public:
    Index() : term_number_(0){};

    void append(const size_t docID, const std::vector<std::string> &content);
    const PostingList operator [] (const std::string &term) const;

    void SetLength(const size_t doc_number);
    double length(const size_t idx) const{return length_[idx];}

    ~Index() = default;

    void display(FILE *fp = stderr){
        fprintf(stderr, "term_number = %lu\n", term_number_);
        for (size_t i = 0; i < term_number_; i ++){
            auto & term = term_[i];
            auto & dict = dictionary_[i];
            fprintf(fp, "term = %s tot number = %lu\n",
                    term.c_str(), dict.tot_number());
            dict.display(fp);
        }
        fprintf(stderr, "============================== end of Index\n");
    }
    std::vector<std::pair<size_t, std::string> > tot_number(){
        std::vector<std::pair<size_t, std::string> > vec;
        for (size_t i = 0; i < term_number_; i ++){
            auto & term = term_[i];
            auto & dict = dictionary_[i];
            vec.push_back(make_pair(dict.tot_number(), term));
        }
        return vec;
    }
};

#endif