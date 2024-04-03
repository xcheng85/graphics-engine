project(tlsf VERSION 3.1.0)
add_library(tlsf include/tlsf.h src/tlsf.c)
target_include_directories(tlsf PUBLIC include)