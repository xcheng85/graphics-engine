# graphics-engine

## concept
data-driven arch
indoor: BSP, portal
c++ lib: boost, folly, loki
phys, ode, bullet
bsp, quadtree,octree,kd tree, sphere hierachy
visual effects: full-screen post effects. 
partical, ---> low-level renderer --> post-effect
built custom profilling and debugging tools
in-game debugging facility: debug drawing, record events and play back. 
how to calculate memory usage
dump memory usage, leakage stats
core dumps

## Software object model
1. how object is identified
2. lifetime of object
3. how object referecn each other

## event-driven arch
1. queue for the events for each frame 
2. event handler of receiver

## C++ compiler and linker, cross-platform concern
preprocesser 
translation unit
object files
libraries

build options: warning, optimization, debugging info

config inline expand 

runtime-assertion vs compile-time

Big-endian and little-endian

tools: endian-swap, integer, float and struct, class
endian-swap float is the same as int

forward declaration: implicit "extern"

internal linkage vs external linkage ? 
internal linkage with "static"

external linkage: be careful of multiple defined symbol, compiler confusion

## exec from os perspective
1. initialized global/static var
2. uninitialized global/static var
3. const global/static var
4. consume memory for program stack
5. os maintains block of memory for heap alloc
6. overload new//delete operator to change behavior for memory allocation

## custom memory allocator for memory-intensive application, Todo
very useful in ECS system: Entity Component System

custom allocators is control and monitoring memory usage (key)
benchmark performance boost

1. memory pool based: linear
2. free-list 

### vector 
heap allocation
fragmentation

### memory allocator polymorphism

1. static polymorphisim: template, c++14

2. classic polymorphisum: pmr, memory_resource c++17
<memory_resource>

### default allocator

### linear memory allocator


### list-based memory allocatior


### stack based memory allocator 

### c++17 polymorphic allocator
header
namespace
pmr::memory_resource 


extra layer between container and custom memory allocator

1. null_memory_resource: singleton
2. new_delete_resource: singleton
3. monotonic_buffer_resource: multiple instance
    holds pointer of big buffer pre-allocated
    when buffer is used up, 

    How to release ? 

    but doesn’t release it with deallocation. It can only grow.


4. unsynchronized_pool_resource: not thread-safe 
pools of different sizes. Each pool is a set of chunks that are divided into blocks of uniform size.

5. sync
6. custom implementation

mentioning that pool resources (including monotonic_buffer_resource) can be chained. If there’s no available memory in a pool, the allocator will allocate from the “upstream” resource.





## memory layout for struct 
#pragma pack() 
order of fields matters
padding: explict for better documentation

### memory layout for virtual class
extra 4/8 bytes for vtable

### High memory usage
thrashing: between memory pages and disk-based pages

### memory alignment


## Tools: 
web-based ui
renderer --> debug payload --> redis stream 
ui to check the debugger info


## Concurrent 
task: web server listening, web client reading, each frame update to vram
data: calculating matrix, add_compile_options

## Process and threads (kernal space)
boost user-space thread

pid
permission
parent process
virtual memory space
env var
open file handle
pwd
ipc mech: message queue, pipes, semaphores
each process has >= threads

two process can share pages

threads: 
thread id: 
call stack
local storage
thread yield: give up the remaining timeslice


thread waiting patterns: 

1. busy wait: while loop polling
2. block: kernal wake up, conditional_var, fopen, open socket(blocking mode), sync i/o, .join, sleep, 
3. poll + yield, kernal call, expensive

context switching between threads in one program and threads from two probrams

thread priority
thread affinity


## fiber (user space)
provide by os api (windows)

## c++ 20 courotines
user-space thread, web server gaimes

## IPC communication
1. pub-sub
2. shared memory mapping

## Difference between mutex and semaphore (c++ 20)
mutex : one thread

semaphore: any thread: group share, resource pool, N threads maximum
semaphore implements pub-sub two thredas 

## Lock-free algorithm


## submodule
```shell
git submodule add https://github.com/glfw/glfw.git external/glfw
git submodule add https://github.com/google/benchmark.git external/benchmark
git submodule add https://github.com/lua/lua.git external/lua
git submodule add https://github.com/libsdl-org/SDL.git external/sdl
git submodule add https://github.com/nlohmann/json.git external/json

git submodule add https://github.com/zeux/volk.git volk
git submodule add https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git VulkanMemoryAllocator
cmake -S . -B build
# Since VMA has no source files, you can skip to installation immediately
cmake --install build --prefix build/install
git checkout v3.11.3
```

## sdl2
```shell
sudo apt-cache search libsdl2

# 
sudo apt-get install libsdl2-dev 
sudo apt-get install libsdl2-image-dev 
sudo apt-get install libsdl2-mixer-dev 
sudo apt-get install libsdl2-ttf-dev 

# header
/usr/include/SDL2
/usr/lib/x86_64_

# check .cmake config files
# automatically crearted
/usr/lib/x86_64-linux-gnu/cmake/SDL2/sdl2-config.cmake
# include dir is automatically created
set(SDL2_INCLUDE_DIRS "${includedir}/SDL2")
# static link
set(SDL2_LIBRARIES "-L${SDL2_LIBDIR}  -lSDL2")

# dependencies for ubuntu22.04
sudo apt-get install build-essential git make \
pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev \
libaudio-dev libjack-dev libsndio-dev libx11-dev libxext-dev \
libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev \
libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev \
libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev fcitx-libs-dev \
libpipewire-0.3-dev libwayland-dev libdecor-0-dev
```

## glfw
```shell
apt install mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev
sudo apt install mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev
sudo apt install libwayland-dev libxkbcommon-dev xorg-dev
git clone https://github.com/glfw/glfw.git
cd glfw/
cmake -S . -B build
cd build/
make
sudo make install
```

## glm
```shell
cd /home/xiao/Downloads/glm-1.0.1

cmake \
    -DGLM_BUILD_TESTS=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -B build .
cmake --build build -- all
sudo cmake --build build -- install

```
## vulkan
```shell
sudo apt update
sudo apt install vulkan-sdk

# or use tarbal to control the version
tar -xvf vulkansdk-linux-x86_64-1.3.280.1.tar.xz
#.bashrc
export VULKAN_SDK=~/vulkan/1.3.280.1/x86_64
export PATH=$VULKAN_SDK/bin:$PATH
export LD_LIBRARY_PATH=$VULKAN_SDK/lib:$LD_LIBRARY_PATH
export VK_LAYER_PATH=$VULKAN_SDK/etc/vulkan/explicit_layer.d
```

## profiling

### 1. nvidia visual profile (COMPUTING)
```shell
sudo apt-get install libcanberra-gtk*
sudo apt install openjdk-8-jdk
nvvp -vm /usr/lib/jvm/java-8-openjdk-amd64/jre/bin/java ./testOpenGL
```

### 2. nvidia nsight graphics
```shell
sh ./NVIDIA_Nsight_Graphics_2024.1.0.24079.run

#xiao@xiao-Alienware-m15-Ryzen-Ed-R5:~/nvidia_nsight_graphics/NVIDIA-Nsight-Graphics-2024.1/host/linux-desktop-nomad-x64$ 

# launch it
# nvidia_nsight_graphics depends on qt and qt depends oin the following
sudo apt-get -y install build-essential openssl libssl-dev libssl1.0 libgl1-mesa-dev libqt5x11extras5 '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev

cd ~/nvidia_nsight_graphics/NVIDIA-Nsight-Graphics-2024.1/host/linux-desktop-nomad-x64
# sudo is needed for nvidia gpu perf counter
sudo ./ngfx-ui

# linking profiler tcmalloc needs to be disabled

# delete "/root/nvidia/NVIDIA-Nsight-Graphics-2024.1"
```

### 3. Heaptrack
```shell
sudo apt install heaptrack
heaptrack ./testOpenGL
heaptrack --analyze "/home/xiao/github.com/xcheng85/graphics-engine/build/bin/heaptrack.testOpenGL.3832308.zst"
heaptrack_gui heaptrack.testOpenGL.2241192.zst 
```

### 4. Valgrind
```shell
sudo apt-get install valgrind
valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all ./testOpenGL 
```
