#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <mqueue.h>
#include "message.h"


int init(){
    struct message request;
    request.msg_code = 'a';

    /* error */
    return -1;
    /* once server responds */
    return 0;
}

int set_value(int key, char *value1, int value2, float value3){
    struct message request;
    request.msg_code = 'b';

    /* error */
    return -1;
    /* once server responds */
    return 0;
}

int get_value(int key, char *value1, int *value2, float *value3){
    struct message request;
    request.msg_code = 'c';

    /* error */
    return -1;
    /* once server responds */
    return 0;
}

int modify_value(int key, char *value1, int value2, float value3){
    struct message request;
    request.msg_code = 'd';

    /* error */
    return -1;
    /* once server responds */
    return 0;
}

int delete_key(int key){
    struct message request;
    request.msg_code = 'e';

    /* error */
    return -1;
    /* once server responds */
    return 0;
}

int exist(int key){
    struct message request;
    request.msg_code = 'f';

    /* error */
    return -1;
    /* once server responds */
    /* not exists */
    return 0;
    /* does exist */
    return 1;
}

int num_items(){
    struct message request;
    request.msg_code = 'g';

    int num_items;

    /* error */
    return -1;
    /* once server responds */
    return num_items;

}