project(engine VERSION 0.0.1)

file(GLOB_RECURSE SRC_FILES src/*.cpp CMAKE_CONFIGURE_DEPENDS)
add_library(${PROJECT_NAME} ${SRC_FILES})

add_definitions(-DGE_EXPORT)
add_definitions(-D_DEBUG)
add_definitions(-DSDL2_WINDOW)
add_definitions(-DVULKAN_DEBUG_REPORT)


# set(BOOST_INCLUDE_LIBRARIES thread filesystem system program_options)
# set(BOOST_ENABLE_CMAKE ON)

# include(FetchContent)
# FetchContent_Declare(
#   Boost
#   GIT_REPOSITORY https://github.com/boostorg/boost.git
#   GIT_TAG boost-1.84.0
#   GIT_SHALLOW TRUE
# )
# FetchContent_MakeAvailable(Boost)

# linux
if (UNIX)
    # search libs
    find_package(Vulkan REQUIRED)
    find_package(glfw3 3.3 REQUIRED)
    find_package(glm 1.0.1 REQUIRED)
    target_link_libraries(${PROJECT_NAME} glad tlsf)   


    find_package(SDL2 REQUIRED)
    find_package(Boost 1.74.0 COMPONENTS program_options REQUIRED)

    target_link_libraries(${PROJECT_NAME} Boost::program_options)

    # find_package(yaml-cpp REQUIRED)
    # find_package(SDL2_ttf REQUIRED)
    # find_package(SDL2_mixer REQUIRED)
    # find_package(SDL2_image REQUIRED)
    # # include headers
    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
    target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})

    # target_include_directories(${PROJECT_NAME} PUBLIC ${YAML_CPP_INCLUDE_DIR})
    # # link lib files
    # target_link_libraries(${PROJECT_NAME} ${YAML_CPP_LIBRARIES})    
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${Vulkan_LIBRARIES})    
    # target_link_libraries(${PROJECT_NAME} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES})    
endif (UNIX)

# precompile headers
target_precompile_headers(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h)

# add include directory
include_directories(${CMAKE_SOURCE_DIR}/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/external/tlsf/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
