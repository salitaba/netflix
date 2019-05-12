#ifndef REQUEST_H
#define REQUEST_H

#include<string>
#include<vector>

#include"Exception.h"

class Request{
public:
    Request(std::string);
    std::vector<std::string> split(std::string input);
private:


};



#endif