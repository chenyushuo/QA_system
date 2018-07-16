#include "document.h"

#include <cstdlib>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

Document::Document(const string &file_list, const string &weight_file) :
    doc_number_(0)
{
    ifstream fin(file_list);
    if (!fin.is_open()){
        cerr << "can't open file_list!" << endl;
        exit(1);
    }
    string line;
    while (getline(fin, line)){
        doc_name_.push_back(line);
        doc_map_.insert(make_pair(line, doc_number_));
        doc_number_ ++;
    }
    fin.close();

    fin.open(weight_file.c_str());
    if (!fin.is_open()){
        cerr << "can't open weight file!" << endl;
        exit(1);
    }
    double w;
    while (fin >> w)
        weight_.push_back(w);
    fin.close();
}

size_t Document::find(const string &doc) const{
    auto iter = doc_map_.find(doc);
    return (iter == doc_map_.end() ? npos : iter -> second);
}

size_t Document::append(const string &doc){
    auto iter = doc_map_.find(doc);
    if (iter == doc_map_.end()){
        doc_map_.insert(make_pair(doc, doc_number_));
        doc_number_ ++;
        doc_name_.push_back(doc);
        return doc_number_ - 1;
    }
    exit(1);
    return iter -> second;
}

string Document::operator [] (size_t idx) const{
    if (idx < doc_number_)
        return local_path + doc_name_[idx].substr(2);
    return string();
}

void Document::Load(ifstream &in){
    in >> doc_number_;
    doc_name_.resize(doc_number_);
    for (size_t i = 0; i < doc_number_; i ++){
        in >> doc_name_[i];
        doc_map_.insert(make_pair(doc_name_[i], i));
    }
}

void Document::Save(ofstream &out){
    out << doc_number_ << endl;
    for (size_t i = 0; i < doc_number_; i ++)
        out << doc_name_[i] << endl;
}
