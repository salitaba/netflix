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

void Film::setDetail(string _name, string _year, string _length, string _price,string _summary, string _director){
    if(_name.size() > 0)  name = _name;
    if(_year.size() > 0)  year = _year;
    if(_length.size() > 0) length = _length;
    if(_price.size() > 0) price = _price;
    if(_summary.size() > 0) summary = _summary;
    if(_director.size() > 0) director = _director;
}

bool Film::isId(int _id){
    return id == _id;
}

bool Film::isUser(User* _user){
    return aurtor == _user;
}

void Film::unusable(){
    usable = false;
}