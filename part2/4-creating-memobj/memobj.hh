#ifndef __TUTORIAL_MEMOBJ_HH__
#define __TUTORIAL_MEMOBJ_HH__

#include "mem/mem_object.hh"
// #include "tutorial/memobj.hh"
#include "params/Memobj.hh"

class Memobj : public MemObject {
    private:
        class CPUSidePort : public SlavePort {
            private: 
                Memobj *owner;

                bool needRetry;

                PacketPtr blockedPacket;
    
            public:
                CPUSidePort(const std::string& name, Memobj *owner) :
                    SlavePort(name, owner),
                    owner(owner),
                    needRetry(false),
                    blockedPacket(nullptr)
                { }

                void sendPacket(PacketPtr pkt);

                AddrRangeList getAddrRanges() const override;

                void trySendRetry();

            protected:
                Tick recvAtomic(PacketPtr pkt) override {
                    panic("recvAtomic unimpl.");
                }

                void recvFunctional(PacketPtr pkt) override;

                bool recvTimingReq(PacketPtr pkt) override;

                void recvRespRetry() override;

                };

            class MemSidePort : public MasterPort {
                private:
                    Memobj *owner;

                    PacketPtr blockedPacket;

                public:
                    MemSidePort(const std::string& name, Memobj *owner) :
                        MasterPort(name, owner),
                        owner(owner),
                        blockedPacket(nullptr)
                    { }

                    void sendPacket(PacketPtr pkt);

                protected:
                    bool recvTimingResp(PacketPtr pkt) override;

                    void recvReqRetry() override;

                    void recvRangeChange() override;
            };

            bool handleRequest(PacketPtr pkt);

            bool handleResponse(PacketPtr pkt);

            void handleFunctional(PacketPtr pkt);

            AddrRangeList getAddrRanges() const;

            void sendRangeChange();

            CPUSidePort instPort;
            CPUSidePort dataPort;

            MemSidePort memPort;

            bool blocked;

    public:
        Memobj(MemobjParams *params);

        Port &getPort(const std::string &if_name, PortID idx=InvalidPortID) override;
};



#endif // __TUTORIAL_SIMPLE_MEMOBJ_HH__
