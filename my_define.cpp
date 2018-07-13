#include "my_define.h"

#ifdef CYS_COMPUTER
    #define local_path "/home/cys/QA/code/"
    #define ltp_model_path "/home/cys/ltp/ltp_data/"
#else
    #define local_path "/root/group2/"
    #define ltp_model_path "/root/group1/ltp-3.4.0/bin/ltp_data/"
#endif

char file_list_path[] = local_path "file_list.txt";
char weight_file_path[] = local_path "time.txt";
char stop_word_path[] = local_path "stop_words.txt";
char replace_file_path[] = local_path "replace.txt";
char restorer_file_path[] = local_path "tag_restorer.txt";
char querytype_path[] = local_path "querytype.dat";
char data_file_path[] = local_path "data.txt";
char speckey_path[] = local_path "speckey.dat";
char qa_path[] = local_path "qa.dat";
char cws_path[] = ltp_model_path "cws.model";
char pos_path[] = ltp_model_path "pos.model";
