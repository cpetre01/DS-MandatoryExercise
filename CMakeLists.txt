cmake_minimum_required(VERSION 3.00)
project(DS-MandatoryExercise1)

set(CMAKE_C_STANDARD 11)

# set up names for executables, .a & .so binaries
set(CLIENT_BIN_NAME client)
set(SERVER_BIN_NAME server)
set(KEYS_BIN_NAME keys)
set(DBMS_BIN_NAME dbms)
set(DB_TESTS_BIN_NAME db_tests)

# set up sources for all components
set(CLIENT_SRC apps/client.c src/utils.c)
set(SERVER_SRC apps/server.c)
set(KEYS_SRC src/keys.c)
set(DBMS_SRC src/dbms.c src/dbmsUtils.c src/utils.c)
set(DB_TESTS_SRC tests/db_tests.c)

# set up required library links
set(CLIENT_LIBS ${KEYS_BIN_NAME})
set(SERVER_LIBS rt pthread ${DBMS_BIN_NAME})
set(KEYS_LIBS rt)
set(DB_TESTS_LIBS ${DBMS_BIN_NAME})

include_directories(.)

# create keys dynamic library
add_library(${KEYS_BIN_NAME} SHARED ${KEYS_SRC})
target_link_libraries(${KEYS_BIN_NAME} PUBLIC ${KEYS_LIBS})

# create dbms library
add_library(${DBMS_BIN_NAME} STATIC ${DBMS_SRC})

# client and server apps
add_executable(${CLIENT_BIN_NAME} ${CLIENT_SRC})
target_link_libraries(${CLIENT_BIN_NAME} PUBLIC ${CLIENT_LIBS})

add_executable(${SERVER_BIN_NAME} ${SERVER_SRC})
target_link_libraries(${SERVER_BIN_NAME} PUBLIC ${SERVER_LIBS})

# tests
add_executable(${DB_TESTS_BIN_NAME} ${DB_TESTS_SRC})
target_link_libraries(${DB_TESTS_BIN_NAME} PUBLIC ${DB_TESTS_LIBS})
