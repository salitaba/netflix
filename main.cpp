#include<string>
#include<iostream>

#include"RequestManager.h"

using namespace std;


int main(){
    RequestManager requestManager;
    requestManager.handle(" POST signup ? email ali user   ali password 1234 age 16 publisher false");
    requestManager.handle("POST login ? user ali password 12345");
}