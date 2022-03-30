#ifndef KEYS_H
#define KEYS_H

/* common error messages */
const char mq_open_error[] = "Error opening message queues";
const char mq_close_error[] = "Error closing message queues";
const char receive_server_reply_error[] = "Error receiving reply from server";
const char send_request_error[] = "Error sending request to server";


/* internal functions used to work with queues */
int open_server_q();
int open_client_q();
int close_server_q();
int close_client_q();
void init_connection(const char *host_name, int port_number);

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