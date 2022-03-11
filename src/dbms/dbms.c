#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include "dbms.h"

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
