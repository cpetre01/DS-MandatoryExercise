#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>
#include <signal.h>
#include "include/utils.h"
#include "include/dbms.h"


/* prototypes */
void copy_request(request_t *local_req, const request_t *request);
void service_thread(request_t *request);
/* services */
void init_db(request_t *request, reply_t *server_reply);
void insert_item(request_t *request, reply_t *server_reply);
void get_item(request_t *request, reply_t *server_reply);
void modify_item(request_t *request, reply_t *server_reply);
void delete_item(request_t *request, reply_t *server_reply);
void item_exists(request_t *request, reply_t *server_reply);
void get_num_items(request_t *request, reply_t *server_reply);


/* server queue */
mqd_t server_q;
/* queue attributes */
struct mq_attr q_attr;


/* mutex and condition variables for the message copy */
pthread_mutex_t mutex_req;
int req_copied = FALSE;
pthread_cond_t cond_req;
pthread_mutex_t mutex_db;


/* internal functions */
void copy_request(request_t *local_req, const request_t *request) {
    /* thread copies request to local request */
    pthread_mutex_lock(&mutex_req);

    local_req->id = request->id;
    local_req->op_code = request->op_code;
    memcpy(&(local_req->item), &(request->item), sizeof(item_t));
    strcpy(local_req->q_name,request->q_name);

    /* wake up server */
    req_copied = TRUE;
    pthread_cond_signal(&cond_req);
    pthread_mutex_unlock(&mutex_req);
}


/* required functions */
void service_thread(request_t *request) {
    /* make a local copy of the client request */
    request_t local_req;
    copy_request(&local_req, request);

    /* open client queue */
    mqd_t client_q;
    client_q = mq_open(local_req.q_name, O_WRONLY);
    if (client_q == -1) {
        perror("Can't open client queue");
        pthread_exit((void *) -1);
    }

    /* execute client request */
    reply_t server_reply;
    switch (local_req.op_code) {
        case INIT:          init_db(&local_req, &server_reply); break;
        case SET_VALUE:     insert_item(&local_req, &server_reply); break;
        case GET_VALUE:     get_item(&local_req, &server_reply); break;
        case MODIFY_VALUE:  modify_item(&local_req, &server_reply); break;
        case DELETE_KEY:    delete_item(&local_req, &server_reply); break;
        case EXIST:         item_exists(&local_req, &server_reply); break;
        case NUM_ITEMS:     get_num_items(&local_req, &server_reply); break;
        default: break;     /* invalid operation */
    }

    /* send server reply */
    if (mq_send(client_q, (char *) &server_reply, sizeof(reply_t), 0) == -1) {
        perror("Error sending server reply");
        mq_close(client_q);
        pthread_exit((void *) -1);
    }
    pthread_exit(0);
}


void init_db(request_t *request, reply_t *server_reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_empty_db();
    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    server_reply->id = request->id;
    server_reply->op_code = request->op_code;

    switch (req_error_code) {
        case 0: server_reply->server_error_code = SUCCESS; break;
        case -1: server_reply->server_error_code = ERROR; break;
        default: break;
    }
}


void insert_item(request_t *request, reply_t *server_reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_write_item(request->item.key, request->item.value1,
                                       &(request->item.value2),&(request->item.value3), CREATE);
    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    server_reply->id = request->id;
    server_reply->op_code = request->op_code;

    switch (req_error_code) {
        case 0: server_reply->server_error_code = SUCCESS; break;
        case -1: server_reply->server_error_code = ERROR; break;
        default: break;
    }
}


void get_item(request_t *request, reply_t *server_reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_read_item(request->item.key, request->item.value1,
                                      &(request->item.value2), &(request->item.value3));
    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    server_reply->id = request->id;
    server_reply->op_code = request->op_code;

    switch (req_error_code) {
        case 0:
            server_reply->server_error_code = SUCCESS;
            server_reply->item.key = request->item.key;
            strcpy(server_reply->item.value1, request->item.value1);
            server_reply->item.value2 = request->item.value2;
            server_reply->item.value3 = request->item.value3;
            break;
        case -1: server_reply->server_error_code = ERROR; break;
        default: break;
    }
}


void modify_item(request_t *request, reply_t *server_reply){
    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_write_item(request->item.key, request->item.value1,
                                       &(request->item.value2), &(request->item.value3), MODIFY);
    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    server_reply->id = request->id;
    server_reply->op_code = request->op_code;

    switch (req_error_code) {
        case 0: server_reply->server_error_code = SUCCESS; break;
        case -1: server_reply->server_error_code = ERROR; break;
        default: break;
    }
}


void delete_item(request_t *request, reply_t *server_reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_delete_item(request->item.key);
    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    server_reply->id = request->id;
    server_reply->op_code = request->op_code;

    switch (req_error_code) {
        case 0: server_reply->server_error_code = SUCCESS; break;
        case -1: server_reply->server_error_code = ERROR; break;
        default: break;
    }
}


void item_exists(request_t *request, reply_t *server_reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_item_exists(request->item.key);
    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    server_reply->id = request->id;
    server_reply->op_code = request->op_code;

    switch (req_error_code) {
        case 1: server_reply->server_error_code = EXISTS; break;
        case 0: server_reply->server_error_code = NOT_EXISTS; break;
        default: break;
    }
}


void get_num_items(request_t *request, reply_t *server_reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int num_items = db_get_num_items();
    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    server_reply->id = request->id;
    server_reply->op_code = request->op_code;

    if (num_items == -1) server_reply->server_error_code = ERROR;
    else {
        server_reply->num_items = num_items;
        server_reply->server_error_code = SUCCESS;
    }
}


void shutdown_server() {
    /* destroy server resources before shutting it down */
    pthread_mutex_destroy(&mutex_req);
    pthread_mutex_destroy(&mutex_db);
    pthread_cond_destroy(&cond_req);
    mq_close(server_q);
    mq_unlink(SERVER_QUEUE_NAME);
    exit(0);
}


int main(void)
{
    /* queue attributes */
    q_attr.mq_maxmsg = MSG_QUEUE_SIZE;
    q_attr.mq_msgsize = sizeof(request_t);

    pthread_t thid;                 /* service thread id */
    pthread_attr_t th_attr;         /* service thread attributes */

    /* get server up & running */
    server_q = mq_open(SERVER_QUEUE_NAME, O_CREAT | O_RDONLY, 0666, &q_attr);
    if (server_q == -1) {
        perror("Can't create server queue");
        return -1;
    } else {
        printf("Press Ctrl + C to shut down server\n");
    }

    pthread_mutex_init(&mutex_req, NULL);
    pthread_mutex_init(&mutex_db, NULL);
    pthread_cond_init(&cond_req, NULL);
    pthread_attr_init(&th_attr);

    /* make service thread detached */
    pthread_attr_setdetachstate(&th_attr, PTHREAD_CREATE_DETACHED);

    /* set up SIGINT (CTRL+C) signal handler to shut down server */
    struct sigaction keyboard_interrupt;
    keyboard_interrupt.sa_handler = shutdown_server;
    keyboard_interrupt.sa_flags = 0;
    sigemptyset(&keyboard_interrupt.sa_mask);
    sigaction(SIGINT, &keyboard_interrupt, NULL);


    while (TRUE) {
        /* receive client request */
        request_t request;
        if (mq_receive(server_q, (char *) &request, sizeof(request), 0) == -1) {
            perror("server receiving error");
            continue;
        }

        /* parse request */
        if (request.op_code == INIT || request.op_code == SET_VALUE || request.op_code == GET_VALUE ||
            request.op_code == MODIFY_VALUE || request.op_code == DELETE_KEY || request.op_code == EXIST ||
            request.op_code == NUM_ITEMS)
            /* launch service thread to process client request */
            pthread_create(&thid, &th_attr, (void *) service_thread, &request);
        else continue;      /* invalid operation */

        /* wait for launched service thread to copy request */
        pthread_mutex_lock(&mutex_req);
        while (!req_copied)
            pthread_cond_wait(&cond_req, &mutex_req);
        req_copied = FALSE;
        pthread_mutex_unlock(&mutex_req);
    }   // END while
}
