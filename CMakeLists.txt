cmake_minimum_required(VERSION 3.10)
project(turing_machine C)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_C_STANDARD 99)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_executable(turing_machine ${SOURCES})
