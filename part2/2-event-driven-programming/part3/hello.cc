#include "tutorial/hello.hh"
#include "debug/HelloDebug.hh"

Hello::Hello(HelloParams *params) : 
    SimObject(params), 
    event([this]{processEvent();}, 
    name()), 
    latency(params->time_to_wait), 
    timesLeft(params->number_of_fires)
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