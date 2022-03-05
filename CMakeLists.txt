cmake_minimum_required(VERSION 3.21)
project(MandatoryExercise1)

set(CMAKE_C_STANDARD 11)

include_directories(MandatoryExercises/Exercise1)

add_executable(client MandatoryExercises/Exercise1/client.c)
add_executable(server MandatoryExercises/Exercise1/server.c)