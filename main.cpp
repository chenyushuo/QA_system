#include "search_engine.h"

#include <bits/stdc++.h>

using namespace std;

int main(){
    SearchEngine search_engine("file_list.txt");
    for (int i = 0; i < 600; i ++){
        cerr << "input:" << endl;
        string line;
        // line = "计算机";
        // line = "副 院长";
        // line = "数学系主任";
        // line = "图灵班 班主任";
        // line = "校长";
        // line = "院长";
        getline(cin, line);
        stringstream keyword_in(line);
        istream_iterator<string> keyword_iter(keyword_in), eof;
        vector<string> keyword(keyword_iter, eof);
        cerr << line << endl;
        // line = "nh";
        getline(cin, line);
        stringstream query_type_in(line);
        istream_iterator<string> query_type_iter(query_type_in);
        vector<string> query_type(query_type_iter, eof);
        search_engine.Search(keyword, query_type);
        cerr << "end" << endl;
        cerr << "\033[2J" << endl;
        //system("clear");
    }
    /*while (true)
        search_engine.Run();*/
    return 0;
}
