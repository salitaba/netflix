#ifndef REQUEST_H
#define REQUEST_H

#include<string>
#include<queue>
#include<map>

#include"Exception.h"

class Request{
public:
    Request(std::string);
    std::queue<std::string> split(std::string input);
    bool isQuery(std::string method,std::string query);
    void convertToMap(std::queue<std::string> keyValue);
private:
    std::string method, query;
    std::map<std::string, std::string>key;
};



#endif