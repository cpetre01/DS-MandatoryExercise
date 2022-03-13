#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include "include/utils.h"
#include "include/dbmsUtils.h"


DIR *open_db(void) {
    errno = 0;
    char db_dir_name[5] = "db";

    /* open db directory */
    DIR *db = opendir(db_dir_name);
    if (db == NULL) {
        switch (errno) {
            case ENOENT:
                /* create it if it doesn't exist*/
                if (mkdir(db_dir_name, S_IRWXU) == -1) {
                    perror("db directory could not be created");
                    return NULL;
                }
                /* and open it */
                db = opendir(db_dir_name);
                if (db == NULL) {
                    perror("Could not open db directory after creating it");
                    return NULL;
                }
                break;
            default:
                perror("Could not open db directory");
                return NULL;
        }
    }
    return db;
}


int open_key_file(const int key, const char mode) {
    int key_fd;
    char key_str[MAX_STR_SIZE];
    sprintf(key_str, "db/%d", key);

    /* open key file */
    if (mode == READ)
        key_fd = open(key_str, O_RDONLY);
    else if (mode == CREATE)
        key_fd = open(key_str, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    else if (mode == MODIFY)
        key_fd = open(key_str, O_WRONLY | O_TRUNC);
    else
        return -1;

    return key_fd;
}


int read_value(const int key_fd, char *value) {
    ssize_t bytes_read;     /* used for error handling of read_line calls */

    /* read value */
    bytes_read = read_line(key_fd, value, MAX_STR_SIZE);
    if (bytes_read == -1) {
        perror("Error reading line");
        close(key_fd);
        return -1;
    } else if (bytes_read == 0) {
        printf("Nothing was read\n");
        close(key_fd);
        return -1;
    }
    return 0;
}


ssize_t read_line(const int fd, void *buffer, const size_t n) {
    ssize_t bytes_read;             /* num of bytes fetched by last read() */
    ssize_t bytes_read_total;       /* total bytes read so far */
    char *buf;
    char ch;

    /* check arguments */
    if (n <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }

    buf = buffer;
    bytes_read_total = 0;
    /* read from fd */
    for (;;) {
        bytes_read = read(fd, &ch, 1);	/* read a byte */
        /* check what's been read */
        if (bytes_read == -1) {
            if (errno == EINTR)	                    /* interrupted -> restart read() */
                continue;
            else
                return -1;		                    /* some other error */
        } else if (bytes_read == 0) {	            /* EOF */
            if (bytes_read_total == 0)	            /* no byres read; return 0 */
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


int write_values(int key_fd, const char *value1, const int *value2, const float *value3) {
    /* write item to key file, one value per line */
    if (dprintf(key_fd, "%s\n", value1) < 0) {
        fprintf(stderr, "Could not write value1\n");
        close(key_fd);
        return -1;
    }
    if (dprintf(key_fd, "%d\n", *value2) < 0) {
        fprintf(stderr, "Could not write value2\n");
        close(key_fd);
        return -1;
    }
    if (dprintf(key_fd, "%f\n", *value3) < 0) {
        fprintf(stderr, "Could not write value3\n");
        close(key_fd);
        return -1;
    }
    return 0;
}
