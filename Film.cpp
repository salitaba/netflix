#include"Film.h"

using namespace std;


Film::Film(string _name, string _year, string _length, string _price,string _summary,
        string _director, int _id, User* _aurtor){
    name = _name;
    year = _year;
    length = _length;
    price = _price;
    summary = _summary;
    director =  _director;
    id = _id;
    aurtor = _aurtor;
}

bool Film::isId(int _id){
    return id == _id;
}

bool Film::isUser(User* _user){
    return aurtor == _user;
}