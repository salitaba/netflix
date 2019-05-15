#ifndef USER_H
#define USER_H

#include<string>
#include<vector>
#include<iostream>
#include<set>

#include"Exception.h"

class Film;

class User{
public:
    User (std::string email, std::string user, std::string password, int age, int _id, std::string publisher);
    void setPublisher();
    int hashCode(std::string const& str);
    bool isUserName(std::string _userName);
    bool isPassword(std::string _password);
    bool isPublisher();
    void showFollower();
private:
    std::string email, userName;
    int password;
    int age;
    int id;
    bool publisher = false;
    std::vector< Film* >films;
    std::set< User* >followers;

};


#endif