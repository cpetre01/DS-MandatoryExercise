/* gtest.h declares the testing framework */
#include "gtest/gtest.h"

extern "C" {
#include "utils.h"
}


TEST(utils_tests, test) {
    ASSERT_EQ(true, true);
}
