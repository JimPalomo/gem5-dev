# gem5-dev

Work done in gem5
> Files are broken down into parts which covers the following: 
> 1. Part 1: Creating a simple configuration script and adding cache objects to the config script.
> 2. Part 2: Creating a SimObject, event-driven programming, adding-parameters, creating memory objects, creating cache objects.
> 3. Part 5: Full System Simulation, create a full system (fs mode) which relays back more accurate run times than System Emulation (se mode). FE mode takes longer to establish than that of SE mode when it comes to processing thousands to millions of requests (can take hours to days to setup).
> Note: Part 3 (Modeling cache coherence with Ruby) & 4 (CPU Models) are more in depth learning modules that were skipped due to time constraints.

<br>

## How to run my files

1. Install gem5 with git (git clone https://gem5.googlesource.com/public/gem5) and follow instructions @ http://learning.gem5.org/book/part1/building.html
2. Add a tutorial folder under gem5/src file and add which files you would like to test
3. Open the terminal and cd into gem5
4. Update gem5 (scons build/X86/gem5.opt)
5. Run the system emulation or full system emulation by starting with build/x86/gem5.opt [debug flags] [file directory]<br>
	Ex. build/X86/gem5.opt --debug-flags=CacheDebug src/tutorial/run_cache.py

## Still creating
Going to create shell scripts to easily run and install gem5