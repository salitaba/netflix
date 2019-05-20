#include"Film.h"

using namespace std;


Film::Film(string _name, string _year, string _length, string _price,string _summary,
        string _director, int _id, User* _aurtor){
    name = _name;
    year = atoi(_year.c_str());
    length = atoi(_length.c_str());
    price = atoi(_price.c_str());
    summary = _summary;
    director =  _director;
    id = _id;
    aurtor = _aurtor;
}

void Film::setDetail(string _name, string _year, string _length, string _price,string _summary, string _director){
    if(_name.size() > 0)  name = _name;
    if(_year.size() > 0)  year = atoi(_year.c_str());
    if(_length.size() > 0) length =atoi( _length.c_str());
    if(_price.size() > 0) price = atoi(_price.c_str());
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

bool Film::find(string _name,string _minYear,string _minRate,string _price,string _maxYear,string _director){
    if(_name.size() > 0 &&  name != _name) return false;
    if(_director.size() > 0 &&  director != _director ) return false;
    if(_minYear.size() > 0 &&  atoi(_minYear.c_str()) > year) return false;
    if(_maxYear.size() > 0 && atoi(_maxYear.c_str()) < year) return false;
    if(_minRate.size() > 0 && atoi(_minRate.c_str()) > rate ) return false;
    return true;
}

void Film::printDetail(int counter){
    cout<< counter << ". " << id << " | " << name << " | " << length << " | " 
        << price << " | " << rate << " | " << year << " | " << director <<endl;
}

void Film::printInformation(){
    cout<<"Details of Film "<< name << endl << "ID = " << id << endl << "Director = " << director << endl
        <<"Length = " << length << "Year = " << year << "Summary = " << summary << endl << "Price = " << price << endl;
}

int Film::getPrice(){
    return price;
}

double Film::getRate(){
    return rate;
}

User* Film::getAuthor(){
    return aurtor;
}

void Film::rateTheRate(int rateNum, User* user){
    if(userRated.size() != 0 && userRated.find(user) == userRated.end())
        return;

    int sumRate = rate * userRated.size();
    userRated.insert(user);
    sumRate += rateNum;
    rate = (double)rateNum / userRated.size();
}

void Film::addComment(string content, User* user){
    comments.push_back(make_pair( content, user));
}