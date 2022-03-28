#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include "utils.h"
#include "keys.h"


mqd_t server_q;         /* queue for server */
mqd_t client_q;         /* queue for client */
struct mq_attr attr;
char client_q_name[MAX_STR_SIZE];


int open_server_q() {
    server_q = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if (server_q == -1) return -1;
    return 0;
}


int open_client_q() {
    client_q = mq_open(client_q_name, O_CREAT|O_RDONLY, 0666, &attr);
    if (client_q == -1) return -1;
    return 0;
}


int close_server_q() {
    if (mq_close(server_q) == -1) return -1;
    return 0;
}


int close_client_q() {
    if (mq_close(client_q) == -1) {
        mq_unlink(client_q_name); return -1;
    } else {
        if (mq_unlink(client_q_name) == -1) return -1;
        return 0;
    }
}


void client_queue_init(char *client_name) {
    /* function to create each client's queue;
     * it receives as argument the name of the client, and creates the queue name */
    snprintf(client_q_name, MAX_STR_SIZE, "%s_%s", CLIENT_QUEUE_NAME_TEMPLATE, client_name);

    /* set maximum number of allowed messages on the queue */
    attr.mq_maxmsg = MSG_QUEUE_SIZE;
    /* set maximum message size to be the size of the reply struct */
    attr.mq_msgsize = sizeof(reply_t);
}


int service(const char op_code, const int key, char *value1, int *value2, float *value3) {
    /* one-size-fits-all function that performs the required services;
     * can perform all 7 services given the proper arguments;
     * op_code determines the service */

    /* open both queues */
    if (open_server_q() == -1 || open_client_q() == -1) {
        perror(mq_open_error); return -1;
    }

    /* create client request */
    request_t request;
    request.op_code = op_code;
    strncpy(request.q_name, client_q_name, MAX_STR_SIZE);
    /* most services require the key as well */
    if (op_code == SET_VALUE || op_code == GET_VALUE || op_code == MODIFY_VALUE
    || op_code == DELETE_KEY || op_code == EXIST)
        request.item.key = key;
    /* these two require the values also */
    if (op_code == SET_VALUE || op_code == MODIFY_VALUE) {
        strncpy(request.item.value1, value1, VALUE1_MAX_STR_SIZE);
        request.item.value2 = *value2;
        request.item.value3 = *value3;
    }

    /* send client request to server */
    if (mq_send(server_q, (const char *) &request, sizeof(request_t), 0) < 0) {
        perror(send_request_error);
        if (close_server_q() == -1 || close_client_q() == -1) perror(mq_close_error);
        return -1;
    }

    /* receive server reply */
    reply_t reply;
    if (mq_receive(client_q, (char *) &reply, sizeof(reply_t), 0) < 0) {
        perror(receive_server_reply_error);
        if (close_server_q() == -1 || close_client_q() == -1) perror(mq_close_error);
        return -1;
    }

    /* close both queues */
    if (close_server_q() == -1 || close_client_q() == -1) {
        perror(mq_close_error); return -1;
    }

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
