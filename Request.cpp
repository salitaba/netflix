#include"Request.h"

using namespace std;

Request::Request(string request){
    vector<string> splitedRequest = this->split(request);
    if(splitedRequest.size() < 3)
        throw BadRequest();
}

vector<string> Request::split(string input){
    vector<string>inputElement;
    int index = 0;
    while(index < input.size()){
        string now = "";
        while(index < input.size() && input[index] != ' '){
            now += input[index];
            index++;
        }
        inputElement.push_back(now);
        index++;
    }
    return inputElement;
}