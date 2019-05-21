#include"Comment.h"

using namespace std;

Comment::Comment(string _content, int _id){
    content = _content;
    id = _id;
}

void Comment::hidden(){
    online = false;
}