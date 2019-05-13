#include<string>
#include<iostream>

#include"RequestManager.h"

using namespace std;


int main(){
    RequestManager requestManager;
    requestManager.handle("POST signup ? ali ali 1234 16 false");
    requestManager.handle("POST login ? ali 12345");
    queue<string>s;
    cout<<"OK"<<endl;
    s.push("!");
    cout<<"OK"<<endl;
    s.push("@");
    cout<<s.front();
}