#ifndef REQUEST_H
#define REQUEST_H

#include<string>
#include<vector>

class Request{
public:
    Request(std::string);
    void split(std::string input, std::vector<std::string>& inputElement);
private:


};



#endif