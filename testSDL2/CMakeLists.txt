project(testSDL2 VERSION 0.0.1)

# get all cpp-files
file(GLOB_RECURSE SRC_FILES *.cpp CMAKE_CONFIGURE_DEPENDS)
add_executable(${PROJECT_NAME} ${SRC_FILES})
# 
target_link_libraries(${PROJECT_NAME} engine profiler tcmalloc lua)
# target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
# include_directories(${CMAKE_SOURCE_DIR}/external/sdl/include)
