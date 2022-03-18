/* client API
 * functions called by the client to perform services */
void open_server_q();
void open_client_q();
void close_server_q();
void close_client_q();
void init_name(char *name_client);
int init();
int set_value(int key, char *value1, int value2, float value3);
int get_value(int key, char *value1, const int *value2, const float *value3);
int modify_value(int key, char *value1, int value2, float value3);
int delete_key(int key);
int exist(int key);
int num_items();
