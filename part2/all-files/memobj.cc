#include "tutorial/memobj.hh"
#include "debug/Memobj.hh"

Memobj::Memobj(MemobjParams *params) :
    MemObject(params),
    instPort(params->name + ".inst_port", this),
    dataPort(params->name + ".data_port", this),
    memPort(params->name + ".mem_side", this),
    blocked(false)
{}

Port &
Memobj::getPort(const std::string &if_name, PortID idx) {
    panic_if(idx != InvalidPortID, "This object doesn't support vector ports");

    if (if_name == "mem_side") {
        return memPort;
    } else if (if_name == "inst_port") {
        return instPort;
    } else if (if_name == "data_port") {
        return dataPort;
    } else {
        return MemObject::getPort(if_name, idx);
    }
}

void Memobj::CPUSidePort::sendPacket(PacketPtr pkt) {
    panic_if(blockedPacket != nullptr, "Should never try to send if blocked!");

    if (!sendTimingResp(pkt)) {
        blockedPacket = pkt;
    }
}

AddrRangeList 
Memobj::CPUSidePort::getAddrRanges() const {
    return owner->getAddrRanges();
}

void Memobj::CPUSidePort::trySendRetry() {
    if (needRetry && blockedPacket == nullptr) {
        needRetry = false;
        DPRINTF(Memobj, "Sending retry req for %d\n", id);
        sendRetryReq();
    }
}

void Memobj::CPUSidePort::recvFunctional(PacketPtr pkt) {
    return owner->handleFunctional(pkt);
}

bool Memobj::CPUSidePort::recvTimingReq(PacketPtr pkt) {
    if (!owner->handleRequest(pkt)) {
        needRetry = true;
        return false;
    } else {
        return true;
    }
}

void Memobj::CPUSidePort::recvRespRetry() {
    assert(blockedPacket != nullptr);

    PacketPtr pkt = blockedPacket;
    blockedPacket = nullptr;

    sendPacket(pkt);
}

// Huge error occurred after fixing this line
void Memobj::MemSidePort::sendPacket(PacketPtr pkt) {
    panic_if(blockedPacket != nullptr, "Should never try to send if blocked!");

    if (!sendTimingReq(pkt)) {
        blockedPacket = pkt;
    }
}

bool Memobj::MemSidePort::recvTimingResp(PacketPtr pkt) {
    return owner->handleResponse(pkt);
}

void Memobj::MemSidePort::recvReqRetry() {
    assert(blockedPacket != nullptr);

    PacketPtr pkt = blockedPacket;
    blockedPacket = nullptr;

    sendPacket(pkt);
}

void Memobj::MemSidePort::recvRangeChange() {
    owner->sendRangeChange();
}

bool Memobj::handleRequest(PacketPtr pkt) {
    if (blocked) {
        return false;
    }

    DPRINTF(Memobj, "Got request for addr %#x\n", pkt->getAddr());

    blocked = true;

    memPort.sendPacket(pkt);

    return true;
}

bool Memobj::handleResponse(PacketPtr pkt) {
    assert(blocked);

    DPRINTF(Memobj, "Got response for addr %#x\n", pkt->getAddr());

    blocked = false;

    if (pkt->req->isInstFetch()) {
        instPort.sendPacket(pkt);
    } else {
        dataPort.sendPacket(pkt);
    }

    instPort.trySendRetry();
    dataPort.trySendRetry();

    return true;
}

void Memobj::handleFunctional(PacketPtr pkt) {
    memPort.sendFunctional(pkt);
}

AddrRangeList
Memobj::getAddrRanges() const {
    DPRINTF(Memobj, "Sending new ranges\n");
    return memPort.getAddrRanges();
}

void Memobj::sendRangeChange() {
    instPort.sendRangeChange();
    dataPort.sendRangeChange();
}

Memobj*
MemobjParams::create() {
    return new Memobj(this);
}