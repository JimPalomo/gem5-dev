import sys

import m5
from m5.objects import *

sys.path.append('configs/common/')
import SimpleOpts

from system import MySystem

SimpleOpts.add_option("--script", default='', help="Script to execute in the simulated system")

if __name__ == "__m5_main__":
    (opts, args) = SimpleOpts.parse_args()

    system = MySystem(opts)

    system.readfile = opts.script

    root = Root(full_system = True, system = system)

    m5.instantiate()

    print("Running the simulation")
    exit_event = m5.simulate()
    print('Exiting @ tick %i because %s' % (m5.curTick(), exit_event.getCause()))              