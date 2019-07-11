# Creating a simple.py configuration

# Importing necessary libraries
import m5 
from m5.objects import *

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

# Creating system-wide memory bus (send channeling data)
system.membus = SystemXBar()

# No caches for this system therefore connect i & d cache directly to bus
system.cpu.icache_port = system.membus.slave
system.cpu.dcache_port = system.membus.slave

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
print('Exiting @ tick {} because {}'
      .format(m5.curTick(), exit_event.getCause()))
