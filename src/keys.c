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

    /* creating new item */
    struct item new_entry;
    new_entry.key= key;                             /* key attribute */

    /* open file */
    FILE *ptr;
    char keyname[10];
    sprintf(keyname, "%d\n", key);
    ptr = fopen(keyname,"r");
    if(ptr == NULL){
        fclose(ptr);
        perror("error while opening file\n");
        /* error*/
        return -1;
    }

    char keystr[10]; char value1str[255]; char value2str[255]; char value3str[255];

    fgets(keystr, 10, (FILE*) ptr);
    fgets(value1str, 255, (FILE*) ptr);
    fgets(value2str, 255, (FILE*) ptr);
    fgets(value3str, 255, (FILE*) ptr);

    strcpy(new_entry.value1,value1str);
    new_entry.value2 = atoi(value2str);
    new_entry.value3 = atof(value3str);

    /* not sure if values must also be returned ? */

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