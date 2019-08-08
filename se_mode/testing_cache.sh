#!/bin/bash

cd 
cd gem5

echo "Running hello world #1"
build/X86/gem5.opt configs/example/se.py --cmd=tests/test-progs/hello/bin/x86/linux/hello \
--l1d_size=64kB \
--l1i_size=16kB \
--caches

$'\r'

echo "Running hello world #2"
build/X86/gem5.opt configs/example/se.py --cmd=tests/test-progs/hello/bin/x86/linux/hello \
--l1d_size=128kB \
--l1i_size=32kB \
--caches

<< Comment
Based on the results doubling the L1 Cache size does not affect run time.
This might be due to the file that I ran (hello world) which might be too 
small to use for research.
Comment