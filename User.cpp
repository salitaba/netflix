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
    for(auto user : followers)
        cout << counter << ". " << user->id << " | " << user->userName << " | " << user->email << endl;
}