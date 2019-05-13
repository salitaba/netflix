#ifndef EXCEPTION_H
#define EXCEPTION_H

class BadRequest: public std::exception{
public:
    const char* what() const throw(){
        return "Bad request!";
    }

};


#endif