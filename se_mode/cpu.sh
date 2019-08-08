#!/bin/bash

cd 
cd gem5

<< listOfCPU
echo "Listing CPU types"
build/X86/gem5.opt configs/example/se.py  \
--list-cpu-types
listOfCPU

<< atomic
echo "Testing AtomicSimpleCPU"
build/X86/gem5.opt configs/example/se.py --cmd=tests/test-progs/hello/bin/x86/linux/hello \
--cpu-type=AtomicSimpleCPU
atomic

<< noncache
# echo "Testing NonCachingSimpleCPU"
echo "Testing NonCachingSimpleCPU"
build/X86/gem5.opt configs/example/se.py --cmd=tests/test-progs/hello/bin/x86/linux/hello \
--cpu-type=NonCachingSimpleCPU
noncache

<< X86K
# echo "Testing X86KvmCPU"
echo "Testing X86KvmCPU"
build/X86/gem5.opt configs/example/se.py --cmd=tests/test-progs/hello/bin/x86/linux/hello \
--cpu-type=X86KvmCPU
X86K

<< Trace
# echo "Testing TraceCPU"
echo "Testing TraceCPU"
build/X86/gem5.opt configs/example/se.py --cmd=tests/test-progs/hello/bin/x86/linux/hello \
--cpu-type=TraceCPU
Trace

<< Timing
# echo "Testing TimingSimpleCPU"
echo "Testing TimingSimpleCPU"
build/X86/gem5.opt configs/example/se.py --cmd=tests/test-progs/hello/bin/x86/linux/hello \
--cpu-type=TimingSimpleCPU
Timing