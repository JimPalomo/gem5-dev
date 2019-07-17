from m5.params import *
from m5.objects.MemObject import MemObject

class Memobj(MemObject):
    type = 'Memobj'
    cxx_header = "tutorial/memobj.hh"

    inst_port = SlavePort("CPU side port, receives requests")
    data_port = SlavePort("CPU side port, receives requests")
    mem_side = MasterPort("Memory side port, sends requests")

