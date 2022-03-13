#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include "include/utils.h"
#include "include/keys.h"


mqd_t server_q;  /*queue for server */
mqd_t client_q;  /*queue for client*/
struct mq_attr attr;
char *client_q_name;


/*initializing server queue*/
void open_server_q() {
    server_q = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
}


/*initializing client queue*/
void open_client_q() {
    client_q = mq_open(client_q_name, O_CREAT|O_RDONLY, 0777, &attr);
}


void close_server_q() {
    mq_close(server_q);
}


void close_client_q() {
    mq_close(client_q);
}


/*function to create each client's queue */
void init_name(char *name_client) {
    /*This function receives a parameter the name of the client, and creates the queue name */
    client_q_name = malloc(strlen(CLIENT_QUEUE_NAME_TEMPLATE) + strlen(name_client) + 1);
    strcpy(client_q_name,CLIENT_QUEUE_NAME_TEMPLATE);
    strcat(client_q_name, name_client);
    /*We will allow a maximum of 5 messages on the queue*/
    attr.mq_maxmsg = MSG_QUEUE_SIZE;
    /*The maximum message size will be the maximum size of the replies struct*/
    attr.mq_msgsize = sizeof(struct reply);
}


int init() {
    /* function used to initialize the queues and wait for the responses*/
    open_server_q();
    open_client_q();

    /* We create and send the request*/
    struct request req;
    req.op_code = INIT;
    strcpy(req.q_name, client_q_name);
    if(mq_send(server_q, (const char *) &req, sizeof(struct request), 0) < 0) {
        perror("Error sending request to server");
    }

    /*wait for receiving a reply*/
    struct reply rep;

    if(mq_receive(client_q, (char *) &rep, sizeof(struct reply), 0) < 0) {
        perror("Error receiving reply from server");
    }

    /*close the queues*/
    close_server_q();
    close_client_q();

    /*check replies*/
    if(rep.server_error_code == SUCCESS) {
        /* 0 means no errors*/
        return 0;
    }else{
        /* Otherwise, there was an error, so we return -1*/
        return -1;
    }

}


int set_value(int key, char *value1, int value2, float value3) {
    /* we open the queues */
    open_server_q();
    open_client_q();
    /*We create and set the struct*/
    struct request req;
    req.op_code = SET_VALUE;
    memcpy(&req.item->key, &key, sizeof(int));
    strcpy(req.item->value1, value1);
    memcpy(&req.item->value2, &value2, sizeof(int));
    memcpy(&req.item->value3, &value3, sizeof(float));
    strcpy(req.q_name,client_q_name);
    if(mq_send(server_q, (const char *) &req, sizeof(struct request), 0) < 0) {
        perror("Error sending message to server");
    }
    /*wait for the rep of the server*/
    struct reply rep;
    if(mq_receive(client_q, (char *) &rep, sizeof(struct reply), 0) < 0) {
        perror("Error receiving reply from server");
    }
    /*Close both queues*/
    close_server_q();
    close_client_q();

    /*Finally, we check the rep from the server:*/
    if(rep.server_error_code == SUCCESS) {
        /*This means that the server included the message properly*/
        return 0;
    }else{
        /*Otherwise, there was an error*/
        return -1;
    }
}


int get_value(int key, char *value1, int *value2, float *value3) {
    /* we open the queues */
    open_server_q();
    open_client_q();
    /* creating the struct*/
    struct request req;
    req.op_code = GET_VALUE;
    memcpy(&req.item->key, &key, sizeof(int));
    strcpy(req.q_name,client_q_name);
    if(mq_send(server_q, (const char *) &req, sizeof(struct request), 0) < 0) {
        perror("Error sending message to server");
    }
    /*wait for the rep of the server*/
    struct reply rep;
    if(mq_receive(client_q, (char *) &rep, sizeof(struct reply), 0) < 0) {
        perror("Error receiving reply from server");
    }

    /*Finally, we check the rep from the server:*/
    if(rep.server_error_code == SUCCESS) {
        /*This means that the server included the message properly*/
        strcpy(value1, rep.item->value1);
        memcpy(&rep.item->value2, &value2, sizeof(int));
        memcpy(&rep.item->value3, &value3, sizeof(float));
    }else{
        /*Otherwise, there was an error*/
        close_server_q();
        close_client_q();
        return -1;
    }

    /*Closing both queues*/
    close_server_q();
    close_client_q();
    return 0;
}


int modify_value(int key, char *value1, int value2, float value3) {
    struct request req;
    req.op_code = MODIFY_VALUE;

    /*opening queues*/
    open_server_q();
    open_server_q();

    memcpy(&req.item->key, &key, sizeof(int));
    strcpy(req.item->value1, value1);
    memcpy(&req.item->value2, &value2, sizeof(int));
    memcpy(&req.item->value3, &value3, sizeof(float));
    strcpy(req.q_name,client_q_name);


    if(mq_send(server_q, (const char *) &req, sizeof(struct request), 0) < 0) {
        perror("Error sending message to server");
    }
    /*wait for server response*/
    struct reply rep;
    if(mq_receive(client_q, (char *) &rep, sizeof(struct reply), 0) < 0) {
        perror("Error receiving reply from server");
    }
    /*Closing queues*/
    close_server_q();
    close_client_q();

    /*Finally, we check the reply from the server:*/
    if(rep.server_error_code == SUCCESS) {
        return 0;
    }else{
        return -1;
    }
}


int delete_key(int key) {

    /*opening the queues*/
    open_server_q();
    open_client_q();
    /* creating and sending the request*/
    struct request req;
    req.op_code = DELETE_KEY;
    strcpy(req.q_name,client_q_name);
    memcpy(&req.item->key, &key, sizeof(int));

    /* send request to server*/
    if(mq_send(server_q, (const char *) &req, sizeof(struct request), 0) < 0) {
        perror("Error sending request to server");
    }

    /*Wait for the response of the server*/
    struct reply rep;
    if(mq_receive(client_q, (char *) &rep, sizeof(struct reply), 0) < 0) {
        perror("Error receiving message from server");
    }

    /*Close the queues:*/
    close_server_q();
    close_client_q();

    /* check value */
    if(rep.server_error_code == SUCCESS) {
        /*no error*/
        return 0;
    }else{
        /*error*/
        return -1;
    }
}


int exist(int key) {

    /*opening the queues*/
    open_server_q();
    open_client_q();
    /* creating and sending the request*/
    struct request req;
    req.op_code = EXIST;
    strcpy(req.q_name,client_q_name);
    memcpy(&req.item->key, &key, sizeof(int));

    /* send request to sever */
    if(mq_send(server_q, (const char *) &req, sizeof(struct request), 0) < 0) {
        perror("Error sending request to server");
    }

    /*Wait for the response of the server*/
    struct reply rep;
    if(mq_receive(client_q, (char *) &rep, sizeof(struct reply), 0) < 0) {
        perror("Error receiving message from server");
    }

    /*Close the queues:*/
    close_server_q();
    close_client_q();

    /* check value */
    if(rep.server_error_code == SUCCESS) {
        /* no error */
        return 0;
    }else{
        /*error*/
        return -1;
    }
}


int num_items() {
    /*opening the queues*/
    open_server_q();
    open_client_q();
    /* creating and sending the request*/

    struct request req;
    req.op_code = NUM_ITEMS;
    strcpy(req.q_name,client_q_name);

    if(mq_send(server_q, (const char *) &req, sizeof(struct request), 0) < 0) {
        perror("Error sending request to server");
    }

    /*Waiting for server reply*/
    struct reply rep;
    if(mq_receive(client_q, (char *) &rep, sizeof(struct reply), 0) < 0) {
        perror("Error receiving reply from server");
    }

    int num_items;
    if(rep.server_error_code == ERROR) {
        /*Closing the queues*/
        close_server_q();
        close_client_q();
        return -1;
    } else {
        memcpy(&num_items, &rep.num_items, sizeof(int));
    }

    /*Closing the queues*/
    close_server_q();
    close_client_q();

    return num_items;
}
