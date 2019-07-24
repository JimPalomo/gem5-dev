# gem5-dev

gem5 is an open simulation platform for computer-system architecture research. gem5 consists of replicating systems through emulated simulation and full system simulation. gem5 is used to study the run times of simulated hardware and programs.

<br>

How to run my files

1. Install gem5 with git (git clone https://gem5.googlesource.com/public/gem5) and follow instructions @ http://learning.gem5.org/book/part1/building.html
2. Add a tutorial folder under gem5/src file and add which files you would like to test
3. Open the terminal and cd into gem5
4. Update gem5 (scons build/X86/gem5.opt)
5. Run the system emulation or full system emulation by starting with build/x86/gem5.opt [debug flags] [file directory]<br>
	Ex. build/X86/gem5.opt --debug-flags=CacheDebug src/tutorial/run_cache.py

