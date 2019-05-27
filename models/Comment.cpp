#include"Comment.h"

using namespace std;

Comment::Comment(string _content, int _id, User* _user){
    content = _content;
    id = _id;
    user = _user;
}

void Comment::hidden(){
    online = false;
}

void Comment::reply(string content){
    replies.push_back(content);
}

void Comment::show(){
    cout << id << ". " << content << endl;
    for(int i = 0 ; i < replies.size() ;i++)
        cout << id << "." <<  i + 1 << ". " << replies[i] << endl;
}

bool Comment::isId(int _id){
    if(online == false)
        return false;
    return id == _id;
}

User* Comment::getUser(){
    return user;
}

bool Comment::isLive(){
    return online;
}

