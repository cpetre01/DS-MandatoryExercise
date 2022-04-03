#include <check.h>               
#include "../src/keys.h"        
#include <stdlib.h>
#include "utils.h"

int f_output;

/* test for function init*/
START_TEST(test_init) {    

    f_output = init();

    /* success */
    ck_assert_int_eq(f_output, 0);

}END_TEST  

START_TEST(test_set_value) {    
    init();
    f_output = set_value(22, "hello", 22, 22.2);
    /* success */
    ck_assert_int_eq(f_output, 0);

    /* failure: creating a file with an existing key*/      
    f_output = set_value(22, "bye", 11, 11.1); 
    ck_assert_int_eq(f_output, -1);

}END_TEST 

START_TEST(test_get_value) { 
    char value1[VALUE1_MAX_STR_SIZE]; int value2; float value3;   
    init();
    set_value(22, "hello", 22, 22.2);
    f_output = get_value(22, value1, &value2, &value3);
    /* success */
    ck_assert_int_eq(f_output, 0);

    /* failure: getting values of a file with non existent key */
    f_output = get_value(33, value1, &value2, &value3);       
    ck_assert_int_eq(f_output, -1);

}END_TEST

START_TEST(test_modify_value) {    
    init();
    set_value(22, "hello", 22, 22.2);
    f_output = modify_value(22, "aloha", 44, 44.4);
    /* success */
    ck_assert_int_eq(f_output, 0);

    f_output = modify_value(33, "aloha", 44, 44.4);
    /* failure: modify values of a file with non existent key  */       
    ck_assert_int_eq(f_output, -1);

}END_TEST 

START_TEST(test_delete_key) {    
    init();
    set_value(22, "hello", 22, 22.2);
    f_output = delete_key(22);
    /* success */
    ck_assert_int_eq(f_output, 0);

    f_output = delete_key(33);
    /* failure: deleting a file with non existent key  */       
    ck_assert_int_eq(f_output, -1);

}END_TEST 

START_TEST(test_exists) {    
    init();
    set_value(22, "hello", 22, 22.2);
    f_output = exists(22);
    /* the key exists */
    ck_assert_int_eq(f_output, 1);

    f_output = exists(33);
    /* the key does not exists */       
    ck_assert_int_eq(f_output, 0);

}END_TEST 

START_TEST(test_num_items) {    
    init();
    set_value(22, "hello", 22, 22.2);
    set_value(33, "hello2", 33, 33.3);
    set_value(44, "hello3", 44, 44.4);

    f_output = num_items();

    /* success */
    ck_assert_int_eq(f_output, 0);
    /* failure */       
    ck_assert_int_eq(f_output, -1);

}END_TEST



Suite *keys_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("keys");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_init);
    tcase_add_test(tc_core, test_set_value);
    tcase_add_test(tc_core, test_get_value);
    tcase_add_test(tc_core, test_modify_value);
    tcase_add_test(tc_core, test_delete_key);
    tcase_add_test(tc_core, test_exists);
    tcase_add_test(tc_core, test_num_items);

    suite_add_tcase(s, tc_core);

  return s;
}



int main(){

    /*creating a client for testing*/
    int control_var = TRUE;
    const char *server_ip = getenv("IP_TUPLES");
    const char *server_port = getenv("PORT_TUPLES");

    if (!server_ip || !server_port) {
        fprintf(stderr, "getenv error\n");
        return -1;
    }

    int server_port_num = str_to_num(server_port, (void *) &server_port, INT);
    if (server_port_num == -1) {
        perror("Invalid server port"); return -1;
    }

    /* initialize connection with the sever side */
    init_connection( server_ip ,server_port_num);

    
    /********************************************/
    /* calling tests */
    int no_failed = 0;                   
    Suite *s;                            
    SRunner *runner;                     

    s = keys_suite();                   
    runner = srunner_create(s);          

    srunner_run_all(runner, CK_NORMAL);  
    no_failed = srunner_ntests_failed(runner); 
    srunner_free(runner);                      
    return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;


}