#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "include/utils.h"


int cast_value(const char *value_str, void *value, const char type) {
    errno = 0;
    /* cast value to int or float */
    char * endptr;      /* used for strtol and strtof castings */
    char error[10];     /* used to display errors */
    void *cast_value;   /* this points to cast value */
    size_t result_size; /* used to determine the size of the cast value in memcpy call */

    /* auxiliary variables to cast value */
    int value2_str_to_int;
    float value3_str_to_float;

    if (!strlen(value_str)) {
        fprintf(stderr, "String is empty\n");
        return -1;
    }

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
    if (errno) {
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


ssize_t read_line(const int fd, void *buffer, const size_t n) {
    ssize_t bytes_read;             /* num of bytes fetched by last read() */
    ssize_t bytes_read_total;       /* total bytes read so far */
    char *buf, ch;

    /* check arguments */
    if (n <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }

    buf = buffer;
    bytes_read_total = 0;
    /* read from fd */
    while (TRUE) {
        bytes_read = read(fd, &ch, 1);	/* read a byte */
        /* check what's been read */
        if (bytes_read == -1) {
            if (errno == EINTR)	                    /* interrupted -> restart read() */
                continue;
            else
                return -1;		                    /* some other error */
        } else if (!bytes_read) {	                /* EOF */
            if (!bytes_read_total)	                /* no byres read; return 0 */
                return 0;
            else
                break;
        } else {			                        /* bytes_read must be 1 if we get here*/
            if (ch == '\n' || ch == '\0')           /* break line or string end found, so line ends */
                break;
            if (bytes_read_total < n - 1) {		    /* discard > (n-1) bytes */
                bytes_read_total++;
                *buf++ = ch;
            }
        }
    }
    *buf = '\0';
    return bytes_read_total;
}
