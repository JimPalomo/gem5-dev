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

class HelloObject(SimObject):
    type = 'HelloObject'
    cxx_header = "learning_gem5/tutorial/hello_object.hh"

