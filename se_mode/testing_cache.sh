#!/bin/bash

cd 
cd gem5

<< L1
echo "Testing L1 Cache #1"
build/X86/gem5.opt configs/example/se.py --cmd=tests/test-progs/hello/bin/x86/linux/hello \
--l1d_size=64kB \
--l1i_size=16kB \
--caches

$'\r'

echo "Testing L1 Cache (Double capacity) #2"
build/X86/gem5.opt configs/example/se.py --cmd=tests/test-progs/hello/bin/x86/linux/hello \
--l1d_size=128kB \
--l1i_size=32kB \
--caches
L1

<< note
Based on the results doubling the L1 Cache size does not affect run time.
This might be due to the file that I ran (hello world) which might be too 
small to use for research.
note

# << L2
echo "Testing L2"
build/X86/gem5.opt configs/example/se.py --cmd=tests/test-progs/hello/bin/x86/linux/hello \
--l2_size=16kB \
--l2cache
# L2