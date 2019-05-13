#include"RequestManager.h"

#define PUT "PUT"
#define POST "POST"
#define DELETE "DELETE"
#define GET "GET"

#define MINIMUM_NUMBER_OF_SIGNUP_FIELD 7
#define MAXIMUM_NUMBER_OF_SIGNUP_FIELD 8

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
    
    
    // if(element.size() < MINIMUM_NUMBER_OF_SIGNUP_FIELD || element.size() > MAXIMUM_NUMBER_OF_SIGNUP_FIELD)
    //     throw BadRequest();  
    // if(this->findUserName(element[4]) == true)
    //     throw BadRequest();
    // User* newUser = new User(element[3], element[4], element[5], atoi(element[6].c_str()), this->getUserId());
    // if(element.size() == MAXIMUM_NUMBER_OF_SIGNUP_FIELD){
    //     if(element[MAXIMUM_NUMBER_OF_SIGNUP_FIELD - 1] == "true")
    //         newUser->setPublisher();
    //     else if(element[MAXIMUM_NUMBER_OF_SIGNUP_FIELD  - 1] != "false")
    //         throw BadRequest();
    // }
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