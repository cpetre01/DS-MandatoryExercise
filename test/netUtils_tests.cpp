/* gtest.h declares the testing framework */
#include "gtest/gtest.h"

extern "C" {
#include "utils.h"
#include "netUtils.h"
}


TEST(keys_tests, test) {
    ASSERT_EQ(true, true);
}
