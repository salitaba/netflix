//  COPY RIGHT

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
#define MIN_RATE "min_rate"
#define MIN_YEAR "min_year"
#define MAX_YEAR "max_rate"
#define RATE "rate"
#define CONTENT "content"
#define AMOUNT "amount"
#define COMMENT_ID "comment_id"
#define USER_ID "user_id"

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
    if( request.getMethod() == POST )
        return this->post(request);
    if( request.getMethod() == PUT )
        return this->put(request);
    if( request.getMethod() == DELETE ) 
        return this->deleteMethod(request);
    if( request.getMethod() == GET )
        return this->getMethod(request);
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

    for(auto user : users)
        user->sendNotification(userLoggined->createPostFilmNotif());
}

void RequestManager::editFilm(Request request){
    vector<string> requirementField{FILM_ID};
    request.check(requirementField);
    if(userLoggined == NULL || userLoggined->isPublisher() == false)
        throw PermissionDenied();

    int id = atoi(request.get(FILM_ID).c_str());
    Film* film = this->getFilm(id);

    if( film->isUser(userLoggined) == false)
        throw PermissionDenied();

    string name = request.get(NAME, true), year = request.get(YEAR, true);
    string length = request.get(LENGTH, true), price = request.get(PRICE, true);
    string summary = request.get(SUMMARY, true), director = request.get(DIRECTOR, true);

    film->setDetail(name, year, length, price, summary, director);
}

Film* RequestManager::getFilm(int id){
    for(auto film : films)
        if(film->isId(id) == true)
            return film;
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

void RequestManager::post(Request request){
    if (request.getQuery() == "signup")
        this->signup(request);
    if(request.getQuery() == "login")
        this->login(request);
    
    if(userLoggined == NULL)
        throw PermissionDenied();

    if(request.getQuery() == "follower")
        this->follow(request);
    if(request.getQuery() == "films")
        this->postFilm(request);    
    if(request.getQuery() == "buy")
        this->buyFilm(request);
    if(request.getQuery() == "rate")
        this->rateFilm(request);
    if(request.getQuery() == "comments")
        this->commentFilm(request);
    if(request.getQuery() == "money")  
        this->moneyHandler(request);
    if(request.getQuery() == "replies")
        this->reply(request);

    cout<<"OK"<<endl;
}

void RequestManager::deleteMethod(Request request){
    if(userLoggined == NULL)
        throw PermissionDenied();

    if(request.getQuery() == "films")
        this->deleteFilm(request);
    if(request.getQuery() == "comment")
        this->deleteComment(request);
    cout<<"OK"<<endl;
}

void RequestManager::put(Request request){
    if(userLoggined == NULL)
        throw PermissionDenied();

    if(request.getQuery() == "films")
        this->editFilm(request);
    cout<<"OK"<<endl;
}

void RequestManager::getMethod(Request request){
    if(userLoggined == NULL)
        throw PermissionDenied();

    if(request.getQuery() == "followers")
        return this->showFollowers(request);
    if(request.getQuery() == "published")
        return this->published(request);
    if(request.getQuery() == "films")
        return this->seprateSearchFromShowDetailFilm(request);
}

void RequestManager::published(Request request){
    string name = request.get(NAME, true), minYear = request.get(MIN_YEAR, true);
    string minRate = request.get(MIN_RATE, true), price = request.get(PRICE, true);
    string maxYear = request.get(MAX_YEAR, true), director = request.get(DIRECTOR, true);

    userLoggined->find(name, minYear, minRate, price, maxYear, director);
}

void RequestManager::seprateSearchFromShowDetailFilm(Request request){
    string filmId = request.get(FILM_ID, true);

    if(filmId.size() > 0)
        this->getFilm(atoi(filmId.c_str()))->printInformation(this->topFilms());
    else
        this->searchFilm(request);
}

void RequestManager::searchFilm(Request request){
    int counter = 1;

    string name = request.get(NAME, true), minYear = request.get(MIN_YEAR, true);
    string minRate = request.get(MIN_RATE, true), price = request.get(PRICE, true);
    string maxYear = request.get(MAX_YEAR, true), director = request.get(DIRECTOR, true);

    for(auto film : films)
        if(film->find(name, minYear, minRate, price, maxYear, director))
            film->printDetail(counter), counter;
    

}

void RequestManager::buyFilm(Request request){
    vector< string > requirementField{ FILM_ID };
    request.check(requirementField);

    Film* film = this->getFilm(atoi(request.get(FILM_ID).c_str()));
    
    userLoggined->buy(film);
    film->getAuthor()->sendNotification(userLoggined->createBuyFilmNotif(film));
}

void RequestManager::rateFilm(Request request){
    vector< string > requiredFields{FILM_ID, RATE};
    request.check(requiredFields);

    Film* film = this->getFilm(atoi(request.get(FILM_ID).c_str()));

    if(userLoggined->checkBuyFilm(film) == true)
        throw PermissionDenied();

    film->rateTheRate(atoi(request.get(RATE).c_str()), userLoggined);
    film->getAuthor()->sendNotification(userLoggined->createRateNotif(film));
}

void RequestManager::commentFilm(Request request){
    vector< string > requiredFieldS{FILM_ID, CONTENT};
    request.check(requiredFieldS);

    Film* film = this->getFilm(atoi(request.get(FILM_ID).c_str()));

    if(userLoggined->checkBuyFilm(film) == true)
        throw PermissionDenied();

    film->addComment(request.get(CONTENT), userLoggined);
}

void RequestManager::increaseMoney(Request request){
    vector< string > requiredFields {AMOUNT};
    request.check(requiredFields);

    userLoggined->increaseMoney(atoi(request.get(AMOUNT).c_str()));
}

void RequestManager::moneyHandler(Request request){
    if(request.get(AMOUNT).size() == 0)
        userLoggined->getMoney();
    else
        this->increaseMoney(request);
}

void RequestManager::reply(Request request){
    vector< string > requiredFields {FILM_ID, COMMENT_ID, CONTENT};
    request.check(requiredFields);

    Film* film = this->getFilm(atoi(request.get(FILM_ID).c_str()));

    if(film->getAuthor() != userLoggined)
        throw PermissionDenied();
    
    int commentId = atoi(request.get(COMMENT_ID).c_str());

    film->reply(commentId, request.get(CONTENT));
    
    User* user = film->getComment(commentId)->getUser();
    user->sendNotification(userLoggined->createReplyNotif());
}

void RequestManager::deleteComment(Request request){
    vector< string > requiredFields {FILM_ID, COMMENT_ID};
    request.check(requiredFields);

    Film* film = this->getFilm(atoi(request.get(FILM_ID).c_str()));

    if(film->getAuthor() != userLoggined)
        throw PermissionDenied();
    
    film->deleteComment(atoi(request.get(COMMENT_ID).c_str()));
}

vector< Film* > RequestManager::topFilms(){
    vector < Film* > topFilm;
    for(int i = 0 ;i < films.size() ;i++)
        for(int j = 0 ;j < films.size() ;j++)
            if(films[j]->getRate() != films[i]->getRate()){
                if(films[j]->getRate() > films[i]->getRate())
                    swap(films[i], films[j]);
            }else{
                if(films[j]->getId() < films[i]->getId())
                    swap(films[i], films[j]);
            }
    int counter = 1, id = 0;
    while(counter < 5 &&  id < films.size()){
        if(userLoggined->isBuyed(films[id]) == false)
            topFilm.push_back(films[id]), counter++;
        id++;
    }
}

void RequestManager::follow(Request request){
    vector< string > requiredFields;
    request.check(requiredFields);

    this->getUser(atoi(request.get(USER_ID).c_str()))->addFollowers(userLoggined);
}

User* RequestManager::getUser(int id){
    for(auto user : users)
        if(user->isId(id) == true)
            return user;
    throw NotFound();
}