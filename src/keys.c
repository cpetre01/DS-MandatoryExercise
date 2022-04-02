#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "utils.h"
#include "netUtils.h"
#include "keys.h"


int client_socket;      /* global client socket descriptor */


int open_socket(const char *host_name, const int port) {
    struct sockaddr_in server_addr;
    struct hostent *hp;

    /* create client socket */
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket < 0) {
        perror("Error creating socket"); return -1;
    }

    /* obtain server address */
    bzero((char*) &server_addr, sizeof server_addr);
    hp = gethostbyname(host_name);
    if (!hp) {
        perror("Error getting hostname"); return -1;
    }
    memcpy (&(server_addr.sin_addr), hp->h_addr, hp->h_length);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    /* connecting to server */
    if (connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server"); return -1;
    }
    fprintf(stderr, "Connection established with server!\n");
    return 0;
}


void close_socket(void) {
    close(client_socket);
}


int service(const char op_code, const int key, char *value1, int *value2, float *value3) {
    /* one-size-fits-all function that performs the required services;
     * can perform all 7 services given the proper arguments;
     * op_code determines the service */

    request_t request;  /* client request */
    request.header.op_code = op_code;
    reply_t reply;      /* server reply */

    /* send client request header */
    if (send_common_header(client_socket, &request.header) == -1) return -1;

    /* send key member if called service requires it */
    if (op_code == GET_VALUE || op_code == DELETE_KEY || op_code == EXIST ||
    op_code == SET_VALUE || op_code == MODIFY_VALUE) {
        request.item.key = key;
        if (send_key(client_socket, &request.item) == -1) return -1;
    }

    /* send value members if called service requires it */
    if (op_code == SET_VALUE || op_code == MODIFY_VALUE) {
        strncpy(request.item.value1, value1, VALUE1_MAX_STR_SIZE);
        request.item.value2 = *value2;
        request.item.value3 = *value3;
        if (send_values(client_socket, &request.item) == -1) return -1;
    }

    /* receive reply header */
    if (recv_reply_header(client_socket, &reply) == -1) return -1;

    /* receive rest of server reply & check it;
     * different actions depending on the called service */
    switch (op_code) {
        case GET_VALUE:
            /* receive rest of server reply */
            if (recv_values(client_socket, &reply.item) == -1) return -1;

            /* return the tuple values obtained from the DB */
            if (reply.server_error_code == SUCCESS) {
                strncpy(value1, reply.item.value1, VALUE1_MAX_STR_SIZE);
                *value2 = reply.item.value2;
                *value3 = reply.item.value3;
                return 0;
            }
            break;
        case EXIST:
            /* returns whether the tuple exists in the DB or not */
            if (reply.server_error_code == EXISTS) return 1;
            else if (reply.server_error_code == NOT_EXISTS) return 0;
            break;
        case NUM_ITEMS:
            /* receive rest of server reply */
            if (recv_num_items(client_socket, &reply) == -1) return -1;

            /* returns how many tuples there are in the DB */
            if (reply.server_error_code == SUCCESS) return (int) reply.num_items;
            break;
        default:    /* remaining services */
            if (reply.server_error_code == SUCCESS) return 0;
            break;
    } // end switch
    return -1;      /* server error, service was executed unsuccessfully */
}


/* client API functions call service function to perform their services;
 * they are just wrappers, really, since most services are very similar */

int init() {
    /* function used to initialize the DB */
    return service(INIT, 0, NULL, NULL, NULL);
}


int set_value(int key, char *value1, int value2, float value3) {
    /* function used to insert a tuple into the DB */
    return service(SET_VALUE, key, value1, &value2, &value3);
}


int get_value(int key, char *value1, int *value2, float *value3) {
    /* function used to read a tuple from the DB */
    return service(GET_VALUE, key, value1, value2, value3);
}


int modify_value(int key, char *value1, int value2, float value3) {
    /* function used to modify a tuple from the DB */
    return service(MODIFY_VALUE, key, value1, &value2, &value3);
}


int delete_key(int key) {
    /* function used to delete a tuple from the DB */
    return service(DELETE_KEY, key, NULL, NULL, NULL);
}


int exist(int key) {
    /* function used to figure out whether a tuple exists in the DB */
    return service(EXIST, key, NULL, NULL, NULL);
}


int num_items() {
    /* function used to figure out how many tuples are in the DB */
    return service(NUM_ITEMS, 0, NULL, NULL, NULL);
}
