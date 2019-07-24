import m5
from m5.objects import *
from m5.util import convert

import x86

from caches import *

class MySystem(LinuxX86System):

    def __init__(self, opts):
        super(MySystem, self).__init__()
        self._opts = opts

        self.clk_domain = SrcClockDomain()
        self.clk_domain.clock = '3GHz'
        self.clk_domain.voltage_domain = VoltageDomain()

        mem_size = '512MB'
        self.mem_ranges = [AddrRange(mem_size), AddrRange(0xC0000000, size=0x100000), ]

        self.membus = SystemXBar()
        self.membus.badaddr_responder = BadAddr()
        self.membus.default = self.membus.badaddr_responder.pio

        self.system_port = self.membus.slave

        x86.init_fs(self, self.membus)

        # self.kernel = 'binaries/x86_64-vmlinux-2.6.22.9'
        self.kernel = 'src/x86/binaries/x86_64-vmlinux-2.6.22.9'


        boot_options = ['earlyprintk=ttyS0', 'console=ttyS0', 'lpj=7999923',
                        'root=/dev/hda1']
        self.boot_osflags = ' '.join(boot_options)

        # self.setDiskImage('disks/linux-x86.img')
        self.setDiskImage('src/x86/disks/linux-x86.img')

        self.createCPU()

        self.createCacheHierarchy()

        self.createMemoryControllers()

        self.setupInterrupts()

    def createCPU(self):
        self.cpu = AtomicSimpleCPU()
        self.mem_mode = 'atomic'
        self.cpu.createThreads()

    def setDiskImage(self, img_path):
        disk0 = CowDisk(img_path)
        self.pc.south_bridge.ide.disks = [disk0]

    def createCacheHierarchy(self):
        self.cpu.icache = L1ICache(self._opts)
        self.cpu.dcache = L1DCache(self._opts)

        self.cpu.icache.connectCPU(self.cpu)
        self.cpu.dcache.connectCPU(self.cpu)

        self.cpu.icache.connectBus(self.membus)
        self.cpu.dcache.connectBus(self.membus)

        self.cpu.itb.walker.port = self.membus.slave
        self.cpu.dtb.walker.port = self.membus.slave

    def createMemoryControllers(self):
        self.mem_cntrl = DDR3_1600_8x8(range = self.mem_ranges[0], port = self.membus.master)

    def setupInterrupts(self):

        self.cpu.createInterruptController()

        self.cpu.interrupts[0].pio = self.membus.master
        self.cpu.interrupts[0].int_master = self.membus.slave
        self.cpu.interrupts[0].int_slave = self.membus.master

class CowDisk(IdeDisk):

    def __init__(self, filename):

        super(CowDisk, self).__init__()
        self.driveID = 'master'
        self.image = CowDiskImage(child=RawDiskImage(read_only=True), read_only=False)
        self.image.child.image_file = filename
