cmake_minimum_required(VERSION 3.21)
project(DS-MandatoryExercise1)

set(CMAKE_C_STANDARD 11)

include_directories(.)

# create dynamic libraries
add_library(keys SHARED src/keys.c)
add_library(dbms SHARED src/dbms/dbms.c)

# create executables for client and server
add_executable(client apps/client.c)
add_executable(server apps/server.c)
add_executable(test_dir apps/test_dir.c)

# link required libraries
target_link_libraries(client rt keys)
target_link_libraries(server rt pthread)
target_link_libraries(test_dir dbms)
