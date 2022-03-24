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
    client_queue_init(argv[1]);

    /* loop to control client requests */
    int control_var = TRUE;
    while (control_var == TRUE) {
        /* display available actions */
        int action; char action_str[2];
        printf("The possible operations to perform are the following:"
               "\n1. Initialize DataBase\n2. Insert a new tuple\n3. Obtain an existing tuple\n"
               "4. Modify an existing tuple\n5. Delete a tuple\n6. Check if a tuple is already stored\n"
               "7. How many tuples are stored?\n8. Exit\n");

        /* ask for an action */
        printf("Please, insert the number of the operation to perform: ");
        scanf("%s", action_str);
        while ((cast_value(action_str, (void *) &action, INT) == -1) || (action < 1) || (action > 8)) {
            printf("Please, select one of the previously defined actions");
            scanf("%s", action_str);
        } // end inner while
        printf("\n");

        /* continue with chosen action */
        switch (action) {
            case 1:
                /* initializing server db */
                if (!init()) {
                    printf("The Database has been initialized\n");
                } else {
                    perror("The Database has not been initialized");
                    continue;
                }
                printf("\n");
                break;
            case 2: {
                /* insert a new tuple */
                int key; char key_str[MAX_STR_SIZE];
                char value1[VALUE1_MAX_STR_SIZE];
                int value2; char value2_str[MAX_STR_SIZE];
                float value3; char value3_str[MAX_STR_SIZE];

                /* ask for the values and key */
                printf("Introduce the value for the key: ");
                scanf("%s", key_str);
                if (cast_value(key_str, (void *) &key, INT) == -1) {
                    fprintf(stderr, "Please introduce an integer\n");
                    continue;
                }
                printf("Introduce value1: ");
                scanf("%s", value1);
                printf("Introduce value2: ");
                scanf("%s", value2_str);
                if (cast_value(value2_str, (void *) &value2, INT) == -1) {
                    fprintf(stderr, "Please introduce an integer\n");
                    continue;
                }
                printf("Introduce value3: ");
                scanf("%s", value3_str);
                if (cast_value(value3_str, (void *) &value3, FLOAT) == -1) {
                    fprintf(stderr, "Please introduce a float\n");
                    continue;
                }
                printf("\n");

                /* call the set_value function to perform the task */
                int error = set_value(key, value1, value2, value3);
                if (!error) {
                    printf("The tuple was successfully inserted\n");
                } else {
                    fprintf(stderr, "Error while inserting the tuple\n");
                }
                printf("\n");
                break;
            } // end case 2
            case 3: {
                /* obtain an existing tuple */
                int key; char key_str[MAX_STR_SIZE];
                char value1[VALUE1_MAX_STR_SIZE];
                int value2;
                float value3;

                /* ask for the key */
                printf("Please, enter the key: ");
                scanf("%s", key_str);
                if (cast_value(key_str, (void *) &key, INT) == -1) {
                    fprintf(stderr, "Please introduce an integer\n");
                    continue;
                }
                printf("\n");

                /* calling the function */
                int error = get_value(key, value1, &value2, &value3);
                if (!error) {
                    printf("The tuple with key %d stores value 1 = %s, value 2 = %d and value 3 = %f\n",
                           key, value1, value2, value3);
                } else {
                    fprintf(stderr, "An error happened when searching the tuple.\n");
                }
                printf("\n");
                break;
            } // end case 3
            case 4: {
                /* modify an existing tuple */
                int key; char key_str[MAX_STR_SIZE];
                char value1[VALUE1_MAX_STR_SIZE];
                int value2; char value2_str[MAX_STR_SIZE];
                float value3; char value3_str[MAX_STR_SIZE];

                /* ask for the key */
                printf("Introduce the value for the key: ");
                scanf("%s", key_str);
                if (cast_value(key_str, (void *) &key, INT) == -1) {
                    fprintf(stderr, "Please introduce an integer\n");
                    continue;
                }
                /* ask for the new values */
                printf("Introduce value1: ");
                scanf("%s", value1);
                printf("Introduce value2: ");
                scanf("%s", value2_str);
                if (cast_value(value2_str, (void *) &value2, INT) == -1) {
                    fprintf(stderr, "Please introduce an integer\n");
                    continue;
                }
                printf("Introduce value3: ");
                scanf("%s", value3_str);
                if (cast_value(value3_str, (void *) &value3, FLOAT) == -1) {
                    fprintf(stderr, "Please introduce a float\n");
                    continue;
                }
                printf("\n");

                /* calling modify function */
                int error = modify_value(key, value1, value2, value3);

                if (!error) {
                    printf("The tuple with key %d was modified to value 1 = %s, value 2 = %d and value 3 = %f\n",
                           key, value1, value2, value3);
                } else {
                    fprintf(stderr, "Error modifying the tuple\n");
                }
                printf("\n");
                break;
            }//end case 4
            case 5: {
                /* delete a tuple */
                int key; char key_str[MAX_STR_SIZE];

                /* ask for the key */
                printf("Please, introduce the key of the tuple to delete: ");
                scanf("%s", key_str);
                if (cast_value(key_str, (void *) &key, INT) == -1) {
                    fprintf(stderr, "Please introduce an integer\n");
                    continue;
                }
                printf("\n");

                /* calling delete function */
                int error = delete_key(key);

                if (!error) {
                    printf("The tuple with key %d was deleted.\n", key);
                } else {
                    fprintf(stderr, "Error deleting the tuple\n");
                }
                printf("\n");
                break;
            }// end case 5
            case 6: {
                /* check if a tuple exists */
                int key; char key_str[MAX_STR_SIZE];

                /* ask for the key */
                printf("Please, introduce the key of the tuple to check: ");
                scanf("%s", key_str);
                if (cast_value(key_str, (void *) &key, INT) == -1) {
                    fprintf(stderr, "Please introduce an integer\n");
                    continue;
                }
                printf("\n");

                /* calling exists function and check for errors*/
                int error = exist(key);

                if(error == 1) {
                    printf("A tuple with the key %d is already stored.\n", key);
                }
                else if (!error) {
                    fprintf(stderr, "There are no tuples with the key %d already stored.\n", key);
                }
                else {
                    fprintf(stderr, "Communication error.\n");
                }
                printf("\n");
                break;

            }// end case 6
            case 7: {
                /* how many tuples are stored? */
                int num_tuples = num_items();
                if(num_tuples >= 0) {
                    printf("There are %d tuples stored.\n", num_tuples);
                }else{
                    fprintf(stderr, "Error counting the number of elements stored.\n");
                }
                printf("\n");
                break;
            } // end case 7
            case 8: {
                /* exit by changing the control var to 0 */
                control_var = 0;
                printf("\n");
                break;
            }// end case 8
            default:
                break;
        } // end switch
    }// end outer while
}
