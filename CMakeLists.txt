cmake_minimum_required(VERSION 3.10)
project(Mutex C)

set(CMAKE_C_STANDARD 99)
set(SOURCE_FILES mutex.c)
set(SYLVANHOME ${PROJECT_SOURCE_DIR}/sylvan-1.2.0/src/)

add_executable(Mutex mutex.c)
target_link_libraries(Mutex sylvan)
