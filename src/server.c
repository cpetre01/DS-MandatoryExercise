#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <mqueue.h>
#include "message.h"
#define MSG_QUEUE_SIZE 5

mqd_t request_queue;
mqd_t response_queue;


void main(void) {
    struct mq_attr attr;
    attr.mq_maxmsg = MSG_QUEUE_SIZE;
    attr.mq_msgsize = sizeof(struct message);

    /* create request queue */
    char request_queue_name[25];
    sprintf(request_queue_name, "/server/request_queue");
    request_queue = mq_open(request_queue_name, O_CREAT | O_RDONLY, 0700, &attr);
    if (request_queue == -1) {
        perror("mq_open");
        exit(-1);
    }

    char response_queue_name[25];
    sprintf(response_queue_name, "/server/response_queue");
    response_queue = mq_open(response_queue_name, O_CREAT | O_WRONLY, 0700, &attr);
    if (response_queue == -1) {
        perror("mq_open");
        exit(-1);
    }


    mq_close(request_queue);
    mq_close(response_queue);

}