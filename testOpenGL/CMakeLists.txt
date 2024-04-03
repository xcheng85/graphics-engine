project(testOpenGL VERSION 0.0.1)

add_definitions(-DGE_EXPORT)
add_definitions(-D_DEBUG)

find_package(glfw3 3.3 REQUIRED)

file(GLOB_RECURSE SRC_FILES *.cpp CMAKE_CONFIGURE_DEPENDS)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} glfw3 glad profiler tcmalloc)
include_directories(${CMAKE_SOURCE_DIR}/glad/include)

set(copy_source_dir "${CMAKE_SOURCE_DIR}/data/")
set(copy_dest_dir "${CMAKE_BINARY_DIR}/Build/bin")

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders
                ${CMAKE_BINARY_DIR}/bin/shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/assets/textures
                ${CMAKE_BINARY_DIR}/bin/textures
        COMMENT "Copying to ${CMAKE_BINARY_DIR}"
)