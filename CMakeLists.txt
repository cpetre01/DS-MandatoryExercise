cmake_minimum_required(VERSION 3.21)
project(MandatoryExercise1)

set(CMAKE_C_STANDARD 11)

include_directories(.)

# create dynamic library
add_library(keys SHARED src/keys.c)

# create executables for client and server
add_executable(client src/client.c)
add_executable(server src/server.c)

# link required libraries
target_link_libraries(client rt)
target_link_libraries(server rt pthread)
