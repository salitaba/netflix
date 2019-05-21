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