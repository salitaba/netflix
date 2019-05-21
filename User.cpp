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

bool User::isPublisher(){
    return publisher;
}

void User::showFollower(){
    cout << "List of Followers" << endl;
    cout << "#. User Id | User Username | User Email" << endl;
    int counter = 1;
    for(auto user : followers){
        cout << counter << ". " << user->id << " | " << user->userName << " | " << user->email << endl;
        counter++;
    }
}

void User::find(string name,string minYear,string minRate,string price,string maxYear,string director){
    cout<<"#. Film Id | Film Name | Film length | Film price | Rate | Production Year | Film Director"<<endl;
    int counter = 1; 
    for(auto film : films){
        if( film->find(name, minYear, minRate, price, maxYear, director) == true)
            film->printDetail(counter), counter++;
    }
}

void User::buy(Film* film){
    int price = film->getPrice();

    if(money < price) 
        throw BadRequest();
    
    buyedFilm.insert(film);

    money -= price;
    film->getAuthor()->sold(this, film);
}

void User::sold(User* user, Film* film){
    solds.insert(make_pair(film->getPrice(), film->getRate()));
}

bool User::checkBuyFilm(Film* film){
    if(buyedFilm.size() == 0 || buyedFilm.find(film) == buyedFilm.end())
        return false;
    return true;
}

void User::increaseMoney(int value){
    money += value;
}

void User::getMoney(){
    for(auto userAndPrice : solds){
        double rate = userAndPrice.second;
        int price = userAndPrice.first;
        if(rate < 5)
            money += price * 0.8;
        else if(rate >= 5 && rate < 8)
            money += price * 0.9;
        else
            money += price * 0.95;
    }
    solds.clear();
}

bool User::isBuyed(Film* film){
    return buyedFilm.find(film) != buyedFilm.end();
}

void User::sendNotification(std::string notification){
    unreadedNotification.push_back(notification);
    allNotification.push_back(notification);
}

string User::createPostFilmNotif(){
    return "Publisher " + userName + " with id " + to_string(id) + " register new film.";
}

string User::createReplyNotif(){
    return "Publisher " + userName + " with id " + to_string(id) + " reply to your comment.";
}

bool User::isId(int _id){
    return id == _id;
}

void User::addFollowers(User* user){
    followers.insert(user);

    // this->sendNotification(user->createFollowNotif());
}

string User::createFollowNotif(){
    return "User " + userName + " with id " + to_string(id) + " follow you.";
}

string User::createBuyFilmNotif(Film* film){
    return "User " + userName + " with id " + to_string(id) + " buy your film " +
            film->getName() + " with id " + to_string(film->getId()) + ".";
}

string User::createRateNotif(Film* film){
    return "User " + userName + " with id " + to_string(id) + " rate your film " + 
            film->getName() + " with id " + to_string(film->getId()) + ".";
}

string User::createCommentNotif(Film* film){
    return "User " + userName + " with id " + to_string(id) + " comment on your film " + 
            film->getName() + " with id " + to_string(film->getId()) + ".";
}

void User::showNotification(){
    cout<<"#. Notification Massage"<<endl;
    int cnt = 1;
    if(unreadedNotification.size() > 0)
        for(int i = unreadedNotification.size() - 1; i >= 0 ; i--){
            cout << cnt << ". " << unreadedNotification[i] << endl;
            cnt++;
        }
    unreadedNotification.clear();
}

void User::showLimitedNotification(int limit){
    cout<<"#. Notification Massage"<<endl;
    int cnt = 1;
    if(allNotification.size() > 0)
        for(int i = allNotification.size() - 1; i >= 0 && cnt <= limit; i--){
            cout << cnt << ". " << allNotification[i] << endl;
            cnt++;
        }
}

void User::findBuyedFilm(string name,string minYear,string minRate,string price,string maxYear,string director){
    cout<<"#. Film Id | Film Name | Film length | Film price | Rate | Production Year | Film Director"<<endl;
    int counter = 1; 
    for(auto film : buyedFilm){
        if( film->find(name, minYear, minRate, price, maxYear, director) == true)
            film->printDetail(counter), counter++;
    }
}

void User::addFilm(Film* film){
    films.push_back(film);
}