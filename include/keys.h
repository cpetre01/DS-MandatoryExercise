/* internal functions used to work with queues */
int open_server_q();
int open_client_q();
int close_server_q();
int close_client_q();
void client_queue_init(char *client_name);


/* client API:
 * functions called by the client to perform services */
int init();
int set_value(int key, char *value1, int value2, float value3);
int get_value(int key, char *value1, int *value2, float *value3);
int modify_value(int key, char *value1, int value2, float value3);
int delete_key(int key);
int exist(int key);
int num_items();
