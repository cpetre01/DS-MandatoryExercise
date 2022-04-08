/* gtest.h declares the testing framework */
#include "gtest/gtest.h"
#include <cstdlib>

extern "C" {
#include "DS-MandatoryExercise/utils.h"
#include "DS-MandatoryExercise/keys.h"
}

/* test error codes */
const int SUCCESS = 0;
const int ERROR = -1;
const int EXISTS = 1;
const int NOT_EXISTS = 0;
/* env variables needed to connect to server */
//const char ip[2][MAX_STR_SIZE] = {"IP_TUPLES\0", "localhost\0"};
//const char port[2][MAX_STR_SIZE] = {"PORT_TUPLES\0", "10000\0"};


//TEST(keys_tests, initial_setup) {
//    /* set up server IP & port for client */
//    setenv(ip[0], ip[1], 1);
//    setenv(port[0], port[1], 1);
//}


TEST(keys_tests, test_init) {
    /* success */
    ASSERT_EQ(init(), SUCCESS);
}


TEST(keys_tests, test_set_value) {
    /* initial setup */
    init();
    /* success */
    int key_1 = 11;
    char value1_1[] = "hello\0";
    int value2_1 = 11;
    float value3_1 = 11.1f;
    ASSERT_EQ(set_value(key_1, value1_1, value2_1, value3_1), SUCCESS);

    /* failure: creating a file with an existing key */
    char value1_2[] = "bye\0";
    int value2_2 = 22;
    float value3_2 = 22.2f;
    ASSERT_EQ(set_value(key_1, value1_2, value2_2, value3_2), ERROR);
}


TEST(keys_tests, test_get_value) {
    /* initial setup */
    init();
    int key_1 = 11;
    char value1_1[] = "hello\0";
    int value2_1 = 11;
    float value3_1 = 11.1f;
    set_value(key_1, value1_1, value2_1, value3_1);

    /* success */
    char value1_2[VALUE1_MAX_STR_SIZE]; int value2_2; float value3_2;
    ASSERT_EQ(get_value(key_1, value1_2, &value2_2, &value3_2), SUCCESS);

    /* failure: getting values of a file with non-existent key */
    int key_2 = 22;
    ASSERT_EQ(get_value(key_2, value1_2, &value2_2, &value3_2), ERROR);

}


TEST(keys_tests, test_modify_value) {
    /* initial setup */
    init();
    int key_1 = 11;
    char value1_1[] = "hello\0";
    int value2_1 = 11;
    float value3_1 = 11.1f;
    set_value(key_1, value1_1, value2_1, value3_1);

    /* success */
    char value1_2[] = "aloha\0";
    int value2_2 = 22;
    float value3_2 = 22.2f;
    ASSERT_EQ(modify_value(key_1, value1_2, value2_2, value3_2), SUCCESS);

    /* failure: modify values of a file with non-existent key */
    int key_2 = 22;
    ASSERT_EQ(modify_value(key_2, value1_2, value2_2, value3_2), ERROR);
}


TEST(keys_tests, test_delete_key) {
    /* initial setup */
    init();
    int key_1 = 11;
    char value1_1[] = "hello\0";
    int value2_1 = 11;
    float value3_1 = 11.1f;
    set_value(key_1, value1_1, value2_1, value3_1);

    /* success */
    ASSERT_EQ(delete_key(key_1), SUCCESS);

    /* failure: deleting a file with non-existent key */
    int key_2 = 22;
    ASSERT_EQ(delete_key(key_2), ERROR);

}


TEST(keys_tests, test_exists) {
    /* initial setup */
    init();
    int key_1 = 11;
    char value1_1[] = "hello\0";
    int value2_1 = 11;
    float value3_1 = 11.1f;
    set_value(key_1, value1_1, value2_1, value3_1);

    /* the key exists */
    ASSERT_EQ(exist(key_1), EXISTS);

    /* the key does not exist */
    int key_2 = 22;
    ASSERT_EQ(exist(key_2), NOT_EXISTS);

}


TEST(keys_tests, test_num_items) {
    /* initial setup */
    init();

    /* success */
    ASSERT_EQ(num_items(), 0);

    /* add some tuples */
    int key_1 = 11; int key_2 = 22; int key_3 = 33;
    char value1_1[] = "hello1\0"; char value1_2[] = "hello2\0"; char value1_3[] = "hello3\0";
    int value2_1 = 11; int value2_2 = 22; int value2_3 = 33;
    float value3_1 = 11.1f; float value3_2 = 22.2f; float value3_3 = 33.3f;
    set_value(key_1, value1_1, value2_1, value3_1);
    set_value(key_2, value1_2, value2_2, value3_2);
    set_value(key_3, value1_3, value2_3, value3_3);

    /* success */
    ASSERT_EQ(num_items(), 3);
}


//TEST(keys_tests, close_setup) {
//    /* shut down the server */
//    system("pkill -SIGINT '^server$'");
//}