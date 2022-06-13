# tests with GoogleTest

# tests for keys library (client API, a running server is required)

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


# tests for str_to_num function (utils module)

set(TARGET_STR_TO_NUM str_to_num_tests)
add_executable(${TARGET_STR_TO_NUM})
target_sources(${TARGET_STR_TO_NUM}
        PRIVATE str_to_num_tests.cpp
                ../src/utils.c
        )
target_include_directories(${TARGET_STR_TO_NUM} PRIVATE ../include)
target_link_libraries(${TARGET_STR_TO_NUM} PRIVATE gtest_main)
add_test(
        NAME ${TARGET_STR_TO_NUM}
        COMMAND ${TARGET_STR_TO_NUM}
)
