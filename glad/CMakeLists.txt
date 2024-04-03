project(glad VERSION 0.0.1)
add_library(glad include/glad/glad.h include/KHR/khrplatform.h src/glad.c)
target_include_directories(glad PUBLIC include)