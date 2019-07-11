# Creating a simple.py configuration

# Importing necessary libraries
import m5 
from m5.objects import *
from caches import *
from optparse import OptionParser

parser = OptionParser()
parser.add_option('--l1i_size', help="L1 instruction cache size")
parser.add_option('--l1d_size', help="L1 data cache size")
parser.add_option('--l2_size', help="Unified L2 cache size")

(options, args) = parser.parse_args()

# Creating a system simObject
system = System()

# Setting the clock on the system: create clock domain, set clock freq, and voltage domain (default)
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '1GHz'
system.clk_domain.voltage_domain = VoltageDomain()

# Setting up timing mode for memory simulation
system.mem_mode = 'timing'
system.mem_ranges = [AddrRange('512MB')]

# Creating a CPU (simple timing-based - single clock cycle)
system.cpu = TimingSimpleCPU()

# Adding cache import (Updated from simple.py)
system.cpu.icache = L1ICache(options)
system.cpu.dcache = L1DCache(options)

system.cpu.icache.connectCPU(system.cpu)
system.cpu.dcache.connectCPU(system.cpu)

# Creating L2 bus to connect caches L1 to L2
system.l2bus = L2XBar()

system.cpu.icache.connectBus(system.l2bus)
system.cpu.dcache.connectBus(system.l2bus)

# Creating L2 cache and connect it to L2 bus & memory bus
system.l2cache = L2Cache(options)
system.l2cache.connectCPUSideBus(system.l2bus)

# Creating system-wide memory bus (send channeling data)
system.membus = SystemXBar()

system.l2cache.connectMemSideBus(system.membus)

# Creating an I/O controller on the CPU and connect it to membus
system.cpu.createInterruptController()
system.cpu.interrupts[0].pio = system.membus.master
system.cpu.interrupts[0].int_master = system.membus.slave
system.cpu.interrupts[0].int_slave = system.membus.master

system.system_port = system.membus.slave

# Creating a memory controller
system.mem_ctrl = DDR3_1600_8x8()
system.mem_ctrl.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.master

# Setting up the process SimObject
process = Process()
process.cmd = ['tests/test-progs/hello/bin/x86/linux/hello']
system.cpu.workload = process
system.cpu.createThreads()

# Creating root object to transfer python simobjects to c++
root = Root(full_system = False, system = system)
m5.instantiate()

# Starting simulation message
print("Beginning simulation!")
exit_event = m5.simulate()

# Ending simulation message
print('Exiting @ tick {} because {}'.format(m5.curTick(), exit_event.getCause()))
