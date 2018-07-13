#include "ltp.h"

#include <vector>
#include <string>
#include <algorithm>

#include "filter.h"
#include "replacer.h"
#include "restorer.h"
#include "segment_dll.h"
#include "postag_dll.h"
#include "my_define.h"

using namespace std;

LTP::LTP(const string &stop_words_file, const string &replace_file,
         const string &restorer_file) :
    segmentor_(segmentor_create_segmentor(cws_path)),
    postagger_(postagger_create_postagger(pos_path)),
    filter_(stop_words_file),
    replacer_(replace_file),
    restorer_(restorer_file)
{

}

void LTP::Replace(Sentence &words) const{
    replacer_.Process(words);
}

void LTP::Filter(Sentence &words) const{
    auto func = [this](const string &str){return this -> filter_.IsVaild(str);};
    auto ptr = stable_partition(words.begin(), words.end(), func);
    words.erase(ptr, words.end());
}

void LTP::Restore(const Sentence &words, Sentence &tags) const{
    restorer_.Process(words, tags);
}

void LTP::Segment(const std::string &term, Sentence &words, MODE mode) const{
    segmentor_segment(segmentor_, term, words);
    if (mode == MODIFY){
        Replace(words);
        Filter(words);
    }
}

void LTP::Postag(const Sentence &words, Sentence &tags, MODE mode) const{
    postagger_postag(postagger_, words, tags);
    if (mode == MODIFY){
        Restore(words, tags);
    }
}

LTP::~LTP(){
    segmentor_release_segmentor(segmentor_);
    postagger_release_postagger(postagger_);
}
