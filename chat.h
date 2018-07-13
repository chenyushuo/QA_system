#ifndef CHAT_H
#define CHAT_H

#include <string>

class Chat{
private:
    static const std::string appkey_;
    static const std::string api_;
    static const std::string timestamp_pre_;
    static const std::string sign_pre_;
    static const std::string app_secret_;
    static const std::string rq_left_;
    static const std::string rq_right_;
    static const std::string url_pre_;

    std::string timestamp_;
    std::string sign_;
    std::string rq_;
    std::string url_;
    std::string answer_;
    
    static void FixAnswer(std::string &answer);

public:
    Chat() = default;
    
    std::string Answer(const std::string &query);
    
    ~Chat() = default;
};

#endif
