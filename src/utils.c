#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "include/utils.h"


int cast_value(const char *value_str, void *value, const char type) {
    /* cast value to int or float */
    char * endptr;      /* used for strtol and strtof castings */
    char error[10];     /* used to display errors */
    void *cast_value;   /* this points to cast value */
    size_t result_size; /* used to determine the size of the cast value in memcpy call */

    /* auxiliary variables to cast value */
    int value2_str_to_int;
    float value3_str_to_float;

    /* cast value */
    switch (type) {
        case INT:
            value2_str_to_int = (int)strtol(value_str, &endptr, 10);
            cast_value = (void *) &value2_str_to_int;
            strcpy(error, "strtol");
            result_size = sizeof(int);
            break;
        case FLOAT:
            value3_str_to_float = strtof(value_str, &endptr);
            cast_value = (void *) &value3_str_to_float;
            strcpy(error, "strtof");
            result_size = sizeof(float);
            break;
        default:
            printf("Invalid casting type\n");
            return -1;
    }

    /* check cast errors */
    if (errno != 0) {
        perror(error);
        return -1;
    }
    if (endptr == value_str) {
        fprintf(stderr, "No digits were found\n");
        return -1;
    }

    /* cast succeeded: set value */
    memcpy(value, cast_value, result_size);
    return 0;
}
