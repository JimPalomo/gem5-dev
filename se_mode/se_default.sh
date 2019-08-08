#!/bin/bash

cd
cd gem5

# Running hello world program without parameters
build/X86/gem5.opt configs/example/se.py --cmd=tests/test-progs/hello/bin/x86/linux/hello
