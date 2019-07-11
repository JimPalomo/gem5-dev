#include "tutorial/hello.hh"
#include <iostream>
using namespace std;

Hello::Hello(HelloParams *params) : SimObject(params) {
    cout << "Hello World! From a SimObject!" << endl;
}

Hello* 
HelloParams::create() {
    return new Hello(this);
}

