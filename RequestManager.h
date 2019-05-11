#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include"Film.h"
#include"User.h"
#include"Exception.h"

#include<string>
#include<vector>
#include<iostream>

class RequestManager{
public:
    void handle(std::string input);
    void split(std::string, std::vector<std::string>& elements);
    void handleEvents(std::vector<std::string>& elements);
    void signup(std::vector<std::string>& elements);
    bool findUserName(std::string userName);
    int getUserId();
    void login(std::vector<std::string> element);
private:
    User *userLoggined = NULL;
    std::vector<Film*> films;
    std::vector<User*> users;
    int userIdCounter;
};


#endif