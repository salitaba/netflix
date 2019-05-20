#ifndef FILM_H
#define FILM_H

#include<string>
#include<iostream>

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
    void printInformation();
    int getPrice();
    double getRate();
    User* getAuthor();

private:
    User* aurtor;
    std::string name, summary, director;
    int id, year, length, price, rate = 0;
    bool usable = true;
};


#endif