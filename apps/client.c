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
        printf("The server has been initialized\n");
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
               "4. Delete a tuple\n5. Check if a tuple is already stored\n6. How many tuples are stored?\n7. Exit\n");

        /* ask for an action */
        printf("Please, insert the number of the operation to perform: ");
        scanf("%s", action_str);
        printf("action_str: %s\n", action_str);
        while ((cast_value(action_str, (void *) &action, INT) == -1) || (action < 1) || (action > 7)) {
            printf("Please, select one of the previous-defined actions");
            scanf("%s", action_str);
        } // end inner while

        /* continue with chosen action */
        switch (action) {
            case 1: {
                printf("case 1\n");
                /* insert a new tuple */
                int key; char key_str[MAX_STR_SIZE];
                char value1[MAX_STR_SIZE];
                int value2; char value2_str[MAX_STR_SIZE];
                float value3; char value3_str[MAX_STR_SIZE];

                /* ask for the values and key */
                printf("Introduce the value for the key: ");
                scanf("%s", key_str);
                if (cast_value(key_str, (void *) &key, INT) == -1) {
                    printf("Please introduce an integer\n");
                    continue;
                }
                printf("Introduce value1: ");
                scanf("%s", value1);
                printf("Introduce value2: ");
                scanf("%s", value2_str);
                if (cast_value(value2_str, (void *) &value2, INT) == -1) {
                    printf("Please introduce an integer\n");
                    continue;
                }
                printf("Introduce value3: ");
                scanf("%s", value3_str);
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
                printf("case 2\n");
                /* obtain an existing tuple */
                int key; char key_str[MAX_STR_SIZE];
                char value1[MAX_STR_SIZE];
                int value2;
                float value3;

                /* ask for the key */
                printf("Please, enter the key: ");
                scanf("%s", key_str);
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
                printf("case 3\n");
                /* modify an existing tuple */
                int key; char key_str[MAX_STR_SIZE];
                char value1[MAX_STR_SIZE];
                int value2; char value2_str[MAX_STR_SIZE];
                float value3; char value3_str[MAX_STR_SIZE];

                /* ask for the key */
                printf("Introduce the value for the key: ");
                scanf("%s", key_str);
                if (cast_value(key_str, (void *) &key, INT) == -1) {
                    printf("Please introduce an integer\n");
                    continue;
                }
                /* ask for the new values */
                printf("Introduce value1:");
                scanf("%s", value1);
                printf("Introduce value2:");
                scanf("%s", value2_str);
                if (cast_value(value2_str, (void *) &value2, INT) == -1) {
                    printf("Please introduce an integer\n");
                    continue;
                }
                printf("Introduce value3:");
                scanf("%s", value3_str);
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
                printf("case 4\n");
                /* delete a tuple */
                int key; char key_str[MAX_STR_SIZE];

                /* ask for the key */
                printf("Please, introduce the key of the tuple to delete: ");
                scanf("%s", key_str);
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
                printf("case 5\n");
                /* check if a tuple exists */
                int key; char key_str[MAX_STR_SIZE];

                /* ask for the key */
                printf("Please, introduce the key of the tuple to check:");
                scanf("%s", key_str);
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
                printf("case 6\n");
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
                printf("case 7\n");
                /* exit by changing the control var to 0 */
                control_var = 0;
                break;
            }// end case 7
            default:
                printf("default\n");
                break;
        } // end switch
    }// end outer while
}
