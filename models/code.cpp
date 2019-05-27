#include<iostream>

using namespace std;

double f(int number, int x){
    if(number == 0)
        return 1;
    return 1 / (1 + f(number - 1, x) );
}

int main(){

    cout << f(3,4) << endl;
    return 0;
}