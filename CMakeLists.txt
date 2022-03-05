cmake_minimum_required(VERSION 3.21)
project(MandatoryExercise1)

set(CMAKE_C_STANDARD 11)

include_directories(MandatoryExercise1)

add_executable(client src/client.c)
add_executable(server src/server.c)