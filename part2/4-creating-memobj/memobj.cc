#ifndef __TUTORIAL_MEMOBJ_HH__
#define __TUTORIAL_MEMOBJ_HH__

#include "mem/mem_object.hh"
#include "params/Memobj.hh"

// Creating a memory object that forwards requests and responses (no cache added)
class Memobj : public MemObject {
    private:

        // Receives requests and forwards those to the owner
        class CPUSidePort : public SlavePort {
            private: 
                // Object owned by Memobj
                Memobj *owner;

                // Used to determine if a request needs to be resent (T = Y)
                bool needRetry;

                // Stores one packet that is blocked
                PacketPtr blockedPacket;
    
            public:

                // Constructor
                CPUSidePort(const std::string& name, Memobj *owner) :
                    SlavePort(name, owner),
                    owner(owner),
                    needRetry(false),
                    blockedPacket(nullptr)
                { }

                // Packets are sent from this port when called by the owner. Flow of packets are done here
                void sendPacket(PacketPtr pkt);

                // Gathers list of address ranges which the owner is responsible for. Slave ports override this function to return a populated list (w/ one item at minimum)
                AddrRangeList getAddrRanges() const override;

                // Send a retry to a nearby port if Memobj whenever is not blocked
                void trySendRetry();

            protected:

                // Receives atomic packet requested by the master port (Used for atomic requests, faster version of timing mode, not used in this memobj)
                Tick recvAtomic(PacketPtr pkt) override {
                    panic("recvAtomic unimpl.");
                }

                // Receives functional packet requested by master port (Used for functional requests, for debugging purposes)
                void recvFunctional(PacketPtr pkt) override;

                // Receives timing request from master port
                bool recvTimingReq(PacketPtr pkt) override;

                // Called by master port if sendTimingResp was called on this specific slave port
                void recvRespRetry() override;

        };

        // Port on memory-side (receives responses but forwards msot of the requests to owner)
        class MemSidePort : public MasterPort {
            private:
                
                // Object owned by Memobj
                Memobj *owner;

                // Stores blocked packet
                PacketPtr blockedPacket;

            public:
                
                // Constructor
                MemSidePort(const std::string& name, Memobj *owner) :
                    MasterPort(name, owner),
                    owner(owner),
                    blockedPacket(nullptr)
                    { }

                // Sends packets through this port and is called by the owner
                void sendPacket(PacketPtr pkt);

            protected:

                // Receive a timing response (from slave port)
                bool recvTimingResp(PacketPtr pkt) override;

                // Called by slave port (if sendTimingReq called on this master port)
                void recvReqRetry() override;

                // Receives address ranges from peer slave port
                void recvRangeChange() override;
        };

        // Handles requests on the CPU-side
        bool handleRequest(PacketPtr pkt);

        // Handles responses from memory side
        bool handleResponse(PacketPtr pkt);

        // Handles functional packets (debugging purposes)
        void handleFunctional(PacketPtr pkt);

        // Returns address ranges 
        AddrRangeList getAddrRanges() const;

        // Requests for memory ranges
        void sendRangeChange();

        // Instantiation of CPU-side ports
        CPUSidePort instPort;
        CPUSidePort dataPort;

        // Instantiation of memory-side port
        MemSidePort memPort;

        // If blocked requests is present, then wait for a response
        bool blocked;

    public:

        // Constructor
        Memobj(MemobjParams *params);
        
        // Get port information when given a name & index
        Port &getPort(const std::string &if_name, PortID idx=InvalidPortID) override;
};



#endif // __TUTORIAL_SIMPLE_MEMOBJ_HH__

