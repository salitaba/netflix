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

User::User(string _email, string _userName, string _password, int _age, int _id){
    email = _email;
    userName = _userName;
    password = this->hashCode(_password);
    age = _age;
    id = _id;
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