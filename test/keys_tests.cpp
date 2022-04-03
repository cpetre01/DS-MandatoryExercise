/* gtest.h declares the testing framework */
#include "gtest/gtest.h"
#include <cstdlib>

extern "C" {
#include "keys.h"
#include "utils.h"
}


TEST(keys_tests, test) {
    ASSERT_EQ(true, true);
}

//TEST(tests, test_init) {
//
//    /* success */
//    ASSERT_EQ(init(), 0);
//
//}
//
//TEST(tests,test_set_value) {
//    init();
//    /* success */
//    ASSERT_EQ(set_value(22, "hello", 22, 22.2), 0);
//
//    /* failure: creating a file with an existing key*/
//    ASSERT_EQ(set_value(22, "bye", 11, 11.1), -1);
//
//}
//
//TEST(tests,test_get_value) {
//    char value1[VALUE1_MAX_STR_SIZE]; int value2; float value3;
//    init();
//    set_value(22, "hello", 22, 22.2);
//    /* success */
//    ASSERT_EQ(get_value(22, value1, &value2, &value3), 0);
//
//    /* failure: getting values of a file with non existent key */
//    ASSERT_EQ( get_value(33, value1, &value2, &value3), -1);
//
//}
//
//TEST(tests,test_modify_value) {
//    init();
//    set_value(22, "hello", 22, 22.2);
//    /* success */
//    ASSERT_EQ(modify_value(22, "aloha", 44, 44.4), 0);
//
//    /* failure: modify values of a file with non existent key  */
//    ASSERT_EQ(modify_value(33, "aloha", 44, 44.4), -1);
//
//}
//
//TEST(tests,test_delete_key) {
//    init();
//    set_value(22, "hello", 22, 22.2);
//    /* success */
//    ASSERT_EQ(delete_key(22), 0);
//
//    /* failure: deleting a file with non existent key  */
//    ASSERT_EQ(delete_key(33), -1);
//
//}
//
//TEST(tests,test_exists) {
//    init();
//    set_value(22, "hello", 22, 22.2);
//    /* the key exists */
//    ASSERT_EQ(exist(22), 1);
//
//    /* the key does not exists */
//    ASSERT_EQ(exist(33), 0);
//
//}
//
//TEST(tests,test_num_items) {
//    init();
//    set_value(22, "hello", 22, 22.2);
//    set_value(33, "hello2", 33, 33.3);
//    set_value(44, "hello3", 44, 44.4);
//
//    /* success */
//    ASSERT_EQ(num_items(), 3);
//
//}
