cmake_minimum_required(VERSION 3.20)
project(DS-MandatoryExercise
        VERSION 2.0
        DESCRIPTION "Mandatory Exercise with sockets."
        LANGUAGES C)

set(CMAKE_C_STANDARD 11)

# create keys dynamic library
add_library(keys SHARED)
target_sources(keys PRIVATE src/keys.c)
target_link_libraries(keys PRIVATE rt)
# using PUBLIC propagates this directory to client target, which needs it to include utils.h & keys.h
target_include_directories(keys PUBLIC include)

# create dbms library
add_library(dbms STATIC)
target_sources(dbms
        PRIVATE src/dbms/dbms.c
                src/dbms/dbmsUtils.c
                src/utils.c)
# using PUBLIC propagates these directories to server and dbms_tests targets
# which need it to include utils.h,  dbms.h & dbmsUtils.h
target_include_directories(dbms
        PUBLIC  include
                include/dbms)

# client and server apps
add_executable(client)
target_sources(client
        PRIVATE app/client.c
                src/utils.c)
target_link_libraries(client PRIVATE keys)

add_executable(server)
target_sources(server PRIVATE app/server.c)
target_link_libraries(server
        PRIVATE rt
                pthread
                dbms)

# tests
add_executable(dbms_tests)
target_sources(dbms_tests PRIVATE test/dbms_tests.c)
target_link_libraries(dbms_tests PRIVATE dbms)
