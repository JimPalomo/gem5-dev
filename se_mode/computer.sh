#!/bin/bash

cd 
cd gem5

build/X86/gem5.opt configs/example/se.py \
--cmd=tests/test-progs/hello/bin/x86/linux/hello \
# --cmd=tests/test-progs/stack-print/bin/x86/linux/stack-print \
--l1d_size=64kB \
--l1i_size=16kB \
--l2_size=16kB \
--l2cache \
--caches \
--cpu-type=AtomicSimpleCPU

