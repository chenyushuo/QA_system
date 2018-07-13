#ifndef MY_DEFINE
#define MY_DEFINE

#include <vector>
#include <string>

typedef std::vector<std::string> Sentence;
typedef std::vector<Sentence> Paragraph;
typedef std::vector<Paragraph> Passage;

typedef std::vector<int> SentenceFlag;
typedef std::vector<SentenceFlag> ParagraphFlag;
typedef std::vector<ParagraphFlag> PassageFlag;

extern char file_list_path[];
extern char weight_file_path[];
extern char stop_word_path[];
extern char replace_file_path[];
extern char restorer_file_path[];
extern char querytype_path[];
extern char data_file_path[];
extern char speckey_path[];
extern char qa_path[];
extern char cws_path[];
extern char pos_path[];

#endif
