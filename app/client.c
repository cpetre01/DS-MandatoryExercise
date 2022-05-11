/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include <stdio.h>
#include "DS-MandatoryExercise/utils.h"
#include "DS-MandatoryExercise/rpc.h"
#include "DS-MandatoryExercise/keys.h"

const char display_actions_str[] = "The possible operations to perform are the following:\n"
                                   "1. Initialize DataBase\n"
                                   "2. Insert a new tuple\n"
                                   "3. Obtain an existing tuple\n"
                                   "4. Modify an existing tuple\n"
                                   "5. Delete a tuple\n"
                                   "6. Check if a tuple is already stored\n"
                                   "7. How many tuples are stored?\n"
                                   "8. Exit\n\0";
/* prompts */
const char ask_key_prompt[] = "Please, enter the key\0";
const char ask_key_delete_prompt[] = "Please, enter the key of the tuple to delete\0";
const char ask_key_exist_prompt[] = "Please, enter the key of the tuple to search for\0";
const char ask_value1_prompt[] = "Please, enter value1\0";
const char ask_value2_prompt[] = "Please, enter value2\0";
const char ask_value3_prompt[] = "Please, enter value3\0";
/* input errors */
const char action_error[] = "Please, select one of the previously defined actions: \0";
const char int_required_error[] = "Please, enter an integer\0";
const char float_required_error[] = "Please, enter a real number\0";

const unsigned int ACTION_STR_LEN = 2;            /* size of client interface action string */


int get_key(int *key, const char *prompt_str);
int get_key_and_values(int *key, char *value1, int *value2, float *value3);


int get_key(int *key, const char *prompt_str) {
    char key_str[MAX_STR_SIZE];

    /* ask for the key */
    printf("%s: ", prompt_str); scanf("%s", key_str);
    if (str_to_num(key_str, (void *) key, INT) == -1) {
        fprintf(stderr, "%s\n", int_required_error); return -1;
    }
    return 0;
}


int get_key_and_values(int *key, char *value1, int *value2, float *value3) {
    char value2_str[MAX_STR_SIZE]; char value3_str[MAX_STR_SIZE];

    /* ask for the key */
    if (get_key(key, ask_key_prompt) == -1) return -1;

    /* ask for the values */
    printf("%s: ", ask_value1_prompt); scanf("%s", value1);
    printf("%s: ", ask_value2_prompt); scanf("%s", value2_str);
    if (str_to_num(value2_str, (void *) value2, INT) == -1) {
        fprintf(stderr, "%s\n", int_required_error); return -1;
    }
    printf("%s: ", ask_value3_prompt); scanf("%s", value3_str);
    if (str_to_num(value3_str, (void *) value3, FLOAT) == -1) {
        fprintf(stderr, "%s\n", float_required_error); return -1;
    }
    return 0;
}

int
main (int argc, char *argv[]) {
    CLIENT *clnt;
    enum clnt_stat retval_1;
    enum clnt_stat retval_2;
    enum clnt_stat retval_3;
    struct item result_3;
    result_3.value1 = malloc(255);
    enum clnt_stat retval_4;
    enum clnt_stat retval_5;
    enum clnt_stat retval_6;
    enum clnt_stat retval_7;
    int control_var = TRUE;

    while (control_var) {
        /* display available actions */
        int action;
        char action_str[ACTION_STR_LEN];
        printf("\n%s\n", display_actions_str);

        printf("Please, insert the number of the operation to perform: ");
        scanf("%s", action_str);
        while ((str_to_num(action_str, (void *) &action, INT) == -1) || (action < 1) || (action > 8)) {
            fprintf(stderr, "%s", action_error);
            scanf("%s", action_str);
        } // end inner while

        switch (action) {
            case 1: {
                retval_1 = init();
                if (retval_1 != 0) {
                    perror("\nThe Database has not been initialized");
                }
                fprintf(stderr, "\nThe Database has been initialized\n");
                break;
            }


            case 2: {
                int key;
                char value1[VALUE1_MAX_STR_SIZE];
                int value2;
                float value3;

                /* ask for the values and key */
                if (get_key_and_values(&key, value1, &value2, &value3) == -1) continue;
                retval_2 = set_value(key, value1, value2, value3);
                if (retval_2 != 0) {
                    fprintf(stderr, "\nError while inserting the tuple\n");
                }
                fprintf(stderr, "\nThe tuple was successfully inserted\n");
                break;
            }

            case 3: {
                int key;
                char value1[VALUE1_MAX_STR_SIZE];
                int value2;
                float value3;
                /* ask for the key */
                if (get_key(&key, ask_key_prompt) == -1) continue;
                retval_3 = get_value(key, value1, &value2, &value3);
                if (retval_3 != 0) {
                    fprintf(stderr, "\nAn error happened when searching the tuple.\n");
                }
                fprintf(stderr, "\nThe tuple with key %d stores value 1 = %s, "
                                "value 2 = %d and value 3 = %f\n", key, value1, value2, value3);
                break;
            }

            case 4: {
                int key;
                char value1[VALUE1_MAX_STR_SIZE];
                int value2;
                float value3;
                /* ask for the values and key */
                if (get_key_and_values(&key, value1, &value2, &value3) == -1) continue;
                retval_4 = modify_value(key, value1, value2, value3);
                if (retval_4 != 0) {
                    fprintf(stderr, "\nError modifying the tuple\n");

                }
                fprintf(stderr, "\nThe tuple with key %d was modified to value 1 = %s, "
                                "value 2 = %d and value 3 = %f\n", key, value1, value2, value3);
                break;
            }

            case 5: {
                int key;
                /* ask for the key */
                if (get_key(&key, ask_key_delete_prompt) == -1) continue;
                retval_5 = delete_key(key);
                if (retval_5 != 0) {
                    fprintf(stderr, "\nError deleting the tuple\n");
                }
                fprintf(stderr, "\nThe tuple with key %d was deleted.\n", key);
                break;
            }

            case 6: {
                int key;
                /* ask for the key */
                if (get_key(&key, ask_key_exist_prompt) == -1) continue;
                retval_6 = exist(key);
                if (retval_6 == 1) fprintf(stderr, "\nA tuple with the key %d is already stored.\n", key);
                else if (!retval_6) fprintf(stderr, "\nThere are no tuples with the key %d stored.\n", key);
                else fprintf(stderr, "\nCommunication error.\n");

                break;
            }

            case 7: {
                retval_7 = num_items();
                if (retval_7 < 0)  fprintf(stderr, "\nError counting the number of elements stored.\n");

                break;
            }

            case 8: {
                control_var = FALSE;
                free(result_3.value1);
                break;
            }
            default:
                break;
        }
    }
}

