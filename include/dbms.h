#include <stdio.h>
#include <dirent.h>

DIR *open_db(void);
int open_key_file(int key, char mode);
ssize_t read_line(int fd, void *buffer, size_t n);
int write_values(int key_fd, const char *value1, const int *value2, const float *value3);
/* callable functions */
int list_db_entries(void);
int empty_db();
int item_exists(int key);
int read_item(int key, char *value1, int *value2, float *value3);
int write_item(int key, const char *value1, const int *value2, const float *value3);
int modify_item(int key, const char *value1, const int *value2, const float *value3);
int delete_item(int key);
