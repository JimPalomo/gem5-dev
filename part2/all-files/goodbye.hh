#ifndef __TUTORIAL_GOODBYE_HH__
#define __TUTORIAL_GOODBYE_HH__

#include <string>

#include "params/Goodbye.hh"
#include "sim/sim_object.hh"

class Goodbye : public SimObject {
    private:
        void processEvent();
        
        void fillBuffer();

        EventFunctionWrapper event;

        float bandwidth;        // Bytes processed per tick

        int bufferSize;         // The size of the buffer that is going to be filled

        char *buffer;           // Pointer to the buffer that is being used

        std::string message;    // Message placed in buffer

        int bufferUsed;         // Represent amount of buffer that is used

    public:
        Goodbye(GoodbyeParams *p);
        ~Goodbye();

        void sayGoodbye(std::string name);
};

#endif // __TUTORIAL_GOODBYE_HH__
