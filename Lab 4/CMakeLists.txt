cmake_minimum_required (VERSION 3.5)
project (OSLab4 C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c11 -pthread")

# Add the executable, -name of exe -name of files
find_package (Threads)
add_executable (hostd src/hostd.c src/queue.c src/sigtrap.c src/utility.c)
target_link_libraries (hostd ${CMAKE_THREAD_LIBS_INIT})