#ifndef KEYS_H
#define KEYS_H

/* common error messages */
const char receive_server_reply_error[] = "Error receiving reply from server";
const char send_request_error[] = "Error sending request to server";


/* internal functions used to work with queues */
int init_connection(const char *host_name, const int port);
void close_connection(void);

/* one-size-fits-all function that performs the required services;
 * can perform all 7 services given the proper arguments;
 * op_code determines the service */
int service(char op_code, int key, char *value1, int *value2, float *value3);


/* client API:
 * functions called by the client to perform services */
int init();
int set_value(int key, char *value1, int value2, float value3);
int get_value(int key, char *value1, int *value2, float *value3);
int modify_value(int key, char *value1, int value2, float value3);
int delete_key(int key);
int exist(int key);
int num_items();

#endif //KEYS_H