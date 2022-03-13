#include <stdio.h>
#include "include/utils.h"
#include "include/dbmsUtils.h"
#include "include/dbms.h"


void test_write_item(const int key)
{
    char value1[MAX_STR_SIZE] = "test";
    int value2 = 123456789;
    float value3 = 1.123456789f;

    int result;

    result = item_exists(key);
    printf("before creating key file: item_exists return value: %d\n", result);

    result = write_item(key, value1, &value2, &value3, CREATE);
    printf("after creating key file: write_item return value: %d\n", result);

    result = item_exists(key);
    printf("after creating key file: item_exists return value: %d\n", result);
}


void test_modify_item(const int key)
{
    char value1[MAX_STR_SIZE] = "test_modify_item";
    int value2 = 987654321;
    float value3 = 9.987654321f;

    int result;

    result = item_exists(key);
    printf("before modifying key file: item_exists return value: %d\n", result);

    if (result > 0) {
        result = write_item(key, value1, &value2, &value3, MODIFY);
        printf("after modifying key file: modify_item return value: %d\n", result);
    } else
        printf("key file doesn't exist\n");

}


void test_read_item(const int key)
{
    char value1[MAX_STR_SIZE];
    int value2;
    float value3;

    int result;

    result = item_exists(key);
    printf("before reading key file: item_exists return value: %d\n", result);

    if (result > 0) {
        result = read_item(key, value1, &value2, &value3);
        printf("after reading key file: read_item return value: %d\n", result);

        printf("\nvalue1: %s\nvalue2: %d\nvalue3: %f\n", value1, value2, value3);
    } else
        printf("key file doesn't exist\n");
}


int main(void)
{
    int result;
    printf("\ntest_open_db\n");
    open_db();

    int n = 2;
    for (int i = 0; i <= n; ++i) {
        printf("\ntesting key: %d\n", i);

        printf("\ntest_write_item\n\n");
        test_write_item(i);

        printf("\ntest_read_item\n\n");
        test_read_item(i);

    }

    printf("\nlist db entries\n");
    result = list_db_entries();
    printf("return value: %d\n\n", result);

    printf("\ntest_read_item (key file %d doesn't exist)\n\n", n+1);
    test_read_item(n+1);

    printf("\ntest_modify_item (key file %d)\n\n", n);
    test_modify_item(n);

    printf("\ntest_read_item (key file %d)\n\n", n);
    test_read_item(n);

    printf("\ntest_modify_item (key file %d doesn't exist)\n\n", n+1);
    test_modify_item(n+1);

    printf("\ndeleting key file %d: expecting 0\n", n);
    result = delete_item(n);
    printf("return value: %d\n\n", result);

    printf("\ndeleting key file %d (key file doesn't exist): expecting -1\n", n+1);
    result = delete_item(n+1);
    printf("return value: %d\n\n", result);

    printf("\nlist db entries after deleting key file %d\n", n);
    result = list_db_entries();
    printf("return value: %d\n\n", result);

    printf("\ntest_empty_db\n");
    result = empty_db();
    printf("return value: %d\n\n", result);

    printf("\nlist db entries after emptying it\n");
    result = list_db_entries();
    printf("return value: %d\n\n", result);

    return 0;
}
