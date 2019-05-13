#include"Request.h"

using namespace std;

Request::Request(string request){
    queue<string> splitedRequest = this->split(request);
    if(splitedRequest.size() < 3)
        throw BadRequest();
    method = splitedRequest.front();
    splitedRequest.pop();
    query = splitedRequest.front();
    splitedRequest.pop();
}

queue<string> Request::split(string input){
    queue<string>inputElement;
    int index = 0;
    while(index < input.size()){
        string now = "";
        while(index < input.size() && input[index] != ' '){
            now += input[index];
            index++;
        }
        inputElement.push(now);
        index++;
    }
    return inputElement;
}

bool Request::isQuery(string _method, string _query){
    return method == method && query == _query;
}