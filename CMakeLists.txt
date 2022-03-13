cmake_minimum_required(VERSION 3.21)
project(DS-MandatoryExercise1)

set(CMAKE_C_STANDARD 11)

include_directories(.)

# create dynamic libraries
add_library(keys SHARED src/keys.c)
add_library(dbms STATIC src/dbms/dbms.c)

# client and server apps

# create executables
add_executable(client apps/client.c)
add_executable(server apps/server.c)

# link required libraries
target_link_libraries(client rt keys)
target_link_libraries(server rt pthread dbms)

# tests
add_executable(db_tests tests/db_tests.c)
target_link_libraries(db_tests dbms)
