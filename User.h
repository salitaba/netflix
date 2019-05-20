#ifndef USER_H
#define USER_H

#include<string>
#include<vector>
#include<iostream>
#include<set>

#include"Exception.h"
#include"Film.h"

class User{
public:
    User (std::string email, std::string user, std::string password, int age, int _id, std::string publisher);
    void setPublisher();
    int hashCode(std::string const& str);
    bool isUserName(std::string _userName);
    bool isPassword(std::string _password);
    bool isPublisher();
    void showFollower();
    void find(std::string name,std::string minYear,std::string minRate,std::string price,std::string maxYear,std::string director);
    void buy(Film* film);
    void sold(User* user, Film* film);
    bool checkBuyFilm(Film* film);
    void increaseMoney(int value);
private:
    std::string email, userName;
    int password;
    int age;
    int id;
    int money = 0 ;
    bool publisher = false;
    std::vector< Film* >films;
    std::set< User* >followers;
    std::set< std::pair<User*, int> >solds;
    std::set< Film* > buyedFilm;
};


#endif