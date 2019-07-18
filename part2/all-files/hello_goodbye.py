# Name changed from run_hello.py to hello_goodbye.py

import m5
from m5.objects import *

root = Root(full_system = False)

# root.hello = Hello()

root.hello = Hello(time_to_wait = '2us', number_of_fires = 5)
root.hello.goodbye = Goodbye(buffer_size = '100B')


m5.instantiate()

print("Beginning simulation!")
exit_event = m5.simulate()
print('Exiting @ tick %i because %s' % (m5.curTick(), exit_event.getCause()))

