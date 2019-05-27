#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H


#include<string>
#include<vector>
#include<iostream>

#include"Film.h"
#include"User.h"
#include"Request.h"
#include"GraphRep.h"

class RequestManager{
public:
    RequestManager();
    void handle(std::string input);
    void split(std::string, std::vector<std::string>& elements);
    void handleEvents(ali::Request);
    void signup(ali::Request request);
    User* findUserName(std::string userName);
    int getUserId();
    void login(ali::Request request);
    void postFilm(ali::Request request);
    int getFilmId();
    void editFilm(ali::Request request);
    Film* getFilm(int id);
    void deleteFilm(ali::Request request);
    void showFollowers(ali::Request request);
    void post(ali::Request request);
    void deleteMethod(ali::Request request);
    void put(ali::Request request);
    void getMethod(ali::Request request);
    void published(ali::Request request);
    void searchFilm(ali::Request request);
    void seprateSearchFromShowDetailFilm(ali::Request request);
    void buyFilm(ali::Request request);
    void rateFilm(ali::Request request);
    void commentFilm(ali::Request reqeust);
    void increaseMoney(ali::Request request);
    void moneyHandler(ali::Request request);
    void reply(ali::Request request);
    void deleteComment(ali::Request request);
    std::vector< Film* > topFilms(Film* film);
    void follow(ali::Request request);
    User* getUser(int id);
    void showNotification();
    void showLimitedNotification(ali::Request request);
    void purchased(ali::Request request);
    void logout();
    void getMoney();
    void addAdjence(Film* film);
private:
    User *userLoggined = NULL;
    std::vector<Film*> films;
    std::vector<User*> users;
    int userIdCounter = 0, filmIdCounter = 0;
    GraphRep graph;
};


#endif