#include<string>
#include<iostream>

#include"RequestManager.h"

using namespace std;


int main(){
    RequestManager requestManager;
    requestManager.handle(" POST signup ? email ali username   ali password 1234 age 16 publisher false");
    requestManager.handle("POST login ? username ali password 1234");
}