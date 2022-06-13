/* gtest.h declares the testing framework */
#include "gtest/gtest.h"

extern "C" {
#include <DS-MandatoryExercise/utils.h>
}

/* test error codes */
const int SUCCESS = 0;
const int ERROR = -1;


TEST(str_to_num_tests, test_int_1) {
    char string[] = "123\0";
    number_t number = str_to_num(string, INT);

    ASSERT_EQ(number.err_code, SUCCESS);
    ASSERT_EQ(number.i, 123);
}


TEST(str_to_num_tests, test_int_2) {
    char string[] = "123 \0";
    number_t number = str_to_num(string, INT);

    ASSERT_EQ(number.err_code, SUCCESS);
    ASSERT_EQ(number.i, 123);
}


TEST(str_to_num_tests, test_float_1) {
    char string[] = "123.456\0";
    number_t number = str_to_num(string, FLOAT);

    ASSERT_EQ(number.err_code, SUCCESS);
    ASSERT_EQ(number.f, 123.456f);
}


TEST(str_to_num_tests, test_float_2) {
    char string[] = " 123.456\0";
    number_t number = str_to_num(string, FLOAT);

    ASSERT_EQ(number.err_code, SUCCESS);
    ASSERT_EQ(number.f, 123.456f);
}


TEST(str_to_num_tests, test_empty_string) {
    char string[] = "\0";
    number_t number = str_to_num(string, INT);

    ASSERT_EQ(number.err_code, ERROR);
}


TEST(str_to_num_tests, test_invalid_type) {
    char string[] = "123\0";
    number_t number = str_to_num(string, 'a');

    ASSERT_EQ(number.err_code, ERROR);
}
