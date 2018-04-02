cmake_minimum_required (VERSION 3.5)
project (OSLab4 C)

# Add the executable, -name of exe -name of files
find_package (Threads)
add_executable (hostd src/hostd.c src/queue.c src/utility.c)
add_executable (process src/sigtrap.c)
target_link_libraries (hostd ${CMAKE_THREAD_LIBS_INIT})

target_compile_options(hostd PUBLIC "-std=gnu99")