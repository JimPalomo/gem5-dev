#include "tutorial/simple_cache.hh"
#include "base/random.hh"
#include "debug/CacheDebug.hh"
#include "sim/system.hh"

SimpleCache::SimpleCache(SimpleCacheParams *params) :
    MemObject(params),
    latency(params->latency),
    blockSize(params->system->cacheLineSize()),
    capacity(params->size / blockSize),
    memPort(params->name + ".mem_side", this),
    blocked(false), 
    originalPacket(nullptr), 
    waitingPortId(-1)
{
    for (int i = 0; i < params-> port_cpu_side_connection_count; i++) {
        cpuPorts.emplace_back(name() + csprintf(".cpu_side[%d]", i), i, this);
    }
}

Port &
SimpleCache::getPort(const std::string &if_name, PortID idx) {
    if (if_name == "mem_side") {
        panic_if(idx != InvalidPortID, "Mem side of simple cache not a vector port");
        return memPort;
    } else if (if_name == "cpu_side" && idx < cpuPorts.size()) {
        return cpuPorts[idx];
    } else {
        return MemObject::getPort(if_name, idx);
    }
}

void SimpleCache::CPUSidePort::sendPacket(PacketPtr pkt) {
    panic_if(blockedPacket != nullptr, "Should never try to send if blocked!");

    DPRINTF(CacheDebug, "Sending %s to CPU\n", pkt->print());
    if (!sendTimingResp(pkt)) {
        DPRINTF(CacheDebug, "failed!\n");
        blockedPacket = pkt;
    }
}

AddrRangeList
SimpleCache::CPUSidePort::getAddrRanges() const {
    return owner->getAddrRanges();
}

void SimpleCache::CPUSidePort::trySendRetry() {
    if (needRetry && blockedPacket == nullptr) {
        needRetry = false;
        DPRINTF(CacheDebug, "Sending retry req.\n");
        sendRetryReq();
    }
}

void SimpleCache::CPUSidePort::recvFunctional(PacketPtr pkt) {
    return owner->handleFunctional(pkt);
}

bool SimpleCache::CPUSidePort::recvTimingReq(PacketPtr pkt) {
    DPRINTF(CacheDebug, "Got request %s\n", pkt->print());

    if (blockedPacket || needRetry) {
        DPRINTF(CacheDebug, "Request blocked\n");
        needRetry = true;
        return false;
    }

    if (!owner-> handleRequest(pkt, id)) {
        DPRINTF(CacheDebug, "Request failed\n");
        needRetry = true;
        return false;
    } else {
        DPRINTF(CacheDebug, "Request succeeded\n");
        return true;
    }
}

void SimpleCache::CPUSidePort::recvRespRetry() {
    assert(blockedPacket != nullptr);

    PacketPtr pkt = blockedPacket;
    blockedPacket = nullptr;

    DPRINTF(CacheDebug, "Retrying response pkt %s\n", pkt->print());

    sendPacket(pkt);

    trySendRetry();
}

void SimpleCache::MemSidePort::sendPacket(PacketPtr pkt) {
    panic_if(blockedPacket != nullptr, "Should never try to send if blocked!");

    if (!sendTimingReq(pkt)) {
        blockedPacket = pkt;
    }
}

bool SimpleCache::MemSidePort::recvTimingResp(PacketPtr pkt) {
    return owner->handleResponse(pkt);
}

void SimpleCache::MemSidePort::recvReqRetry() {
    assert(blockedPacket != nullptr);
    
    PacketPtr pkt = blockedPacket;
    blockedPacket = nullptr;

    sendPacket(pkt);
}

void SimpleCache::MemSidePort::recvRangeChange() {
    owner->sendRangeChange();
}

bool SimpleCache::handleRequest(PacketPtr pkt, int port_id) {
    if (blocked) {
        return false;
    }

    DPRINTF(CacheDebug, "Got request for addr %#x\n", pkt->getAddr());

    blocked = true;

    assert(waitingPortId == -1);
    waitingPortId = port_id;

    schedule(new EventFunctionWrapper([this, pkt]{ accessTiming(pkt);}, name() + ".accessEvent", true), clockEdge(latency));

    return true;
}

bool SimpleCache::handleResponse(PacketPtr pkt) {
    assert(blocked);
    DPRINTF(CacheDebug, "Got response for addr %#x\n", pkt->getAddr());

    insert(pkt);

    missLatency.sample(curTick() - missTime);

    if (originalPacket != nullptr) {
        DPRINTF(CacheDebug, "Copying data from new packet to old\n");


        bool hit M5_VAR_USED = accessFunctional(originalPacket);
        panic_if(!hit, "Should always hit after inserting");
        originalPacket->makeResponse();
        delete pkt;
        pkt = originalPacket;
        originalPacket = nullptr;
    }

    sendResponse(pkt);

    return true;
}

void SimpleCache::sendResponse(PacketPtr pkt) {
    assert(blocked);
    DPRINTF(CacheDebug, "Sending resp for addr %#x\n", pkt->getAddr());

    int port = waitingPortId;

    blocked = false;
    waitingPortId = -1;

    cpuPorts[port].sendPacket(pkt);

    for (auto& port : cpuPorts) {
        port.trySendRetry();
    }
}

void SimpleCache::handleFunctional(PacketPtr pkt) {
    if (accessFunctional(pkt)) {
        pkt->makeResponse(); 
    } else {
        memPort.sendFunctional(pkt);
    }
}

void SimpleCache::accessTiming(PacketPtr pkt) {
    bool hit = accessFunctional(pkt);

    DPRINTF(CacheDebug, "%s for packet: %s\n", hit ? "Hit" : "Miss", pkt->print());

    if (hit) {
        hits++;
        DDUMP(CacheDebug, pkt->getConstPtr<uint8_t>(), pkt->getSize());
        pkt->makeResponse();
        sendResponse(pkt);
    } else {
        misses++;
        missTime = curTick();
        Addr addr = pkt->getAddr();
        Addr block_addr = pkt->getBlockAddr(blockSize);
        unsigned size = pkt->getSize();
        if (addr == block_addr && size == blockSize) {
            DPRINTF(CacheDebug, "forwarding packet\n");
            memPort.sendPacket(pkt);
        } else {
            DPRINTF(CacheDebug, "Upgrading packet to block size\n");
            panic_if(addr - block_addr + size > blockSize, "Cannot handle accesses that span multiple cache lines");

            assert(pkt->needsResponse());
            MemCmd cmd;
            if (pkt->isWrite() || pkt->isRead()) {
                cmd = MemCmd::ReadReq;
            } else {
                panic("Unknown packet type in upgrade size");
            }

            PacketPtr new_pkt = new Packet(pkt->req, cmd, blockSize);
            new_pkt->allocate();

            assert(new_pkt->getAddr() == new_pkt->getBlockAddr(blockSize));

            originalPacket = pkt;

            DPRINTF(CacheDebug, "forwarding packet\n");
            memPort.sendPacket(new_pkt);
        }
    }
}

bool SimpleCache::accessFunctional(PacketPtr pkt) {
    Addr block_addr = pkt->getBlockAddr(blockSize);
    auto it = cacheStore.find(block_addr);

    if (it != cacheStore.end()) {
        if (pkt->isWrite()) {
            pkt->writeDataToBlock(it->second, blockSize);
        } else if (pkt->isRead()) {
            pkt->setDataFromBlock(it->second, blockSize);
        } else {
            panic("Unknown packet type!");
        }
        return true;
    }
    return false;
}

void SimpleCache::insert(PacketPtr pkt) {
    assert(pkt->getAddr() == pkt->getBlockAddr(blockSize));

    assert(cacheStore.find(pkt->getAddr()) == cacheStore.end());

    assert(pkt->isResponse());

    if (cacheStore.size() >= capacity) {
        int bucket, bucket_size;
        do {
            bucket = random_mt.random(0, (int)cacheStore.bucket_count() - 1);
        } while ((bucket_size = cacheStore.bucket_size(bucket)) == 0);
        auto block = std::next(cacheStore.begin(bucket), random_mt.random(0, bucket_size - 1));

        DPRINTF(CacheDebug, "Removing addr %#x\n", block->first);

        RequestPtr req = std::make_shared<Request> (block->first, blockSize, 0, 0);
        // RequestPtr req = new Request(block->first, blockSize, 0, 0);

        PacketPtr new_pkt = new Packet(req, MemCmd::WritebackDirty, blockSize);
        new_pkt->dataDynamic(block->second);

        DPRINTF(CacheDebug, "Writing packet back %s\n", pkt->print());

        memPort.sendPacket(new_pkt);

        cacheStore.erase(block->first);
    }

    DPRINTF(CacheDebug, "Inserting %s\n", pkt->print());
    DDUMP(CacheDebug, pkt->getConstPtr<uint8_t>(), blockSize);

    uint8_t *data = new uint8_t[blockSize];

    cacheStore[pkt->getAddr()] = data;

    pkt->writeDataToBlock(data, blockSize);
}

AddrRangeList 
SimpleCache::getAddrRanges() const {
    DPRINTF(CacheDebug, "Sending new ranges\n");

    return memPort.getAddrRanges();
}

void SimpleCache::sendRangeChange() const {
    for (auto& port : cpuPorts) {
        port.sendRangeChange();
    }
}

void SimpleCache::regStats() {
    MemObject::regStats();

    hits.name(name() + ".hits").desc("Number of hits");

    misses.name(name() + ".misses").desc("Number of misses");

    missLatency.name(name() + ".missLatency").desc("Ticks for misses to the cache").init(16);

    hitRatio.name(name() + ".hitRatio").desc("The ratio of hits to the total accesses to the cache");

    hitRatio = hits / (hits + misses);
}

SimpleCache*
SimpleCacheParams::create() {
    return new SimpleCache(this);
}