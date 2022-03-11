#include <dirent.h>

DIR *open_db(void);
int read_db(void);
int write_item_to_db(int key, const char *value1, const int *value2, const float *value3);
int item_exists(int key);
int read_item_from_db(int key, char *value1, int *value2, float *value3);