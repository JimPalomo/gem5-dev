#ifndef __TUTORIAL_HELLO_HH__
#define __TUTORIAL_HELLO_HH__

#include "tutorial/goodbye.hh"
#include "params/Hello.hh"
#include "sim/sim_object.hh"

class Hello : public SimObject {
    private:
        void processEvent();
        
        EventFunctionWrapper event;

        Goodbye* goodbye;
        
        const std::string myName;
        
        const Tick latency;

        int timesLeft;
    public: 
        Hello(HelloParams *p);

        void startup();
};

#endif // __TUTORIAL_HELLO_HH__