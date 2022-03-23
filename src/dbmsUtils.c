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

    /* open DB directory */
    DIR *db = opendir(DB_NAME);
    if (db == NULL) {
        switch (errno) {
            case ENOENT:
                /* create it if it doesn't exist*/
                if (mkdir(DB_NAME, S_IRWXU) == -1) {
                    perror("DB directory could not be created");
                    return NULL;
                }
                /* and open it */
                db = opendir(DB_NAME);
                if (db == NULL) {
                    perror("Could not open DB directory after creating it");
                    return NULL;
                }
                break;
            default:
                perror("Could not open DB directory");
                return NULL;
        }
    }
    return db;
}


int open_keyfile(const int key, const char mode) {
    int key_fd;
    char key_str[MAX_STR_SIZE];
    snprintf(key_str, MAX_STR_SIZE, "%s/%d", DB_NAME, key);

    /* open key file */
    if (mode == READ)
        key_fd = open(key_str, O_RDONLY);
    else if (mode == CREATE)
        key_fd = open(key_str, O_WRONLY | O_CREAT | O_EXCL, 0600);
    else if (mode == MODIFY)
        key_fd = open(key_str, O_WRONLY | O_TRUNC);
    else
        return -1;

    return key_fd;
}


int read_value_from_keyfile(const int key_fd, char *value) {
    ssize_t bytes_read;     /* used for error handling of read_line calls */

    /* read value */
    bytes_read = read_line(key_fd, value, MAX_STR_SIZE);
    if (bytes_read == -1) {
        perror("Error reading line");
        close(key_fd);
        return -1;
    } else if (!bytes_read) {
        fprintf(stderr, "Nothing was read\n");
        close(key_fd);
        return -1;
    }
    return 0;
}


int write_values_to_keyfile(const int key_fd, const char *value1, const int *value2, const float *value3) {
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
