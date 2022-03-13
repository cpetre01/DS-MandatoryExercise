#include "include/message.h"
#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Usage client <clientName>\n");
        exit(0);
    }

    /* initializing the queue */
    init_name(argv[1]);

    /* initializing server */
    if (init_queue() == 0) {
        printf("The server has been initialized");
    } else {
        perror("Server has not been initialized");
    }

    /* loop to control server requests*/
    int controlVAr = 1;
    while (controlVAr == 1) {
        int action;
        printf("The possible operations to perform are the following:"
               "\n1. Insert a new tuple\n2. Obtain an existing tuple\n3. Modify an existing tuple\n"
               "4. Delete a tuple\n5. Check if a tuple is already stored\n6. How many tuples are stored?\n7. Exit");
        scanf("Please, insert the number of the operation to perform: %d", &action);
        while ((action < 1) || (action > 7)) {
            printf("Please, select one of the previous-defined actions");
            scanf("%d", &action);
        } // end second while


        switch (action) {
            case 1: {
                // insert a new tuple
                int key;
                char value1[MAX_STR_SIZE];
                int value2;
                float value3;

                //ask for the values
                scanf("Introduce the value for the key: %d", &key);
                scanf("Introduce value2: %s", &value1);
                scanf("Introduce value3: %d", &value2);
                scanf("Introduce value3: %f", &value3);

                // call the set_value function to perform the task
                int error = set_value(key, value1, value2, value3);
                if (error == 0) {
                    printf("The tuple was successfully inserted");
                } else {
                    printf("Error while inserting the tuple");
                }

                break;
            } // end case 1


            case 2: {
                /*Obtain an existing tuple: */
                int key;
                char value1[MAX_STR_SIZE];
                int value2;
                float value3;

                scanf("Please, enter the key %d", &key);
                /* calling the function*/
                int error = get_value(key, value1, &value2, &value3);
                if (error == 0) {
                    printf("The tuple with key %d stores value 1 = %s, value 2 = %d and value 3 = %f\n", key, value1,
                           value2, value3);
                } else {
                    printf("An error happened when searching the tuple.\n");
                }
                break;
            } // end case 2


            case 3: {
                /* Modify an existing tuple */
                int key;
                char value1[MAX_STR_SIZE];
                int value2;
                float value3;

                // ask for the new values
                scanf("Please, introduce the key:%d", &key);
                scanf("Please, introduce value 1: %s", &value1);
                scanf("Please, introduce value 2: %d", &value2);
                scanf("Please, introduce value 3: %f", &value3);

                //calling modify function
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
                /* Delete a tuple*/
                int key;
                scanf("Please, introduce the key of the tuple to delete:%d", &key);
                //calling delete function
                int error = delete_key(key);

                if (error == 0) {
                    printf("The tuple with key %d was deleted.\n", key);
                } else {
                    printf("Error deleting the tuple\n");
                }
                break;
            }// end case 4


            case 5: {
                /*Check if a tuple is already stored*/
                int key;
                //ask for the key
                scanf("Please, introduce the key of the tuple to check:%d", &key);
                int error = exist(key);
                //And show if the operation was or not succeed:
                if(error==0){
                    printf("A tuple with the key %d is already stored.\n", key);
                }else{
                    printf("There are no tuples with the key %d already stored.\n", key);
                }
                break;
            }// end case 5


            case 6: {
                /*How many tuples are stored?*/
                int num_tuples = num_items();
                if(num_tuples >= 0){
                    printf("There are %d tuples stored.\n", num_tuples);
                }else{
                    printf("Error counting the number of elements stored.\n");
                }
                break;
            } // end case 6


            case 7: {
                /*exit by changing the control var to 0*/
                controlVAr = 0;
                break;
            }// end case 7

        } // end switch

    }// end outer while

}// end main
        
