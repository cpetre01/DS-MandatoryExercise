#include <stdio.h>
#include <pthread.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>
#include "include/message.h"
#define MSG_QUEUE_SIZE 5
#define TRUE 1
#define FALSE 0

/* mutex and condition variables for the message copy */
pthread_mutex_t mutex_msg;
int msg_not_copied = TRUE;
pthread_cond_t cond_msg;
const char *db_dir = "db";

//int start_db() {
//    /* deleting all files */
//    if (remove("*") == 0) {
//        /* files were deleted successfully*/
//        return 0;
//    } else {
//        /* error*/
//        return -1;
//    }
//    return 0;
//}

void process_message(struct request *msg) {
    struct request msg_local;       /* local message */
    mqd_t q_client;                 /* client queue */
    int result;

    /* thread copies message to local message */
    pthread_mutex_lock(&mutex_msg);

    memcpy(&msg_local, msg, sizeof(struct request));

    /* wake up server */
    msg_not_copied = FALSE;

    pthread_cond_signal(&cond_msg);
    pthread_mutex_unlock(&mutex_msg);

    /* execute client request ad prepare reply */
//    result = msg_local.a + msg_local.b;

    /* return result to client by sending it to queue */
    q_client = mq_open(msg_local.q_name, O_WRONLY);

    if (q_client == -1) {
        char error[40];
        sprintf(error, "Can't open client queue %d", errno);
        perror(error);
    } else {
        mq_send(q_client, (char *) &result, sizeof(int), 0);
        mq_close(q_client);
    }
    pthread_exit(0);
}

int main(void)
{
    mqd_t q_server;                 /* server queue */
    struct request msg;             /* message to receive */
    struct mq_attr q_attr;          /* queue attributes */
    pthread_attr_t t_attr;          /* thread attributes */

    q_attr.mq_maxmsg = MSG_QUEUE_SIZE;
    q_attr.mq_msgsize = sizeof(struct request);

    q_server = mq_open("/SERVER", O_CREAT|O_RDONLY, 0766, &q_attr);
    if (q_server == -1) {
        perror("Can't create server queue");
        return -1;
    }

    pthread_mutex_init(&mutex_msg, NULL);
    pthread_cond_init(&cond_msg, NULL);
    pthread_attr_init(&t_attr);

    /* thread attributes */
    pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);


    while (TRUE) {
        mq_receive(q_server, (char *) &msg, sizeof(struct request), 0);
        if (mq_receive(q_server, (char *) &msg, sizeof(struct request), 0) != -1) {
            perror("server receiving error");
            return -1;
        }

        pthread_t thid;
        pthread_create(&thid, &t_attr, (void *(*)(void *)) process_message, &msg);

        /* wait for thread to copy message */
        pthread_mutex_lock(&mutex_msg);
        while (msg_not_copied)
            pthread_cond_wait(&cond_msg, &mutex_msg);
        msg_not_copied = TRUE;
        pthread_mutex_unlock(&mutex_msg);
    }   /* END while */
}   /* END main */

