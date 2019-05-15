#include"RequestManager.h"

#define PUT "PUT"
#define POST "POST"
#define DELETE "DELETE"
#define GET "GET"
#define USERNAME "username"
#define PASSWORD "password"
#define EMAIL "email"
#define AGE "age"
#define NAME "name"
#define YEAR "year"
#define LENGTH "length"
#define PRICE "price"
#define SUMMARY "summary"
#define DIRECTOR "director"
#define FILM_ID "film_id"

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
    if (request.getMethod() == POST && request.getQuery() == "signup")
        return this->signup(request);
    if(request.getMethod() == POST && request.getQuery() == "login")
        return this->login(request);
    if(request.getMethod() == POST && request.getQuery() == "films")
        return this->postFilm(request);    
    if(request.getMethod() == PUT && request.getQuery() == "films")
        return this->editFilm(request);
    if(request.getMethod() == DELETE && request.getQuery() == "films")
        return this->deleteFilm(request);
    if(request.getMethod() == GET && request.getQuery() == "followers")
        return this->showFollowers(request);
    throw BadRequest();

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
    return userIdCounter;
}

int RequestManager::getFilmId(){
    filmIdCounter++;
    return filmIdCounter;
}

void RequestManager::postFilm(Request request){
    vector<string> requirementField{NAME, YEAR, LENGTH, PRICE, SUMMARY, DIRECTOR};
    request.check(requirementField);
    if(userLoggined == NULL || userLoggined->isPublisher() == false)
        throw PermissionDenied();

    string name = request.get(NAME), year = request.get(YEAR);
    string length = request.get(LENGTH), price = request.get(PRICE);
    string summary = request.get(SUMMARY), director = request.get(DIRECTOR);

    films.push_back(new Film(name, year, length, price, summary, director, this->getFilmId(), userLoggined));
}

void RequestManager::editFilm(Request request){
    vector<string> requirementField{FILM_ID};
    request.check(requirementField);
    if(userLoggined == NULL || userLoggined->isPublisher() == false)
        throw PermissionDenied();

    int id = atoi(request.get(FILM_ID).c_str());
    Film* film = this->getFilm(id);

    string name = request.get(NAME, true), year = request.get(YEAR, true);
    string length = request.get(LENGTH, true), price = request.get(PRICE, true);
    string summary = request.get(SUMMARY, true), director = request.get(DIRECTOR, true);

    film->setDetail(name, year, length, price, summary, director);
}

Film* RequestManager::getFilm(int id){
    for(auto film : films)
        if(film->isId(id) == true){
            if( film->isUser(userLoggined) == true)
                return film;
            throw PermissionDenied();
        } 
    throw NotFound();
}

void RequestManager::deleteFilm(Request request){
    vector<string> requirement{FILM_ID};
    request.check(requirement);

    if(userLoggined == NULL || userLoggined->isPublisher() == false)
        throw PermissionDenied();

    int id = atoi(request.get(FILM_ID).c_str());
    Film* film = this->getFilm(id);

    film->unusable();
}

void RequestManager::showFollowers(Request request){
    
    if(userLoggined == NULL)
        throw PermissionDenied();
    
    userLoggined->showFollower();
}