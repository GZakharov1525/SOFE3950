cmake_minimum_required (VERSION 3.5)
project (OSLab3 C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c++11 -pthread")

# Add the executable, -name of exe -name of files
find_package (Threads)
add_executable (OSLab3 src/sudokuValidator.c)
target_link_libraries (OSLab3 ${CMAKE_THREAD_LIBS_INIT})