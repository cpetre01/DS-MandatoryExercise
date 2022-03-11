#include "include/message.h"
#include <mqueue.h>
#include <string.h>
#include <stdio.h>

void main(void) {
    mqd_t q_server;         /* server message queue */
    mqd_t q_client;         /* client message queue */

    struct request req;
    int res;
    struct mq_attr attr;

    attr.mq_maxmsg = 1;
    attr.mq_msgsize = sizeof(int);

    q_client = mq_open("/CLIENT", O_CREAT|O_RDONLY, 0766, &attr);
    if (q_client == -1) {
        perror("Can't create client queue");
        return;
    }

    q_server = mq_open("/SERVER", O_WRONLY);
    if (q_server == -1) {
        perror("Can't open server queue");
        return;
    }

    /* fill in request */
//    req.a = 5;
//    req.b = 2;
    strcpy(req.q_name, "/CLIENT");

//    printf("req.a = %d\nreq.b = %d\nreq.q_name = %s\n", req.a, req.b, req.q_name);

    mq_send(q_server, (char *) &req, sizeof(struct request), 0);
    if (mq_send(q_server, (const char *) &req, sizeof(struct request), 0) == -1) {
        perror("client sending error");
        return;
    }
    printf("Result: %d\n", res);

    printf("after sending to server");
    mq_receive(q_client, (char *)&res, sizeof(int), 0);
    if (mq_receive(q_client, (char *) &res, sizeof(int), 0) == -1) {
        perror("client receiving error");
        return;
    }

    printf("Result: %d\n", res);

    mq_close(q_server);
    mq_close(q_client);
    mq_unlink("/CLIENT");
}
