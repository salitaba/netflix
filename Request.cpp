#include"Request.h"

using namespace std;

Request::Request(string request){
    queue<string> splitedRequest = this->split(request);
    method = splitedRequest.front();
    splitedRequest.pop();
    while(splitedRequest.size() != 0 && splitedRequest.front() != "?"){
        query += splitedRequest.front();
        splitedRequest.pop();
        if(splitedRequest.size() != 0 && splitedRequest.front() != "?")
            query+=" ";
    }
    if(splitedRequest.size() == 0) 
        return ;
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

string Request::getMethod(){
    return method;
}

string Request::getQuery(){
    return query;
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

void Request::check(vector<string> requirementField){
    for(auto field : requirementField)
        if(key[field].size() == 0){
            throw BadRequest();
        }
            
}

string Request::get(string _key, bool optional){
    if(optional == false && key[_key].size() == 0)
        throw BadRequest();
    return key[_key];
}

string Request::get(string _key, bool optional, string defaultValue){
    if(optional == false && key[_key].size() == 0)
        throw BadRequest();
    if(key[_key].size() == 0)
        return defaultValue;
    return key[_key];
}