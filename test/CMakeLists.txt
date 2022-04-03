# tests with GoogleTest

# tests for keys library

set(TARGET_KEYS_TESTS keys_tests)
add_executable(${TARGET_KEYS_TESTS})
target_sources(${TARGET_KEYS_TESTS} PRIVATE keys_tests.cpp)
target_link_libraries(${TARGET_KEYS_TESTS}
        PRIVATE gtest_main
                ${TARGET_KEYS}
        )
add_test(
        NAME ${TARGET_KEYS_TESTS}
        COMMAND ${TARGET_KEYS_TESTS}
)

# tests for server app

set(TARGET_SERVER_TESTS server_tests)
add_executable(${TARGET_SERVER_TESTS})
target_sources(${TARGET_SERVER_TESTS} PRIVATE server_tests.cpp ../app/server.c)
target_link_libraries(${TARGET_SERVER_TESTS}
        PRIVATE gtest_main
                rt
                pthread
                ${TARGET_NET_UTILS}
                ${TARGET_DBMS}
        )
add_test(
        NAME ${TARGET_SERVER_TESTS}
        COMMAND ${TARGET_SERVER_TESTS}
)

# tests for dbms library

set(TARGET_DBMS_TESTS dbms_tests)
add_executable(${TARGET_DBMS_TESTS})
target_sources(${TARGET_DBMS_TESTS} PRIVATE dbms_tests.cpp)
target_link_libraries(${TARGET_DBMS_TESTS}
        PRIVATE gtest_main
                ${TARGET_DBMS}
        )
add_test(
        NAME ${TARGET_DBMS_TESTS}
        COMMAND ${TARGET_DBMS_TESTS}
)

# tests for dbmsUtils component

set(TARGET_DBMS_UTILS_TESTS dbmsUtils_tests)
add_executable(${TARGET_DBMS_UTILS_TESTS})
target_sources(${TARGET_DBMS_UTILS_TESTS}
        PRIVATE dbmsUtils_tests.cpp
                ../src/dbms/dbmsUtils.c
                ../src/utils.c
        )
target_link_libraries(${TARGET_DBMS_UTILS_TESTS} PRIVATE gtest_main)
target_include_directories(${TARGET_DBMS_UTILS_TESTS}
        PRIVATE ../include
                ../include/dbms
        )
add_test(
        NAME ${TARGET_DBMS_UTILS_TESTS}
        COMMAND ${TARGET_DBMS_UTILS_TESTS}
)

# tests for netUtils library

set(TARGET_NET_UTILS_TESTS netUtils_tests)
add_executable(${TARGET_NET_UTILS_TESTS})
target_sources(${TARGET_NET_UTILS_TESTS} PRIVATE netUtils_tests.cpp)
target_link_libraries(${TARGET_NET_UTILS_TESTS}
        PRIVATE gtest_main
                ${TARGET_NET_UTILS}
        )
add_test(
        NAME ${TARGET_NET_UTILS_TESTS}
        COMMAND ${TARGET_NET_UTILS_TESTS}
)

# tests for Utils component

set(TARGET_UTILS_TESTS Utils_tests)
add_executable(${TARGET_UTILS_TESTS})
target_sources(${TARGET_UTILS_TESTS}
        PRIVATE utils_tests.cpp
                ../src/utils.c
        )
target_link_libraries(${TARGET_UTILS_TESTS} PRIVATE gtest_main)
target_include_directories(${TARGET_UTILS_TESTS} PRIVATE ../include)
add_test(
        NAME ${TARGET_UTILS_TESTS}
        COMMAND ${TARGET_UTILS_TESTS}
)
