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
    int getFilmId();
    void editFilm(Request request);
    Film* getFilm(int id);
    void deleteFilm(Request request);
    void showFollowers(Request request);
    void post(Request request);
    void deleteMethod(Request request);
    void put(Request request);
    void getMethod(Request request);
    void published(Request request);
    void searchFilm(Request request);
    void seprateSearchFromShowDetailFilm(Request request);
    void buyFilm(Request request);
    void rateFilm(Request request);
    void commentFilm(Request reqeust);
    void increaseMoney(Request request);
    void moneyHandler(Request request);
    void reply(Request request);
    void deleteComment(Request request);
    std::vector< Film* > topFilms();
    void follow(Request request);
    User* getUser(int id);
private:
    User *userLoggined = NULL;
    std::vector<Film*> films;
    std::vector<User*> users;
    int userIdCounter = 0, filmIdCounter = 0;
};


#endif