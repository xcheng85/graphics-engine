project(testSDL2 VERSION 0.0.1)

find_package(SDL2 REQUIRED)

# get all cpp-files
file(GLOB_RECURSE SRC_FILES *.cpp CMAKE_CONFIGURE_DEPENDS)
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} profiler tcmalloc lua ${SDL2_LIBRARIES})