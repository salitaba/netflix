#ifndef FILM_H
#define FILM_H

#include<string>
#include<iostream>
#include<vector>
#include<set>

#include"Exception.h"
#include"Comment.h"

class User;

class Film{
public:
    Film(std::string _name, std::string _year, std::string _length, std::string _price, 
            std::string _summary, std::string _director, int id, User* _aurtor);

    void setDetail(std::string _name, std::string _year, std::string _length, std::string _price, 
            std::string _summary, std::string _director);
    bool isId(int id);
    bool isUser(User* user);
    void unusable();
    bool find(std::string name,std::string minYear,std::string minRate,std::string price,std::string maxYear,std::string director);
    void printDetail(int id);
    void printInformation(std::vector< Film* > topFilms);
    int getPrice();
    double getRate();
    User* getAuthor();
    void rateTheRate(int rateNum, User* user);
    void addComment(std::string content, User* user);
    void reply(int commentId, std::string content);
    void deleteComment(int commentId);
    int getId();
    void printShortDetail();
    Comment* getComment(int id);
    std::string getName();
private:
    User* aurtor;
    std::string name, summary, director;
    int id, year, length, price;
    double rate = 0;
    bool usable = true;
    std::set< User* > userRated;
    std::vector< Comment* > comments;
};


#endif