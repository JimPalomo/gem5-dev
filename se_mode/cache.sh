#!/bin/bash

cd
cd gem5

# Running hello world program with cache
build/X86/gem5.opt configs/example/se.py --cmd=tests/test-progs/hello/bin/x86/linux/hello --cpu-type=TimingSimpleCPU --l1d_size=64kB --l1i_size=16kB --caches

# Make sure that cache is called to incorporate caches (--cache)
