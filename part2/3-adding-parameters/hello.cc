#include "tutorial/hello.hh"
#include "debug/HelloDebug.hh"
// #include "base/misc.hh"

Hello::Hello(HelloParams *params) : 
    SimObject(params), 
    event([this]{processEvent();}, name() + ".event"),
    // event(*this),
    goodbye(params->goodbye),
    myName(params->name), 
    latency(params->time_to_wait), 
    timesLeft(params->number_of_fires)
{
    DPRINTF(HelloDebug, "Created the hello object with the name %s\n", myName);
    panic_if(!goodbye, "HelloObject must have a non-null GoodbyeObject");
}

void Hello::startup() {
    schedule(event, latency);
}

void Hello::processEvent() {
    timesLeft--;
    DPRINTF(HelloDebug, "Hello world! Processing the event! %d left\n", timesLeft);

    if (timesLeft <= 0) {
        DPRINTF(HelloDebug, "Done firing!\n");
        goodbye->sayGoodbye(myName);
    } else {
        schedule(event, curTick() + latency);
    }
}

Hello* 
HelloParams::create() {
    return new Hello(this);
}
