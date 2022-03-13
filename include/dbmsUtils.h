#include <stdio.h>
#include <dirent.h>


/* functions called internally in dbms module */
DIR *open_db(void);
int open_key_file(int key, char mode);
int read_value(int key_fd, char *value);
ssize_t read_line(int fd, void *buffer, size_t n);
int write_values(int key_fd, const char *value1, const int *value2, const float *value3);
