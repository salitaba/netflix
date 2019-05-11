#include<string>
#include<iostream>

#include"RequestManager.h"

using namespace std;


int main(){
    RequestManager requestManager;
    requestManager.handle("POST signup ? ali ali 1234 16 false");
    requestManager.handle("POST login ? ali 1234");

}