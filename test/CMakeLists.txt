# tests

# tests for keys library

set(TARGET_TESTS keys_tests)
add_executable(${TARGET_TESTS})
target_sources(${TARGET_TESTS} PRIVATE keys_tests.cpp)
target_link_libraries(${TARGET_TESTS}
        PUBLIC  gtest_main
                ${TARGET_KEYS}
        )
add_test(
        NAME ${TARGET_TESTS}
        COMMAND ${TARGET_TESTS}
)

# tests for dbms library

set(TARGET_DBMS_TESTS dbms_tests)
add_executable(${TARGET_DBMS_TESTS})
target_sources(${TARGET_DBMS_TESTS} PRIVATE dbms_tests.cpp)
target_link_libraries(${TARGET_DBMS_TESTS}
        PUBLIC  gtest_main
                ${TARGET_DBMS}
        )
add_test(
        NAME ${TARGET_DBMS_TESTS}
        COMMAND ${TARGET_DBMS_TESTS}
)
