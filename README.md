# Developing gem5
gem5 is an open simulation platform for computer-system architecture research. gem5 consists of replicating systems through emulated simulation and full system simulation. gem5 is used to study the run times of simulated hardware and programs.

> Files are broken down into parts which covers the following: 
> <br> Part 1: Creating a simple configuration script and adding cache objects to the config script.
> <br> Part 2: Creating a SimObject, event-driven programming, adding-parameters, creating memory objects, creating cache objects.
> <br> Part 5: Full System Simulation, create a full system (fs mode) which relays back more accurate run times than System Emulation (se mode). FE mode takes longer to establish than that of SE mode when it comes to processing thousands to millions of requests (can take hours to days to setup). Every part before part 5 is done under SE mode. <br><br>
> Notes: Part 3 (Modeling cache coherence with Ruby) & 4 (CPU Models) are more in depth learning modules that were skipped due to time constraints. 

<br>

## How to run my files

1. Install gem5 with git (git clone https://gem5.googlesource.com/public/gem5) and follow instructions @ http://learning.gem5.org/book/part1/building.html
2. Add a tutorial folder under gem5/src file and add which files you would like to test
3. Open the terminal and cd into gem5
4. Update gem5 (scons build/X86/gem5.opt)
5. Run the system emulation or full system emulation by starting with build/x86/gem5.opt [debug flags] [file directory]<br>
	Ex. build/X86/gem5.opt --debug-flags=CacheDebug src/tutorial/run_cache.py

> easy install by running install.sh shell script
> 1. Authorize shell script install.sh by `chmod 755 install.sh` and run by `./install.sh`

## Architecture Diagrams
To add the architecture diagrams you must have pip installed and install pydot -> `pip install pydot`. 
<br>
Note: diagrams are created under the m5out file after a scons rebuild -> `scons build/X86/gem5.opt` (X86 for 64 bit architecture, different for each computer). 

## Notes
Must scons rebuild `scons build.X86/gem5.opt` after creating new configuration files.

## Common Issues
When running gem5, scons rebuild can not figure out specified members. This is due to a newer compiler version and an older version of gem5. Basically, the two are out of sync. https://gem5-users.gem5.narkive.com/lHPinJyI/error-isafakeparams-has-no-member-named-fake-mem
 
