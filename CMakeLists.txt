cmake_minimum_required(VERSION 3.29)

project(ge)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DRELEASE)
endif()

if(UNIX)
    add_definitions(-DPOSIX)
elseif(WIN32)
    add_definitions(-DWIN32)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -pedantic -lprofiler -g -fcoroutines -fmodules-ts")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -fmodules-ts")
    #add_definitions(-DUSING_RANGE_V3)
    #include_directories("/home/xcheng85/range-v3/include")
    # option to set toolchain to different gcc:
    # set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--gcc-toolchain=/home/xcheng85/gcc/bin/")
  # using Clang
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  add_definitions(-D_USE_MATH_DEFINES)
  include_directories(${PROJECT_SOURCE_DIR})
endif()


# set(CMAKE_CXX_FLAGS "-Wall -pedantic -lprofiler -g -O3")
# -g -00 for the memcheck valgrind
# set(CMAKE_CXX_FLAGS "-Wall -pedantic -lprofiler -g")

# # for c++ 20 modules
# function(denote_module_interface)
#     if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#         foreach(f ${ARGV})
#             set_source_files_properties(${f} PROPERTIES COMPILE_FLAGS /interface)
#         endforeach()
#     endif()
# endfunction()



# output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# # third-party
add_subdirectory(glad)
add_subdirectory(external/lua)
set( SDL_LIBC ON CACHE BOOL "" FORCE )
set( SDL_TEST OFF CACHE BOOL "" FORCE )
add_subdirectory(external/sdl)
add_subdirectory(external/tlsf)

# add engine and game
#add_subdirectory(engine)
#add_subdirectory(app)
add_subdirectory(tests)
add_subdirectory(testMicroBenchmark)
add_subdirectory(profile_0)
add_subdirectory(testMemory)
add_subdirectory(testLua)
add_subdirectory(testGSL)
add_subdirectory(testSDL2)
add_subdirectory(testOpenGL)
add_subdirectory(testVulkan)
add_subdirectory(volk)
add_subdirectory(VulkanMemoryAllocator)
enable_testing()