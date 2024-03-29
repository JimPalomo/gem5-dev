'''
Creating a new SimObject
    Step 1: Create a Python class
    Step 2: Implement the C++
    Step 3: Register the SimObject and C++ file
    Step 4: (Re-)build gem5
    Step 5: Create a config script
'''

from m5.params import *
from m5.SimObject import SimObject

class Hello(SimObject):
    type = 'Hello'
    cxx_header = "tutorial/hello.hh"

    time_to_wait = Param.Latenc("Time before firing the even")
    number_of_fires = Param.Int(1, "Number of times to fire the event before goodbye")
