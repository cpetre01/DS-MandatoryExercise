#ifndef NETUTILS_H
#define NETUTILS_H

/* sending functions */
int send_common_header(int socket, header_t *header);
int send_reply_header(int client_socket, reply_t *reply);
int send_num_items(int client_socket, reply_t *reply);
int send_keys(int socket, item_t *item);
int send_item(int socket, item_t *item);

/* receiving functions */
int recv_request_header(int client_socket, request_t *request);
int recv_key(int socket, item_t *item);
int recv_item(int socket, item_t *item);


#endif //NETUTILS_H
