#include"Request.h"

using namespace std;

Request::Request(string request){
    queue<string> splitedRequest = this->split(request);
    cout<<splitedRequest.size()<<endl;
    if(splitedRequest.size() < 3)
        throw BadRequest();
    method = splitedRequest.front();
    splitedRequest.pop();
    query = splitedRequest.front();
    splitedRequest.pop();
    if(splitedRequest.front() != "?")
        throw BadRequest();
    splitedRequest.pop();
    this->convertToMap(splitedRequest);
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
        if(now.size())
            inputElement.push(now);
        index++;
    }
    return inputElement;
}

bool Request::isQuery(string _method, string _query){
    return method == method && query == _query;
}

string Request::getMethod(){
    return method;
}

void Request::convertToMap(queue<string> keyValue){
    if(keyValue.size() % 2 == 1)
        throw BadRequest();
    while(keyValue.size() > 0){
        string keyy = keyValue.front();
        keyValue.pop();
        key[keyy] = keyValue.front();
        keyValue.pop();
    }              
}