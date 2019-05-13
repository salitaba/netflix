#include"RequestManager.h"

#define PUT "PUT"
#define POST "POST"
#define DELETE "DELETE"
#define GET "GET"
#define USERNAME "username"
#define PASSWORD "password"
#define EMAIL "email"
#define AGE "age"
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
    if(request.getMethod() == POST && request.getQuery() == "login")
         return this->login(request);
    // throw BadRequest();

}

void RequestManager::signup(Request request){
    vector<string> requirementField{EMAIL, USERNAME, PASSWORD, AGE};
    vector<string> optionalField{"publisher"};
    request.check(requirementField);
    

    string username = request.get(USERNAME, false);
    string email = request.get(EMAIL, false);
    string password = request.get(PASSWORD, false);
    string age = request.get(AGE, false);
    string publisher = request.get("publisher", true, "false");
    

    if(this->findUserName(username) == true)
        throw BadRequest();
    User* newUser = new User(email, username, password, atoi(age.c_str()), this->getUserId(), publisher);
    userLoggined = newUser;
    users.push_back(newUser);
}


bool RequestManager::findUserName(string userName){
    for( auto user : users)
        if(user->isUserName(userName) == true)
            return true;
    return false;
}

void RequestManager::login(Request request){
    vector<string> requirementField{USERNAME, PASSWORD};
    request.check(requirementField);
    for( auto user : users)
        if(user->isUserName(request.get(USERNAME, false)) == true && 
            user->isPassword(request.get(PASSWORD, false)) == true){
            userLoggined = user;
            return;
        }
    throw BadRequest();
}

int RequestManager::getUserId(){
    userIdCounter++;
    return userIdCounter - 1;
}