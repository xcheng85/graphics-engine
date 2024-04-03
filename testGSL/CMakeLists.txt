project(testGSL VERSION 0.0.1)

include(FetchContent)
FetchContent_Declare(GSL
    GIT_REPOSITORY "https://github.com/microsoft/GSL"
    GIT_TAG "v4.0.0"
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(GSL)
# get all cpp-files
file(GLOB_RECURSE SRC_FILES src/*.cpp CMAKE_CONFIGURE_DEPENDS)
add_executable(${PROJECT_NAME} ${SRC_FILES})
# 
target_link_libraries(${PROJECT_NAME} PRIVATE profiler tcmalloc Microsoft.GSL::GSL)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

