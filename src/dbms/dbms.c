#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include "include/dbms/dbms.h"

#define MAXSIZE 255

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


int read_db(void) {
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


int write_item_to_db(const int key, const char *value1, const int *value2, const float *value3) {
    /* open item file */
    FILE *key_fd;
    char key_str[MAXSIZE];
    sprintf(key_str, "%d", key);
    key_fd = fopen(key_str, "w");
    /* error if there is no file associated with that key */
    if (key_fd == NULL){
        perror("Error opening key file");
        return -1;
    }

    /* write item to it */
    fprintf(key_fd, "%s\n", value1);
    fprintf(key_fd, "%d\n", *value2);
    fprintf(key_fd, "%f\n", *value3);

    fclose(key_fd);
    return 0;
}


int item_exists(const int key) {
    /* open item file */
    FILE *key_fd;
    char key_str[MAXSIZE];
    sprintf(key_str, "%d", key);
    key_fd = fopen(key_str, "r");
    /* error if there is no file associated with that key */
    if (key_fd == NULL){
        /* key file doesn't exist */
        return 0;
    }
    /* key file was opened, so it exists */
    fclose(key_fd);
    return 1;

}


int read_item_from_db(int key, char *value1, int *value2, float *value3) {
    /* open item file */
    FILE *key_fd;
    char key_str[MAXSIZE];
    sprintf(key_str, "%d", key);
    key_fd = fopen(key_str, "r");
    /* error if there is no file associated with that key */
    if (key_fd == NULL){
        /* key file doesn't exist */
        perror("Error opening key file");
        return -1;
    }
    /* now read stuff */


    fclose(key_fd);
    return 1;

}