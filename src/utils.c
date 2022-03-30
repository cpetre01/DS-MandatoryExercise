#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "utils.h"


int str_to_num(const char *value_str, void *value, const char type) {
    /* function that casts a given string value_str to a number;
     * writes cast value to value pointer; this value pointer
     * should be a pointer to the desired number type.
     * Currently supported types: int, float */
    errno = 0;

    char * endptr;                  /* used for castings */
    char error[MAX_STR_SIZE];       /* used to display errors */
    void *cast_value;               /* this points to cast value */
    size_t result_size;             /* used to determine the size of the cast value in memcpy call */

    /* auxiliary variables to cast value */
    int value_str_to_int;
    float value_str_to_float;

    if (!strlen(value_str)) {
        fprintf(stderr, "String is empty\n"); return -1;
    }

    /* cast value */
    switch (type) {
        case INT:
            value_str_to_int = (int)strtol(value_str, &endptr, 10);
            cast_value = (void *) &value_str_to_int;
            strncpy(error, "strtol", MAX_STR_SIZE);
            result_size = sizeof(int);
            break;
        case FLOAT:
            value_str_to_float = strtof(value_str, &endptr);
            cast_value = (void *) &value_str_to_float;
            strncpy(error, "strtof", MAX_STR_SIZE);
            result_size = sizeof(float);
            break;
        default: fprintf(stderr, "Invalid casting type\n"); return -1;
    }

    /* check casting errors */
    if (errno) {
        perror(error); return -1;
    }
    if (endptr == value_str) {
        fprintf(stderr, "No digits were found\n"); return -1;
    }

    /* cast succeeded: set value */
    memcpy(value, cast_value, result_size);
    return 0;
}


ssize_t read_line(const int fd, void *buffer, const size_t buf_space) {
    /* reads a line of at most (buf_space -1) chars,
     * excluding '\0' or '\n' terminating byte */
    ssize_t bytes_read;             /* number of bytes fetched by last read() */
    ssize_t bytes_read_total;       /* total bytes read so far */
    char *buf, ch;

    /* check arguments */
    if (buf_space <= 0 || buffer == NULL) {
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
            if (!bytes_read_total)	                /* no bytes read; return 0 */
                return 0;
            else
                break;
        } else {			                        /* bytes_read must be 1 if we get here */
            if (ch == '\n' || ch == '\0')           /* break line or string end found, so line ends */
                break;
            if (bytes_read_total < buf_space - 1) {		    /* discard > (n-1) bytes */
                bytes_read_total++;
                *buf++ = ch;
            }
        }
    }
    *buf = '\0';
    return bytes_read_total;
}


/* not used, currently not working */
ssize_t read_line_opt(const int fd, void *buffer, const size_t buf_space) {
    /* reads a line of at most (buf_space -1) chars,
     * excluding '\0' or '\n' terminating byte */
    ssize_t bytes_read;             /* number of bytes fetched by last read() */
    ssize_t bytes_read_total;       /* total bytes read so far */
    size_t bytes_left = buf_space;  /* number of bytes left to be read */
    char t[buf_space];                  /* temporary buffer to read from fd */
    char *tmp_buf = t;
    char *final_buf;                /* buffer to store read bytes */

    /* check arguments */
    if (buf_space <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }

//    tmp_buf = (char *) malloc(buf_space + 1);
//    if (!tmp_buf) {
//        fprintf(stderr, "Malloc error\n"); return -1;
//    }
    final_buf = buffer;
    bytes_read_total = 0;

    /* read stuff and buffer it */
    while(TRUE) {
        bytes_read = read(fd, tmp_buf, bytes_left);

        /* check what's been read */
        if (bytes_read == -1) {
            if (errno == EINTR)	                    /* interrupted -> restart read() */
                continue;
            else
                return -1;		                    /* some other error */
        } else if (!bytes_read) {	                /* EOF */
            if (bytes_left == buf_space)	        /* no bytes read; return 0 */
                return 0;
            else
                break;
        } else {                                    /* something was read */
            bytes_left -= bytes_read;
            bytes_read_total += bytes_read;
            tmp_buf += bytes_read;
        }
    }

    /* make tmp_buf point to the beginning of the space */
    tmp_buf -= bytes_read_total;

    /* now let's transfer the stuff to final_buf */
    for (int i = 0; i < bytes_read_total; i++) {
        if (tmp_buf[i] == '\n' || tmp_buf[i] == '\0')   /* break line or string end found, so line ends */
            break;
        if (bytes_read_total < buf_space - 1) {		            /* discard > (n-1) bytes */
            bytes_read_total++;
            *final_buf++ = tmp_buf[i];
        }
    }

//    free(tmp_buf);
    *final_buf = '\0';
    return bytes_read_total;
}
