#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include "include/utils.h"
#include "include/keys.h"


mqd_t server_q;         /* queue for server */
mqd_t client_q;         /* queue for client */
struct mq_attr attr;
char client_q_name[MAX_STR_SIZE];


int open_server_q() {
    /* initializing server queue */
    server_q = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if (server_q == -1)
        return -1;
    else
        return 0;
}


int open_client_q() {
    /* initializing client queue */
    client_q = mq_open(client_q_name, O_CREAT|O_RDONLY, 0666, &attr);
    if (client_q == -1)
        return -1;
    else
        return 0;
}


int close_server_q() {
    if (mq_close(server_q) == -1)
        return -1;
    else
        return 0;
}


int close_client_q() {
    if (mq_close(client_q) == -1) {
        mq_unlink(client_q_name);
        return -1;
    } else {
        if (mq_unlink(client_q_name) == -1) {
            return -1;
        }
        return 0;
    }

}


void client_queue_init(char *client_name) {
    /* function to create each client's queue */
    /* This function receives as argument the name of the client, and creates the queue name */
    strcpy(client_q_name,CLIENT_QUEUE_NAME_TEMPLATE);
    strcat(client_q_name, client_name);

    /* We will allow a maximum of 5 messages on the queue */
    attr.mq_maxmsg = MSG_QUEUE_SIZE;
    /* The maximum message size will be the maximum size of the reply struct */
    attr.mq_msgsize = sizeof(reply_t);
}


int init() {
    /* function used to initialize the queues and wait for replies */

    /* we open the queues */
    if (open_server_q() == -1 || open_client_q() == -1) {
        fprintf(stderr, "Error opening message queues\n");
        return -1;
    }

    /* We create and send the request */
    request_t request;
    request.op_code = INIT;
    strcpy(request.q_name, client_q_name);

    if(mq_send(server_q, (const char *) &request, sizeof(request_t), 0) < 0) {
        perror("Error sending request to server");
        /* Closing the queues */
        if (close_server_q() == -1 || close_client_q() == -1) {
            fprintf(stderr, "Error closing message queues\n");
            return -1;
        }
        return -1;
    }

    /* receive server reply */
    reply_t reply;

    if(mq_receive(client_q, (char *) &reply, sizeof(reply_t), 0) < 0) {
        perror("Error receiving reply from server");
        /* Closing the queues */
        if (close_server_q() == -1 || close_client_q() == -1) {
            fprintf(stderr, "Error closing message queues\n");
            return -1;
        }
        return -1;
    }

    /* close queues */
    if (close_server_q() == -1 || close_client_q() == -1) {
        fprintf(stderr, "Error closing message queues\n");
        return -1;
    }

    /* check server reply */
    if(reply.server_error_code == SUCCESS) {
        return 0;
    }else{
        /* Otherwise, there was an error, so we return -1 */
        return -1;
    }
}


int set_value(int key, char *value1, int value2, float value3) {
    /* we open the queues */
    if (open_server_q() == -1 || open_client_q() == -1) {
        fprintf(stderr, "Error opening message queues\n");
        return -1;
    }

    /* We create and set the struct */
    request_t request;
    request.op_code = SET_VALUE;
    request.item.key = key;
    strcpy(request.item.value1, value1);
    request.item.value2 = value2;
    request.item.value3 = value3;
    strcpy(request.q_name,client_q_name);

    if(mq_send(server_q, (const char *) &request, sizeof(request_t), 0) < 0) {
        perror("Error sending message to server");
        /* Closing the queues */
        if (close_server_q() == -1 || close_client_q() == -1) {
            fprintf(stderr, "Error closing message queues\n");
            return -1;
        }
        return -1;
    }
    /* wait for the reply of the server */
    reply_t reply;
    if(mq_receive(client_q, (char *) &reply, sizeof(reply_t), 0) < 0) {
        perror("Error receiving reply from server");
        /* Closing the queues */
        if (close_server_q() == -1 || close_client_q() == -1) {
            fprintf(stderr, "Error closing message queues\n");
            return -1;
        }
        return -1;
    }

    /* Close both queues */
    if (close_server_q() == -1 || close_client_q() == -1) {
        fprintf(stderr, "Error closing message queues\n");
        return -1;
    }

    /* Finally, we check the reply from the server: */
    if(reply.server_error_code == SUCCESS) {
        /* This means that the server included the message properly */
        return 0;
    }else{
        /* Otherwise, there was an error */
        return -1;
    }
}


int get_value(int key, char *value1, int *value2, float *value3) {
    /* we open the queues */
    if (open_server_q() == -1 || open_client_q() == -1) {
        fprintf(stderr, "Error opening message queues\n");
        return -1;
    }

    /* creating the struct*/
    request_t request;
    request.op_code = GET_VALUE;
    request.item.key = key;
    strcpy(request.q_name,client_q_name);

    if(mq_send(server_q, (const char *) &request, sizeof(request_t), 0) < 0) {
        perror("Error sending message to server");
        /* Closing the queues */
        if (close_server_q() == -1 || close_client_q() == -1) {
            fprintf(stderr, "Error closing message queues\n");
            return -1;
        }
        return -1;
    }

    /* wait for the reply of the server */
    reply_t reply;

    if(mq_receive(client_q, (char *) &reply, sizeof(reply_t), 0) < 0) {
        perror("Error receiving reply from server");
        /* Closing the queues */
        if (close_server_q() == -1 || close_client_q() == -1) {
            fprintf(stderr, "Error closing message queues\n");
            return -1;
        }
        return -1;
    }

    /* Close both queues */
    if (close_server_q() == -1 || close_client_q() == -1) {
        fprintf(stderr, "Error closing message queues\n");
        return -1;
    }

    /* Finally, we check the reply from the server: */
    if(reply.server_error_code == SUCCESS) {
        /* This means that the server included the message properly */
        strcpy(value1, reply.item.value1);
        *value2 = reply.item.value2;
        *value3 = reply.item.value3;
        return 0;
    }else{
        /* Otherwise, there was an error */
        return -1;
    }
}


int modify_value(int key, char *value1, int value2, float value3) {
    /* we open the queues */
    if (open_server_q() == -1 || open_client_q() == -1) {
        fprintf(stderr, "Error opening message queues\n");
        return -1;
    }

    /* creating the struct */
    request_t request;
    request.op_code = MODIFY_VALUE;
    request.item.key = key;
    strcpy(request.item.value1, value1);
    request.item.value2 = value2;
    request.item.value3 = value3;
    strcpy(request.q_name,client_q_name);

    if(mq_send(server_q, (const char *) &request, sizeof(request_t), 0) < 0) {
        perror("Error sending message to server");
        /* Closing the queues */
        if (close_server_q() == -1 || close_client_q() == -1) {
            fprintf(stderr, "Error closing message queues\n");
            return -1;
        }
        return -1;
    }

    /* wait for server response */
    reply_t reply;
    if(mq_receive(client_q, (char *) &reply, sizeof(reply_t), 0) < 0) {
        perror("Error receiving reply from server");
        /* Closing the queues */
        if (close_server_q() == -1 || close_client_q() == -1) {
            fprintf(stderr, "Error closing message queues\n");
            return -1;
        }
        return -1;
    }

    /* Closing queues */
    if (close_server_q() == -1 || close_client_q() == -1) {
        fprintf(stderr, "Error closing message queues\n");
        return -1;
    }

    /* Finally, we check the reply from the server: */
    if(reply.server_error_code == SUCCESS) {
        return 0;
    }else{
        return -1;
    }
}


int delete_key(int key) {
    /* opening the queues */
    if (open_server_q() == -1 || open_client_q() == -1) {
        fprintf(stderr, "Error opening message queues\n");
        return -1;
    }

    /* creating and sending the request */
    request_t request;
    request.op_code = DELETE_KEY;
    strcpy(request.q_name,client_q_name);
    request.item.key = key;

    /* send request to server */
    if(mq_send(server_q, (const char *) &request, sizeof(request_t), 0) < 0) {
        perror("Error sending request to server");
        /* Closing the queues */
        if (close_server_q() == -1 || close_client_q() == -1) {
            fprintf(stderr, "Error closing message queues\n");
            return -1;
        }
        return -1;
    }

    /* Wait for the response of the server */
    reply_t reply;

    if(mq_receive(client_q, (char *) &reply, sizeof(reply_t), 0) < 0) {
        perror("Error receiving message from server");
        /* Closing the queues */
        if (close_server_q() == -1 || close_client_q() == -1) {
            fprintf(stderr, "Error closing message queues\n");
            return -1;
        }
        return -1;
    }

    /* Close the queues: */
    if (close_server_q() == -1 || close_client_q() == -1) {
        fprintf(stderr, "Error closing message queues\n");
        return -1;
    }

    /* check value */
    if(reply.server_error_code == SUCCESS) {
        return 0;
    }else{
        /*error*/
        return -1;
    }
}


int exist(int key) {
    /* opening the queues */
    if (open_server_q() == -1 || open_client_q() == -1) {
        fprintf(stderr, "Error opening message queues\n");
        return -1;
    }

    /* creating and sending the request */
    request_t request;
    request.op_code = EXIST;
    strcpy(request.q_name,client_q_name);
    request.item.key = key;

    /* send request to sever */
    if(mq_send(server_q, (const char *) &request, sizeof(request_t), 0) < 0) {
        perror("Error sending request to server");
        /* Closing the queues */
        if (close_server_q() == -1 || close_client_q() == -1) {
            fprintf(stderr, "Error closing message queues\n");
            return -1;
        }
        return -1;
    }

    /* Wait for the response of the server */
    reply_t reply;

    if(mq_receive(client_q, (char *) &reply, sizeof(reply_t), 0) < 0) {
        perror("Error receiving message from server");
        /* Closing the queues */
        if (close_server_q() == -1 || close_client_q() == -1) {
            fprintf(stderr, "Error closing message queues\n");
            return -1;
        }
        return -1;
    }

    /* Close the queues: */
    if (close_server_q() == -1 || close_client_q() == -1) {
        fprintf(stderr, "Error closing message queues\n");
        return -1;
    }

    /* check value */
    if(reply.server_error_code == EXISTS) {
        return 1;
    }
    else if(reply.server_error_code == NOT_EXISTS){
        /*error*/
        return 0;
    }
}


int num_items() {
    /* opening the queues */
    if (open_server_q() == -1 || open_client_q() == -1) {
        fprintf(stderr, "Error opening message queues\n");
        return -1;
    }

    /* creating and sending the request*/
    request_t request;
    request.op_code = NUM_ITEMS;
    strcpy(request.q_name,client_q_name);

    if(mq_send(server_q, (const char *) &request, sizeof(request_t), 0) < 0) {
        perror("Error sending request to server");
        /* Closing the queues */
        if (close_server_q() == -1 || close_client_q() == -1) {
            fprintf(stderr, "Error closing message queues\n");
            return -1;
        }
        return -1;
    }

    /* Waiting for server reply */
    reply_t reply;

    if(mq_receive(client_q, (char *) &reply, sizeof(reply_t), 0) < 0) {
        perror("Error receiving reply from server");
        /* Closing the queues */
        if (close_server_q() == -1 || close_client_q() == -1) {
            fprintf(stderr, "Error closing message queues\n");
            return -1;
        }
        return -1;
    }

    /* Closing the queues */
    if (close_server_q() == -1 || close_client_q() == -1) {
        fprintf(stderr, "Error closing message queues\n");
        return -1;
    }

    if(reply.server_error_code == ERROR) {
        return -1;
    } else {
        int num_items;
        memcpy(&num_items, &reply.num_items, sizeof(int));
        return num_items;
    }
}
