#ifndef FILM_H
#define FILM_H

#include"User.h"

class Film{
public:
    Film(std::string _name, std::string _year, std::string _length, std::string _price, 
            std::string _summary, std::string _director);
    

private:
    User* aurtor;
    std::string name, year, length, price, summary, director;

};


#endif