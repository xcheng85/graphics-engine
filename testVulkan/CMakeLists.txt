project(testSDLVulkan VERSION 0.0.1)

find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glslang CONFIG REQUIRED)

# use subproject approach
# find_package(VulkanMemoryAllocator CONFIG REQUIRED)
# get all cpp-files
file(GLOB_RECURSE SRC_FILES *.cpp CMAKE_CONFIGURE_DEPENDS)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS} ${Vulkan_INCLUDE_DIRS} )
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} profiler tcmalloc lua ${SDL2_LIBRARIES} ${Vulkan_LIBRARIES} 
volk_headers GPUOpen::VulkanMemoryAllocator  glslang::glslang glslang::SPIRV glslang::glslang-default-resource-limits)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders
                ${CMAKE_BINARY_DIR}/bin/testVulkan/shaders
        # COMMAND ${CMAKE_COMMAND} -E copy_directory
        #         ${CMAKE_CURRENT_SOURCE_DIR}/assets/textures
        #         ${CMAKE_BINARY_DIR}/bin/testVulkan/textures
        # COMMAND ${CMAKE_COMMAND} -E copy_directory
        #         ${CMAKE_CURRENT_SOURCE_DIR}/assets/gltf
        #         ${CMAKE_BINARY_DIR}/bin/testVulkan/gltf
        COMMENT "Copying to ${CMAKE_BINARY_DIR}"
)