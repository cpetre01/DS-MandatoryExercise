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
