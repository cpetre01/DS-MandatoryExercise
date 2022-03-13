/* functions called by the server to manage the DB */
int list_db_entries(void);
int empty_db();
int item_exists(int key);
int read_item(int key, char *value1, int *value2, float *value3);
int write_item(int key, const char *value1, const int *value2, const float *value3, char mode);
int delete_item(int key);
