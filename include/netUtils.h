#ifndef NETUTILS_H
#define NETUTILS_H

#define MAX_CONN_BACKLOG 5          /* max number of open client connections */

/* sending functions */
int send_common_header(int socket, header_t *header);
int send_reply_header(int socket, reply_t *reply);
int send_num_items(int socket, reply_t *reply);
int send_keys(int socket, item_t *item);
int send_item(int socket, item_t *item);

/* receiving functions */
int recv_common_header(int client_socket, header_t *header);
int recv_reply_header(int socket, reply_t *reply);
int recv_key(int socket, item_t *item);
int recv_item(int socket, item_t *item);


#endif //NETUTILS_H