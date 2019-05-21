#ifndef COMMENT_H
#define COMMENT_H

#include<string>
#include<vector>

class User;

class Comment{
public:
    Comment(std::string content, int id, User* user);
    void hidden();
    void reply(std::string content);
private:
    std::string content;
    int id;
    int online = true;
    User* user;
    std::vector< std::string > replies;
};




#endif