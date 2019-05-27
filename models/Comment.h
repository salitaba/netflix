#ifndef COMMENT_H
#define COMMENT_H

#include<string>
#include<iostream>
#include<vector>

class User;

class Comment{
public:
    Comment(std::string content, int id, User* user);
    void hidden();
    void reply(std::string content);
    void show();
    bool isId(int id);
    User* getUser();
    bool isLive();
private:
    std::string content;
    int id;
    int online = true;
    User* user;
    std::vector< std::string > replies;
};




#endif