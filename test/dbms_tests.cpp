#include "gtest/gtest.h"
#include <cstdio>

extern "C" {
#include "dbms.h"
#include "dbmsUtils.h"
#include "utils.h"
}


TEST(dbms_tests, test_write_item) {
    
    db_empty_db();
    int value2 = 123456789;
    float value3 = 1.123456789f;

    // file with key 100 do not exists in the database
    ASSERT_EQ(db_item_exists(100), 0);
    
    // creating file
    ASSERT_EQ(db_write_item(100, "test", &value2, &value3, CREATE),0);
    
    // checking if it exists now
    ASSERT_EQ(db_item_exists(100), 1);
    
    // creating file with the same key should give already existent error
    ASSERT_EQ(db_write_item(100, "test", &value2, &value3, CREATE),-1);

}

TEST(dbms_tests, test_modify_item)
{
    db_empty_db();
    int value2 = 987654321;
    float value3 = 9.987654321f;

    // modifying a non existent file
    ASSERT_EQ(db_write_item(101, "test_modify_item", &value2, &value3, MODIFY),-1);
    
    // creating the file
    db_write_item(101, "test", &value2, &value3, CREATE);

    // modifying the file
    ASSERT_EQ(db_write_item(101, "test_modify_item", &value2, &value3, MODIFY),0);

}


TEST(dbms_tests,test_read_item)
{
    db_empty_db();
    char value1[VALUE1_MAX_STR_SIZE];
    int value2;
    float value3;
    
    // the file to read does not exists
    ASSERT_EQ(db_read_item(102, value1, &value2, &value3), -1);

    // creating the file
    db_write_item(102, "test", &value2, &value3, CREATE);

    // reading the file
    ASSERT_EQ(db_read_item(102, value1, &value2, &value3),0);
    
}


/*

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

    printf("\nlist DB entries\n");
    result = db_list_items();
    printf("return value: %d\n\n", result);

    printf("\ntest_read_item (key file %d doesn't exist)\n\n", n+1);
    test_read_item(n+1);

    printf("\ntest_modify_item (key file %d)\n\n", n);
    test_modify_item(n);

    printf("\ntest_read_item (key file %d)\n\n", n);
    test_read_item(n);

    printf("\ntest_modify_item (key file %d doesn't exist)\n\n", n+1);
    test_modify_item(n+1);

    printf("\ntest_write_item (key file %d exists)\n\n", n);
    test_write_item(n);

    printf("\ndeleting key file %d: expecting 0\n", n);
    result = db_delete_item(n);
    printf("return value: %d\n\n", result);

    printf("\ndeleting key file %d (key file doesn't exist): expecting -1\n", n+1);
    result = db_delete_item(n + 1);
    printf("return value: %d\n\n", result);

    printf("\nlist DB entries after deleting key file %d\n", n);
    result = db_list_items();
    printf("return value: %d\n\n", result);

    printf("\ntest_empty_db\n");
    result = db_empty_db();
    printf("return value: %d\n\n", result);

    printf("\nlist DB entries after emptying it\n");
    result = db_list_items();
    printf("return value: %d\n\n", result);

    return 0;
}
*/