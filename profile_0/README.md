# Profile (code without mannual instrumentation)
1. instrumentation
2. sample-based: polls the program regularlys, not accurate

## Perf
sample-based tool
in linux kernel

```shell
cd ./build/bin/
# sudo is needed
sudo perf stat ./profile_0
# select counter

#797,517      cache-misses                     #   42.26% of all cache refs 
sudo perf stat -e cache-references,cache-misses ./profile_0

# debug + optimized (close to production)
-g -O3

sudo perf record ./profile_0

# enter, and q
sudo perf report

```

## GperfTools
1. hardware performance counter
2. linking-time instrumentation

```shell
git clone https://github.com/gperftools/gperftools
cd gperftools
./autogen.sh
./configure
make
sudo make install
sudo apt install google-perftools

# To install the CPU profiler into your executable, add -lprofiler to the link-time step for your executable. -lprofiler

sudo find / -name libprofiler.so
cd ./build/bin/
LD_PRELOAD=/usr/local/lib/libprofiler.so CPUPROFILE=./test.prof CPUPROFILE_FREQUENCY=1000 ./profile_0
# interactive shell mode
google-pprof ./profile_0 ./test.prof
# shows you the line of code
text --lines 
#16  88.9%  88.9%       16  88.9% SortingRequest::compare (inline) /home/xiao/github.com/xcheng85/graphics-engine/profile_0/src/SortingRequest.h:61

# web browser
pprof --web ./profile_0 ./test.prof
# pdf (call graph)
google-pprof --pdf ./profile_0 ./test.prof > output.pdf

# control profiling with os signal
LD_PRELOAD=/usr/local/lib/libprofiler.so CPUPROFILE=./test.prof CPUPROFILE_FREQUENCY=1000  CPUPROFILESIGNAL=12 ./profile_0
# start profile
killall -12 ./profile_0
# end profile
killall -12 ./profile_0
```

## Software timer (instrumentation)
chrono: wall-clock time, cannot measure cpu time
cpu time: linux os api, clock_gettime()

timer for app
timer for process
timer for calling threads


## Task Schedule

queue for workers
queue for requests

mutex

conditional_var 
unique_lock
polling mechanism 


1. Worker thread
2. Scheduler(dispatcher)
3. Main thread publish request(event) to scheduler



