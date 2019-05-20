#include<string>
#include<iostream>

#include"RequestManager.h"

using namespace std;


int main(){
    RequestManager requestManager;
    requestManager.handle("POST signup ? email ali username   ali password 1234 age 16 publisher true");
    requestManager.handle("POST login ? username ali password 1234");
    requestManager.handle("POST films ? name uncharted year 2018 length 20 price 300 summary go_home director AliTaba");
    requestManager.handle("PUT films ? film_id 1 name uncharted");
    requestManager.handle("DELETE films ? film_id 1");
    requestManager.handle("GET followers ?");
}