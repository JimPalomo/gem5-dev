#include "tutorial/hello.hh"
#include <iostream>
#include "debug/HelloDebug.hh"
using namespace std;

Hello::Hello(HelloParams *params) : 
    SimObject(params), event([this]{processEvent();}, name()), latency(100), timesLeft(10)
{
    // cout << "Hello World! From a SimObject!" << endl;
    // DPRINTF(HelloDebug, "Created the hello object\n");
}

Hello* 
HelloParams::create() {
    return new Hello(this);
}


void Hello::processEvent() {
    timesLeft--;
    DPRINTF(HelloDebug, "Hello world! Processing the event! %d left\n", timesLeft);

    if (timesLeft <= 0) {
        DPRINTF(HelloDebug, "Done firing!\n");
    } else {
        schedule(event, curTick() + latency);
    }
}

void Hello::startup() {
    schedule(event, latency);
}