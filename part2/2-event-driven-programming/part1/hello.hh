#ifndef __TUTORIAL_HELLO_HH__
#define __TUTORIAL_HELLO_HH__

#include "params/Hello.hh"
#include "sim/sim_object.hh"

class Hello : public SimObject {
    private:
        void processEvent();

        EventWrapper<Hello, &Hello::processEvent> event;
    public: 
        Hello(HelloParams *p);

        void startup();
};

#endif // __TUTORIAL_HELLO_HH__