#include <stdio.h>
#include <stdlib.h>
#include "include/utils.h"
#include "include/keys.h"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Usage client <clientName>\n");
        exit(0);
    }

    /* initializing the queue */
    init_name(argv[1]);

    /* initializing server */
    if (init() == 0) {
        printf("The server has been initialized");
    } else {
        perror("Server has not been initialized");
    }

    /* loop to control client requests */
    int control_var = TRUE;
    while (control_var == TRUE) {
        /* display available actions */
        int action; char action_str[2];
        printf("The possible operations to perform are the following:"
               "\n1. Insert a new tuple\n2. Obtain an existing tuple\n3. Modify an existing tuple\n"
               "4. Delete a tuple\n5. Check if a tuple is already stored\n6. How many tuples are stored?\n7. Exit");

        /* ask for an action */
        scanf("Please, insert the number of the operation to perform: %s", action_str);
        while ((cast_value(action_str, (void *) &action, INT) == -1) || (action < 1) || (action > 7)) {
            printf("Please, select one of the previous-defined actions");
            scanf("%s", action_str);
        } // end inner while

        /* continue with chosen action */
        switch (action) {
            case 1: {
                /* insert a new tuple */
                int key; char key_str[MAX_STR_SIZE];
                char value1[MAX_STR_SIZE];
                int value2; char value2_str[MAX_STR_SIZE];
                float value3; char value3_str[MAX_STR_SIZE];

                /* ask for the values and key */
                scanf("Introduce the value for the key: %s", key_str);
                if (cast_value(key_str, (void *) &key, INT) == -1) {
                    printf("Please introduce an integer\n");
                    continue;
                }
                scanf("Introduce value1: %s", value1);
                scanf("Introduce value2: %s", value2_str);
                if (cast_value(value2_str, (void *) &value2, INT) == -1) {
                    printf("Please introduce an integer\n");
                    continue;
                }
                scanf("Introduce value3: %s", value3_str);
                if (cast_value(value3_str, (void *) &value3, FLOAT) == -1) {
                    printf("Please introduce a float\n");
                    continue;
                }

                /* call the set_value function to perform the task */
                int error = set_value(key, value1, value2, value3);
                if (error == 0) {
                    printf("The tuple was successfully inserted\n");
                } else {
                    printf("Error while inserting the tuple\n");
                }
                break;
            } // end case 1
            case 2: {
                /* obtain an existing tuple */
                int key; char key_str[MAX_STR_SIZE];
                char value1[MAX_STR_SIZE];
                int value2;
                float value3;

                /* ask for the key */
                scanf("Please, enter the key: %s", key_str);
                if (cast_value(key_str, (void *) &key, INT) == -1) {
                    printf("Please introduce an integer\n");
                    continue;
                }
                /* calling the function */
                int error = get_value(key, value1, &value2, &value3);
                if (error == 0) {
                    printf("The tuple with key %d stores value 1 = %s, value 2 = %d and value 3 = %f\n",
                           key, value1, value2, value3);
                } else {
                    printf("An error happened when searching the tuple.\n");
                }
                break;
            } // end case 2
            case 3: {
                /* modify an existing tuple */
                int key; char key_str[MAX_STR_SIZE];
                char value1[MAX_STR_SIZE];
                int value2; char value2_str[MAX_STR_SIZE];
                float value3; char value3_str[MAX_STR_SIZE];

                /* ask for the key */
                scanf("Introduce the value for the key: %s", key_str);
                if (cast_value(key_str, (void *) &key, INT) == -1) {
                    printf("Please introduce an integer\n");
                    continue;
                }
                /* ask for the new values */
                scanf("Introduce value1: %s", value1);
                scanf("Introduce value2: %s", value2_str);
                if (cast_value(value2_str, (void *) &value2, INT) == -1) {
                    printf("Please introduce an integer\n");
                    continue;
                }
                scanf("Introduce value3: %s", value3_str);
                if (cast_value(value3_str, (void *) &value3, FLOAT) == -1) {
                    printf("Please introduce a float\n");
                    continue;
                }

                /* calling modify function */
                int error = modify_value(key, value1, value2, value3);

                if (error == 0) {
                    printf("The tuple with key %d was modified to value 1 = %s, value 2 = %d and value 3 = %f\n", key,
                           value1, value2, value3);
                } else {
                    printf("Error modifying the tuple\n");
                }
                break;
            }//end case 3
            case 4: {
                /* delete a tuple */
                int key; char key_str[MAX_STR_SIZE];

                /* ask for the key */
                scanf("Please, introduce the key of the tuple to delete: %s", key_str);
                if (cast_value(key_str, (void *) &key, INT) == -1) {
                    printf("Please introduce an integer\n");
                    continue;
                }
                /* calling delete function */
                int error = delete_key(key);

                if (error == 0) {
                    printf("The tuple with key %d was deleted.\n", key);
                } else {
                    printf("Error deleting the tuple\n");
                }
                break;
            }// end case 4
            case 5: {
                /* check if a tuple exists */
                int key; char key_str[MAX_STR_SIZE];

                /* ask for the key */
                scanf("Please, introduce the key of the tuple to check: %s", key_str);
                if (cast_value(key_str, (void *) &key, INT) == -1) {
                    printf("Please introduce an integer\n");
                    continue;
                }

                int error = exist(key);
                /* And show if the operation was or not succeed: */
                if(error == 0) {
                    printf("A tuple with the key %d is already stored.\n", key);
                }else{
                    printf("There are no tuples with the key %d already stored.\n", key);
                }
                break;
            }// end case 5
            case 6: {
                /* how many tuples are stored? */
                int num_tuples = num_items();
                if(num_tuples >= 0) {
                    printf("There are %d tuples stored.\n", num_tuples);
                }else{
                    printf("Error counting the number of elements stored.\n");
                }
                break;
            } // end case 6
            case 7: {
                /* exit by changing the control var to 0 */
                control_var = 0;
                break;
            }// end case 7
            default:
                break;
        } // end switch
    }// end outer while
}
