#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "utils.h"
#include "keys.h"
#include "netUtils.h"


int client_socket;


void init_connection(const char *host_name, const int port_number) {
    struct sockaddr_in server_addr;
    struct hostent *hp;

    /* creating the socket */
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket < 0){
        perror("Error while creating socket");
    }

    /*obtain the server address */
    bzero((char*)&server_addr, sizeof(server_addr));
    hp = gethostbyname(host_name);
    if (hp == NULL){
        perror("Error getting hostname");
    }
    memcpy (&(server_addr.sin_addr), hp->h_addr, hp->h_length);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);

    //connecting with the server
    int err = connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if(err >= 0){
        printf("Connection established with server!\n");
    }else{
        perror("Error connecting to server");
    }
}


int close_connection() {
    close(client_socket);
    return 0;
}


int service(const char op_code, const int key, char *value1, int *value2, float *value3) {
    /* one-size-fits-all function that performs the required services;
     * can perform all 7 services given the proper arguments;
     * op_code determines the service */


    /* create client request */
    request_t request;
    reply_t reply;
    request.header.op_code = op_code;
    /* send client request*/
    if(send_common_header(client_socket, &request) < 0){
        perror("Error sending header ");
    }

    /*receive the response for the header */
    if(recv_common_header(client_socket, &reply) < 0){
        perror("Error sending header ");
    }

    /* for some methods, we also need to send the key*/
    if (op_code == SET_VALUE || op_code == GET_VALUE || op_code == MODIFY_VALUE
        || op_code == DELETE_KEY || op_code == EXIST){
        request.item.key = key;
        if(send_keys(client_socket, &key) < 0){
            perror("Error sending key");
        }

        /*now, we receive the response from the server*/
        if(recv_key(client_socket, &request.item) < 0){
            perror("Error receiving response for header");
        }
    }

    /* here, we also need to send the values */
    if (op_code == SET_VALUE || op_code == MODIFY_VALUE) {
        strncpy(request.item.value1, value1, VALUE1_MAX_STR_SIZE);
        request.item.value2 = *value2;
        request.item.value3 = *value3;
        if(send_item(client_socket, &reply.item) < 0){
            perror("Error sending header ");
        }

        /*receive response from the server*/
        if(recv_item(client_socket, &reply.item) < 0){
            perror("Error receiving response for items ");
        }
    }

    /*close client socket*/
    close_connection(client_socket);

    /* check server reply, different actions depending on the called service */
    switch (op_code) {
        case GET_VALUE:
            /* this returns the tuple values obtained from the DB */
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
            /* returns how many tuples there are in the DB */
            if (reply.server_error_code == SUCCESS) {
                int num_items;
                memcpy(&num_items, &reply.num_items, sizeof(int));
                return num_items;
            }
            break;
        default:    /* remaining services */
            if (reply.server_error_code == SUCCESS) return 0;
            break;
    } // end switch
    return -1;      /* server error */
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
