#ifndef EXCEPTION_H
#define EXCEPTION_H

class BadRequest: public std::exception{
public:
    const char* what() const throw(){
        return "Bad Request";
    }
};

class PermissionDenied: public std::exception{
public:
    const char* what() const throw(){
        return "Permission Denied";
    }
};

class NotFound: public std::exception {
public: 
    const char* what() const throw(){
        return "Not Found";
    }
};

#endif