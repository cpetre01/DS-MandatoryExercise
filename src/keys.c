#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <mqueue.h>
#include "message.h"


int init() {
    struct request request;
    request.msg_code = 'a';

    return 0;
}

int set_value(int key, char *value1, int value2, float value3){
    struct request request;
    request.msg_code = 'b';

    /* error if the key already exists*/
    FILE *ptr;
    char keyname = key +'0';
    if (fopen(keyname,"r") != NULL){
        fclose(ptr);
        perror("It already exists a file storing that key.\n");
        return -1;
    }


    /* creating new item */
    struct item new_entry;
    new_entry.key= key;                             /* key attribute */
    strcpy(new_entry.value1,value1);        /* string attribute */
    new_entry.value2 = value2;                               /* int attribute */
    new_entry.value3 = value3;                               /* float attribute */


    /* storing the new item */
    ptr = fopen(keyname,"w");
    if(ptr == NULL){
        perror("error while opening file\n");
        /* error*/
        return -1;
    }

    /* Write data to file in bytes */
    fprintf(ptr, "%s\n%d\n%f\n", new_entry.value1, new_entry.value2, new_entry.value3);

    /* Close file to save file data */
    fclose(ptr);


    /* once server responds */
    return 0;
}

int get_value(int key, char *value1, int *value2, float *value3){
    struct request request;
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
    struct request request;
    request.msg_code = 'd';

    FILE *ptr;
    char keyname = key +'0';
    /* error if there is no file associated with that key */
    if (fopen(keyname,"r") == NULL){
        fclose(ptr);
        perror("It already exists a file storing that key.\n");
        return -1;
    }


}

int delete_key(int key){
    struct request request;
    request.msg_code = 'e';

    FILE *ptr;
    char keyname = key +'0';
    /* error if there is no file associated with that key */
    if (fopen(keyname,"r") == NULL){
        fclose(ptr);
        perror("It does not exist a file storing that key.\n");
        return -1;
    }
    if (remove(keyname) == 0) {
        /* file was deleted successfully*/
        return 0;
    } else {
        /* error*/
        return -1;
    }

}

int exist(int key){
    struct request request;
    request.msg_code = 'f';

    FILE *ptr;
    char keyname = key +'0';

    if (fopen(keyname,"r") == NULL){
        fclose(ptr);
        return -1;
    }
    return 0;
}

int num_items(){
    struct request request;
    request.msg_code = 'g';

    int num_items;

    /* error */
    return -1;
    /* once server responds */
    return num_items;

}