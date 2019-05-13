#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include"Film.h"
#include"User.h"
#include"Request.h"

#include<string>
#include<vector>
#include<iostream>

class RequestManager{
public:
    void handle(std::string input);
    void split(std::string, std::vector<std::string>& elements);
    void handleEvents(Request);
    void signup(Request request);
    bool findUserName(std::string userName);
    int getUserId();
    void login(Request request);
    void postFilm(Request request);
private:
    User *userLoggined = NULL;
    std::vector<Film*> films;
    std::vector<User*> users;
    int userIdCounter;
};


#endif