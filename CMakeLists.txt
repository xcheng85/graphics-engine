cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS "-Wall -pedantic -lprofiler -g -O3")
set(CMAKE_CXX_FLAGS "-Wall -pedantic -lprofiler -g")

project(ge)
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
# add_subdirectory(engine)
# add_subdirectory(app)
add_subdirectory(tests)
add_subdirectory(testMicroBenchmark)
add_subdirectory(profile_0)
add_subdirectory(testMemory)
add_subdirectory(testLua)
add_subdirectory(testGSL)
# add_subdirectory(testSDL2)
add_subdirectory(testOpenGL)

enable_testing()