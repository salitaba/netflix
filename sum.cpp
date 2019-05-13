#include<string>
#include<iostream>

#include"RequestManager.h"

using namespace std;


int main(){
    RequestManager requestManager;
    requestManager.handle(" POST signup ? email ali username   ali password 1234 age 16 publisher false");
    requestManager.handle("POST login ? username ali password 12345");
}
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
#include"RequestManager.h"

#define PUT "PUT"
#define POST "POST"
#define DELETE "DELETE"
#define GET "GET"

using namespace std;

void RequestManager::handle(string input){
    try{
        Request query(input);
        this->handleEvents(query);
    }catch(exception &e){
        cout<<e.what()<<endl;
    }
}

void RequestManager::split(string input, vector<string>& inputElement){
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
}

void RequestManager::handleEvents(Request request){
    if (request.getMethod() == POST && request.getQuery() == "signup"){
        return this->signup(request);
    } 
    // if(element[0] == POST && element[1] == "signup")
    //     return this->signup(element);
    // if(element[0] == POST && element[1] == "login")
    //     return this->login(element);
    // throw BadRequest();

}

void RequestManager::signup(Request request){
    vector<string> requirementField{"email", "username", "password", "age"};
    vector<string> optionalField{"publisher"};
    request.check(requirementField);
    

    string username = request.get("username", false);
    string email = request.get("email", false);
    string password = request.get("password", false);
    string age = request.get("age", false);
    string publisher = request.get("publisher", true, "false");
    

    if(this->findUserName(username) == true)
        throw BadRequest();
    User* newUser = new User(email, username, password, atoi(age.c_str()), this->getUserId(), publisher);
    // userLoggined = newUser;
    // users.push_back(newUser);
}


bool RequestManager::findUserName(string userName){
    for( auto user : users)
        if(user->isUserName(userName) == true)
            return true;
    return false;
}

void RequestManager::login(vector<string> element){
    for( auto user : users)
        if(user->isUserName(element[3]) == true && user->isPassword(element[4]) == true){
            userLoggined = user;
            return;
        }
    throw BadRequest();
}

int RequestManager::getUserId(){
    userIdCounter++;
    return userIdCounter - 1;
}
#include"User.h"

const long long mod = 1e9 + 7;

using namespace std;

int User::hashCode(string const &str) { 
    long long hash = 0;
    for (auto Letter : str){
        hash = ( hash * 313 + Letter ) % mod;
    }
    return hash;
}

User::User(string _email, string _userName, string _password, int _age, int _id, string _publisher){
    email = _email;
    userName = _userName;
    password = this->hashCode(_password);
    age = _age;
    id = _id;
    if(_publisher == "true")
        publisher = true;
    else if(_publisher != "false")
        throw BadRequest();
}

void User::setPublisher(){
    publisher = true;
}

bool User::isUserName(string _userName){
    return userName == _userName;
}

bool User::isPassword(string _password){
    return password == this->hashCode(_password);
}
#ifndef EXCEPTION_H
#define EXCEPTION_H

class BadRequest: public std::exception{
public:
    const char* what() const throw(){
        return "Bad request!";
    }

};


#endif
#ifndef FILM_H
#define FILM_H

#include"User.h"

class Film{
public:


private:
    User* aurtor;


};


#endif
#ifndef REQUEST_H
#define REQUEST_H

#include<string>
#include<queue>
#include<map>
#include<iostream>

#include"Exception.h"

class Request{
public:
    Request(std::string);
    std::queue<std::string> split(std::string input);
    void convertToMap(std::queue<std::string> keyValue);
    std::string getMethod();
    std::string getQuery();
    void check(std::vector<std::string> requirementField);
    std::string get(std::string key, bool optional);
    std::string get(std::string key, bool optional, std::string defaultValue);
private:
    std::string method, query;
    std::map<std::string, std::string>key;
};



#endif
#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include"Film.h"
#include"User.h"
#include"Request.h"

#include<string>
#include<vector>
#include<iostream>

class RequestManager{
public:
    void handle(std::string input);
    void split(std::string, std::vector<std::string>& elements);
    void handleEvents(Request);
    void signup(Request elements);
    bool findUserName(std::string userName);
    int getUserId();
    void login(std::vector<std::string> element);
private:
    User *userLoggined = NULL;
    std::vector<Film*> films;
    std::vector<User*> users;
    int userIdCounter;
};


#endif
#ifndef USER_H
#define USER_H

#include<string>
#include<vector>

#include"Exception.h"

class Film;

class User{
public:
    User (std::string email, std::string user, std::string password, int age, int _id, std::string publisher);
    void setPublisher();
    int hashCode(std::string const& str);
    bool isUserName(std::string _userName);
    bool isPassword(std::string _password);
private:
    std::string email, userName;
    int password;
    int age;
    int id;
    bool publisher = false;
    std::vector<Film*>films;

};


#endif