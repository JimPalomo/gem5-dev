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
        # if not options or not options.l1i_size:
        #     return
        # self.size = options.l1i_size

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
        # if not options or not options.l1d_size:
        #     return
        # self.size = options.l1d_size

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
    def __init__(self, options = None):
        super(L2Cache, self).__init__()
        if not options or not options.l2_size:
            return
        self.size = options.l2_size

    # Adding connections
    def connectCPUSideBus(self, bus):
        self.cpu_side = bus.master

    def connectMemSideBus(self, bus):
        self.mem_side = bus.slave

