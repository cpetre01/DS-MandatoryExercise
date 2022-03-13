#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include "include/dbms/dbms.h"

#define MAXSIZE 255

int write_values(int key_fd, const char *value1, const int *value2, const float *value3);

DIR *open_db(void)
{
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


int list_db_entries(void)
{
    struct dirent *dir_ent;
    DIR *db = open_db();

    if (db == NULL) {
        perror("Could not open db directory");
        return -1;
    }

    while ((dir_ent = readdir(db)) != NULL) {
        if (strcmp(dir_ent->d_name, ".") == 0 || strcmp(dir_ent->d_name, "..") == 0)
            continue;
        printf("%s\n", dir_ent->d_name);
    }

    closedir(db);
    return 0;
}


int empty_db()
{
    struct dirent *dir_ent;
    DIR *db = open_db();

    if (db == NULL) {
        perror("Could not open db directory");
        return -1;
    }

    while ((dir_ent = readdir(db)) != NULL) {
        if (strcmp(dir_ent->d_name, ".") == 0 || strcmp(dir_ent->d_name, "..") == 0)
            continue;
        char db_entry_name[MAXSIZE];
        sprintf(db_entry_name, "db/%s", dir_ent->d_name);
        remove(db_entry_name);
    }

    closedir(db);
    return 0;
}


int open_key_file(const int key, const char mode)
{
    int key_fd;
    char key_str[MAXSIZE];
    sprintf(key_str, "db/%d", key);

    /* open key file */
    if (mode == 'r')
        key_fd = open(key_str, O_RDONLY);
    else if (mode == 'w')
        key_fd = open(key_str, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    else if (mode == 'e')
        key_fd = open(key_str, O_WRONLY | O_TRUNC);
    else
        return -1;

    return key_fd;
}


int item_exists(const int key)
{
    /* open item file */
    int key_fd = open_key_file(key, 'r');

    /* error if there is no file associated with that key */
    if (key_fd == -1){
        /* key file doesn't exist */
        return 0;
    }

    /* key file was opened, so it exists */
    close(key_fd);
    return 1;
}


int read_item(const int key, char *value1, int *value2, float *value3)
{
    errno = 0;
    /* open key file */
    int key_fd = open_key_file(key, 'r');

    /* error if there is no file associated with that key */
    if (key_fd == -1){
        /* key file doesn't exist */
        perror("Key file doesn't exist");
        return -1;
    }

    /* start reading key file */
    ssize_t bytes_read;     /* used for error handling of read_line calls */

    /* now read value1 */
    bytes_read = read_line(key_fd, value1, MAXSIZE);
    if (bytes_read == -1) {
        perror("Error reading line");
        close(key_fd);
        return -1;
    } else if (bytes_read == 0) {
        printf("Nothing was read\n");
        close(key_fd);
        return -1;
    }

    /* now read value2 */
    char value2_str[MAXSIZE];
    bytes_read = read_line(key_fd, value2_str, MAXSIZE);
    if (bytes_read == -1) {
        perror("Error reading line");
        close(key_fd);
        return -1;
    } else if (bytes_read == 0) {
        printf("Nothing was read\n");
        close(key_fd);
        return -1;
    }

    /* cast value2_str to int */
    char * endptr;      /* used for strtol and strtof castings */

    int value2_str_to_int = (int)strtol(value2_str, &endptr, 10);
    /* check cast errors */
    if (errno != 0) {
        perror("strtol");
        close(key_fd);
        return -1;
    }
    if (endptr == value2_str) {
        fprintf(stderr, "No digits were found\n");
        close(key_fd);
        return -1;
    }

    /* cast succeeded: set value2 */
    *value2 = value2_str_to_int;

    /* now read value3 */
    char value3_str[MAXSIZE];
    bytes_read = read_line(key_fd, value3_str, MAXSIZE);
    if (bytes_read == -1) {
        perror("Error reading line");
        close(key_fd);
        return -1;
    } else if (bytes_read == 0) {
        printf("Nothing was read\n");
        close(key_fd);
        return -1;
    }

    /* cast value3_str to float */
    float value3_str_to_float = (float)strtof(value3_str, &endptr);
    /* check cast errors */
    if (errno != 0) {
        perror("strtof");
        close(key_fd);
        return -1;
    }
    if (endptr == value2_str) {
        fprintf(stderr, "No digits were found\n");
        close(key_fd);
        return -1;
    }

    /* cast succeeded: set value3 */
    *value3 = value3_str_to_float;

    /* all three values were read at this point, so close file and return */
    close(key_fd);
    return 1;
}


int write_item(const int key, const char *value1, const int *value2, const float *value3)
{
    /* open key file */
    int key_fd = open_key_file(key, 'w');

    /* error if there is no file associated with that key */
    if (key_fd == -1){
        perror("Error opening key file");
        return -1;
    }

    /* write item to key file, one value per line */
    int result = write_values(key_fd, value1, value2, value3);

    /* all three values were written at this point, so close file and return */
    close(key_fd);
    return result;
}


int modify_item(int key, const char *value1, const int *value2, const float *value3)
{
    /* open key file */
    int key_fd = open_key_file(key, 'e');

    /* error if there is no file associated with that key */
    if (key_fd == -1){
        perror("Key file doesn't exist");
        return -1;
    }
    /* write item to key file, one value per line */
    int result = write_values(key_fd, value1, value2, value3);

    /* all three values were written at this point, so close file and return */
    close(key_fd);
    return result;
}

int write_values(int key_fd, const char *value1, const int *value2, const float *value3)
{
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


int delete_item(const int key)
{
    int exists = item_exists(key);
    if (exists == 0)            /* key file doesn't exist */
        return -1;
    else {                      /* key file does exist, so delete it */
        char key_file_name[MAXSIZE];
        sprintf(key_file_name, "db/%d", key);
        remove(key_file_name);
        return 0;
    }
}


ssize_t read_line(const int fd, void *buffer, const size_t n)
{
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
