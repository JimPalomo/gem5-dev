#include "tutorial/goodbye.hh"

#include "debug/HelloDebug.hh"
#include "sim/sim_exit.hh"

Goodbye::Goodbye(GoodbyeParams *params) :
    SimObject(params), 
    // event(*this), 
    event([this]{ processEvent(); }, name() + ".event"),
    bandwidth(params->write_bandwidth),
    bufferSize(params->buffer_size),
    buffer(nullptr),
    bufferUsed(0)
{
    buffer = new char[bufferSize];
    DPRINTF(HelloDebug, "Created the goodbye object\n");
}

Goodbye::~Goodbye() {
    delete[] buffer;
}

void Goodbye::processEvent() {
    DPRINTF(HelloDebug, "Processing the event!\n");
    fillBuffer();
}

void Goodbye::sayGoodbye(std::string other_name) {
    DPRINTF(HelloDebug, "Saying goodbye to %s\n", other_name);

    message = "Goodbye " + other_name + "!! ";

    fillBuffer();
}

void Goodbye::fillBuffer() {
    assert(message.length() > 0);

    int bytes_copied = 0;
    for(auto it = message.begin();
        it < message.end() && bufferUsed < bufferSize - 1; it++, bufferUsed++, bytes_copied++) {
            buffer[bufferUsed] = *it;
        }
    
    if (bufferUsed < bufferSize - 1) {
        DPRINTF(HelloDebug, "Scheduling another fillBuffer in %d ticks\n", bandwidth * bytes_copied);
        schedule(event, curTick() + bandwidth * bytes_copied);
    } else {
        DPRINTF(HelloDebug, "Goodbye done copying!\n");
        exitSimLoop(buffer, 0, curTick() + bandwidth * bytes_copied);
    }
}

Goodbye*
GoodbyeParams::create() {
    return new Goodbye(this);
}

