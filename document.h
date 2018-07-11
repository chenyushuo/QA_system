#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <cstddef>

#include <vector>
#include <string>
#include <unordered_map>

class Document{
public:
    static const size_t npos = -1;

private:
    size_t doc_number_;
    std::vector<double> weight_;
    std::vector<std::string> doc_name_;
    std::unordered_map<std::string, size_t> doc_map_;

public:
    Document(const std::string &weight_file = "time.txt");

    size_t doc_number() const{return doc_number_;}
    double weight(size_t idx) const{return weight_[idx];}
    size_t find(const std::string &doc) const;//返回doc对应的id，没有就返回npos
    size_t append(const std::string &doc);

    std::string operator [] (size_t idx) const;

    ~Document() = default;
};

#endif