#ifndef COMMENT_H
#define COMMENT_H

#include<string>

class Film;

class Comment{
public:
    Comment(std::string content, int id);
    void hidden();
private:
    std::string content;
    int id;
    int online = true;
};




#endif