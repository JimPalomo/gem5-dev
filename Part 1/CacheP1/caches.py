# Creating a cache configuration file

'''
    General Information:
        Bus: a collection of wires that data passes through measured in Mhz (size of bus depends on how many bits it can transfer at a time ex. 64 bits)
            3 types of busses: 
                Address Bus: sends memory address to memory (RAM) from the CPU (unidirectional)
                Data Bus: sends data to memory and vice versa (bidirectional)
                Control Bus: carries commands from the CPU to devices (bidirectional)
        Latency: measurement of delay before data transfer begins
'''

import m5
from m5.objects import Cache

# Creating L1Cache class derived from Cache parent class
class L1Cache(Cache):
    assoc = 2
    tag_latency = 2
    data_latency = 2
    response_latency = 2
    mshrs = 4               # Miss Status and handling Register
    tgts_per_mshr = 20

    # Adding constructor
    def __init__(self, options = None):
        super(L1Cache, self).__init__()
        pass

# Creating L1 Instrutions & Data Cache
class L1ICache(L1Cache):
    size = '16kB'

    # Adding constructor
    def __init__(self, options = None): 
        super(L1ICache, self).__init__(options)
        if not options or not options.l1i_size:
            return
        self.size = options.l1i_size

    def connectBus(self, bus):
        self.mem_side = bus.slave

    # Adding connections
    def connectCPU(self, cpu):
        self.cpu_side = cpu.icache_port

class L1DCache(L1Cache):
    size = '64kB'

    # Adding constructor
    def __init__(self, options = None):
        super(L1DCache, self).__init__(options)
        if not options or not options.l1d_size:
            return
        self.size = options.l1d_size

    def connectBus(self, bus):
        self.mem_side = bus.slave

    # Adding connections
    def connectCPU(self, cpu):
        self.cpu_side = cpu.dcache_port

# Creating L2Cache class derived from Cache parent class
class L2Cache(Cache):
    size = '256kB'
    assoc = 8
    tag_latency = 20
    data_latency = 20
    response_latency = 20
    mshrs = 20
    tgts_per_mshr = 12

    # Adding constructors
    

    # Adding connections
    def connectCPUSideBus(self, bus):
        self.cpu_side = bus.master

    def connectMemSideBus(self, bus):
        self.mem_side = bus.slave

