cmake_minimum_required(VERSION 3.21)
project(DS-MandatoryExercise1)

set(CMAKE_C_STANDARD 11)

include_directories(.)

# create libraries
add_library(utils STATIC src/utils.c)
add_library(keys SHARED src/keys.c)
target_link_libraries(keys PUBLIC rt)

# create dbms library
add_library(dbmsUtils STATIC src/dbmsUtils.c)
add_library(dbms STATIC src/dbms.c)
target_link_libraries(dbms PUBLIC dbmsUtils utils)

# client and server apps

# create executables
add_executable(client apps/client.c)
add_executable(server apps/server.c)

# link required libraries
target_link_libraries(client PUBLIC rt keys utils)
target_link_libraries(server PUBLIC rt pthread dbms)


# tests
add_executable(db_tests tests/db_tests.c)
target_link_libraries(db_tests PUBLIC dbms)
