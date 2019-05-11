#ifndef USER_H
#define USER_H

#include"Film.h"
#include<string>
#include<vector>

class Film;

class User{
public:
    User (std::string email, std::string user, std::string password, int age, int _id);
    void setPublisher();
    int hashCode(std::string const& str);
    bool isUserName(std::string _userName);
private:
    std::string email, userName, password;
    int age;
    int id;
    bool publisher = false;
    std::vector<Film*>films;

};


#endif