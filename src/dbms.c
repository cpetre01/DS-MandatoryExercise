#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include "include/utils.h"
#include "include/dbmsUtils.h"
#include "include/dbms.h"


int db_list_db_items(void) {
    struct dirent *dir_ent;
    DIR *db = open_db();

    if (db == NULL) {
        perror("Could not open DB directory");
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


int db_get_num_items(void) {
    struct dirent *dir_ent;
    DIR *db = open_db();

    if (db == NULL) {
        perror("Could not open DB directory");
        return -1;
    }

    int num_items = 0;

    while ((dir_ent = readdir(db)) != NULL) {
        if (strcmp(dir_ent->d_name, ".") == 0 || strcmp(dir_ent->d_name, "..") == 0)
            continue;
        num_items++;
//        printf("%s\n", dir_ent->d_name);
    }

    closedir(db);
    return num_items;
}


int db_empty_db(void) {
    struct dirent *dir_ent;
    DIR *db = open_db();

    if (db == NULL) {
        perror("Could not open DB directory");
        return -1;
    }

    /* change to DB directory to manage inner files easily */
    chdir(DB_NAME);

    /* go through and delete all key files */
    while ((dir_ent = readdir(db)) != NULL) {
        if (strcmp(dir_ent->d_name, ".") == 0 || strcmp(dir_ent->d_name, "..") == 0)
            continue;
        if (remove(dir_ent->d_name) == -1) {
            perror("Couldn't delete entire DB");
            chdir("..");
            closedir(db);
            return -1;
        }
    }

    /* change back to executable's directory */
    chdir("..");
    closedir(db);
    return 0;
}


int db_item_exists(const int key) {
    /* open key file */
    int key_fd = open_key_file(key, READ);

    /* error if there is no file associated with that key */
    if (key_fd == -1) {
        /* key file doesn't exist */
        return 0;
    }

    /* key file was opened, so it exists */
    close(key_fd);
    return 1;
}


int db_read_item(const int key, char *value1, int *value2, float *value3) {
    errno = 0;
    /* open key file */
    int key_fd = open_key_file(key, READ);

    /* error if there is no file associated with that key */
    if (key_fd == -1) {
        /* key file doesn't exist */
        perror("Key file doesn't exist");
        return -1;
    }

    /* read value1 */
    if (read_value(key_fd, value1) == -1)
        return -1;

    /* read value2 */
    char value2_str[MAX_STR_SIZE];
    if (read_value(key_fd, value2_str) == -1)
        return -1;

    /* cast value2_str to int */
    if (cast_value(value2_str, (void *) value2, INT) == -1) {
        close(key_fd);
        return -1;
    }

    /* now read value3 */
    char value3_str[MAX_STR_SIZE];
    if (read_value(key_fd, value3_str) == -1)
        return -1;

    /* cast value3_str to float */
    if (cast_value(value3_str, (void *) value3, FLOAT) == -1) {
        close(key_fd);
        return -1;
    }

    /* all three values were read at this point, so close file and return */
    close(key_fd);
    return 0;
}


int db_write_item(const int key, const char *value1, const int *value2, const float *value3, const char mode) {
    if (mode != CREATE && mode != MODIFY) {
        perror("Invalid open file mode");
        return -1;
    }

    /* open key file */
    int key_fd = open_key_file(key, mode);

    /* error if there is no file associated with that key */
    if (key_fd == -1) {
        switch (errno) {
            /* set_value API call inserting existing key error */
            case EEXIST:
                perror("Key file already exists");
                return -1;
                break;
            default:
                perror("Error opening key file");
                return -1;
                break;
        }
    }

    /* write item to key file, one value per line */
    int result = write_values(key_fd, value1, value2, value3);

    /* all three values were written at this point, so close file and return */
    close(key_fd);
    return result;
}


int db_delete_item(const int key) {
    int exists = db_item_exists(key);
    if (exists == 0)            /* key file doesn't exist */
        return -1;
    else {                      /* key file does exist, so delete it */
        char key_file_name[MAX_STR_SIZE];
        sprintf(key_file_name, "%s/%d", DB_NAME, key);
        if (remove(key_file_name) == -1) {
            perror("Couldn't delete key file");
            return -1;
        }
        return 0;
    }
}
