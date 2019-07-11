#include "tutorial/hello.hh"
#include <iostream>
#include "debug/HelloDebug.hh"
using namespace std;

Hello::Hello(HelloParams *params) : 
    SimObject(params), event([this]{processEvent();}, name())
{
    // cout << "Hello World! From a SimObject!" << endl;
    // DPRINTF(HelloDebug, "Created the hello object\n");
}

Hello* 
HelloParams::create() {
    return new Hello(this);
}


void Hello::processEvent() {
    DPRINTF(HelloDebug, "Hello world! Processing the event!\n");
}

void Hello::startup() {
    schedule(event, 100);
}
