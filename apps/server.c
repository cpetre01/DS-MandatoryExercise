#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>
#include "include/utils.h"
#include "include/dbms.h"


/* prototypes */
void copy_request(struct request *local_req, const struct request *request);
/* services */
void init_db(struct request *request);
void insert_item(struct request *request);
void get_item(struct request *request);
void modify_item(struct request *request);
void delete_item(struct request *request);
void item_exists(struct request *request);
void get_num_items(struct request *request);


/* mutex and condition variables for the message copy */
pthread_mutex_t mutex_req;
int req_copied = FALSE;
pthread_cond_t cond_req;
pthread_mutex_t mutex_db;


/* internal functions */
void copy_request(struct request *local_req, const struct request *request) {
    /* thread copies request to local request */
    pthread_mutex_lock(&mutex_req);

    local_req->id = request->id;
    local_req->op_code = request->op_code;
    memcpy(&(local_req->item), &(request->item), sizeof(struct item));
    strcpy(local_req->q_name,request->q_name);

    /* wake up server */
    req_copied = TRUE;
    pthread_cond_signal(&cond_req);
    pthread_mutex_unlock(&mutex_req);
}


/* required functions */
void init_db(struct request *request) {
    /* make a local copy of the client request */
    struct request local_req;
    copy_request(&local_req, request);

    /* open client queue */
    mqd_t client_q;
    client_q = mq_open(local_req.q_name, O_WRONLY);
    if (client_q == -1) {
        perror("Can't open client queue");
        pthread_exit((void *) -1);
    }

    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_empty_db();
    pthread_mutex_unlock(&mutex_db);

    /* create server reply */
    struct reply server_reply;
    server_reply.id = local_req.id;
    server_reply.op_code = local_req.op_code;

    switch (req_error_code) {
        case 0:
            server_reply.server_error_code = SUCCESS;
            break;
        case -1:
            server_reply.server_error_code = ERROR;
            break;
        default:
            break;
    }

    /* send server reply */
    if (mq_send(client_q, (char *) &server_reply, sizeof(struct reply), 0) == -1) {
        perror("Error sending server reply");
        mq_close(client_q);
        pthread_exit((void *) -1);
    }
    pthread_exit(0);
}


void insert_item(struct request *request) {
    /* make a local copy of the client request */
    struct request local_req;
    copy_request(&local_req, request);

    /* open client queue */
    mqd_t client_q;
    client_q = mq_open(local_req.q_name, O_WRONLY);
    if (client_q == -1) {
        perror("Can't open client queue");
        pthread_exit((void *) -1);
    }

    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_write_item(local_req.item.key, local_req.item.value1,
                                       &(local_req.item.value2),&(local_req.item.value3), CREATE);
    pthread_mutex_unlock(&mutex_db);

    /* create server reply */
    struct reply server_reply;
    server_reply.id = local_req.id;
    server_reply.op_code = local_req.op_code;

    switch (req_error_code) {
        case 0:
            server_reply.server_error_code = SUCCESS;
            break;
        case -1:
            server_reply.server_error_code = ERROR;
            break;
        default:
            break;
    }

    /* send server reply */
    if (mq_send(client_q, (char *) &server_reply, sizeof(struct reply), 0) == -1) {
        perror("Error sending server reply");
        mq_close(client_q);
        pthread_exit((void *) -1);
    }
    pthread_exit(0);
}


void get_item(struct request *request) {
    /* make a local copy of the client request */
    struct request local_req;
    copy_request(&local_req, request);

    /* open client queue */
    mqd_t client_q;
    client_q = mq_open(local_req.q_name, O_WRONLY);
    if (client_q == -1) {
        perror("Can't open client queue");
        pthread_exit((void *) -1);
    }

    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_read_item(local_req.item.key, local_req.item.value1,
                                       &(local_req.item.value2), &(local_req.item.value3));
    pthread_mutex_unlock(&mutex_db);

    /* create server reply */
    struct reply server_reply;
    server_reply.id = local_req.id;
    server_reply.op_code = local_req.op_code;

    switch (req_error_code) {
        case 0:
            server_reply.server_error_code = SUCCESS;
            server_reply.item.key = local_req.item.key;
            strcpy(server_reply.item.value1, local_req.item.value1);
            server_reply.item.value2 = local_req.item.value2;
            server_reply.item.value3 = local_req.item.value3;
            break;
        case -1:
            server_reply.server_error_code = ERROR;
            break;
        default:
            break;
    }

    /* send server reply */
    if (mq_send(client_q, (char *) &server_reply, sizeof(struct reply), 0) == -1) {
        perror("Error sending server reply");
        mq_close(client_q);
        pthread_exit((void *) -1);
    }
    pthread_exit(0);
}


void modify_item(struct request *request){
    /* make a local copy of the client request */
    struct request local_req;
    copy_request(&local_req, request);

    /* open client queue */
    mqd_t client_q;
    client_q = mq_open(local_req.q_name, O_WRONLY);
    if (client_q == -1) {
        perror("Can't open client queue");
        pthread_exit((void *) -1);
    }

    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_write_item(local_req.item.key, local_req.item.value1,
                                       &(local_req.item.value2), &(local_req.item.value3), MODIFY);
    pthread_mutex_unlock(&mutex_db);

    /* create server reply */
    struct reply server_reply;
    server_reply.id = local_req.id;
    server_reply.op_code = local_req.op_code;

    switch (req_error_code) {
        case 0:
            server_reply.server_error_code = SUCCESS;
            break;
        case -1:
            server_reply.server_error_code = ERROR;
            break;
        default:
            break;
    }

    /* send server reply */
    if (mq_send(client_q, (char *) &server_reply, sizeof(struct reply), 0) == -1) {
        perror("Error sending server reply");
        mq_close(client_q);
        pthread_exit((void *) -1);
    }
    pthread_exit(0);
}


void delete_item(struct request *request) {
    /* make a local copy of the client request */
    struct request local_req;
    copy_request(&local_req, request);

    /* open client queue */
    mqd_t client_q;
    client_q = mq_open(local_req.q_name, O_WRONLY);
    if (client_q == -1) {
        perror("Can't open client queue");
        pthread_exit((void *) -1);
    }

    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_delete_item(local_req.item.key);
    pthread_mutex_unlock(&mutex_db);

    /* create server reply */
    struct reply server_reply;
    server_reply.id = local_req.id;
    server_reply.op_code = local_req.op_code;

    switch (req_error_code) {
        case 0:
            server_reply.server_error_code = SUCCESS;
            break;
        case -1:
            server_reply.server_error_code = ERROR;
            break;
        default:
            break;
    }

    /* send server reply */
    if (mq_send(client_q, (char *) &server_reply, sizeof(struct reply), 0) == -1) {
        perror("Error sending server reply");
        mq_close(client_q);
        pthread_exit((void *) -1);
    }
    pthread_exit(0);
}


void item_exists(struct request *request) {
    /* make a local copy of the client request */
    struct request local_req;
    copy_request(&local_req, request);

    /* open client queue */
    mqd_t client_q;
    client_q = mq_open(local_req.q_name, O_WRONLY);
    if (client_q == -1) {
        perror("Can't open client queue");
        pthread_exit((void *) -1);
    }

    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_item_exists(local_req.item.key);
    pthread_mutex_unlock(&mutex_db);

    /* create server reply */
    struct reply server_reply;
    server_reply.id = local_req.id;
    server_reply.op_code = local_req.op_code;

    switch (req_error_code) {
        case 0:
            server_reply.server_error_code = SUCCESS;
            break;
        case -1:
            server_reply.server_error_code = ERROR;
            break;
        default:
            break;

    }

    /* send server reply */
    if (mq_send(client_q, (char *) &server_reply, sizeof(struct reply), 0) == -1) {
        perror("Error sending server reply");
        mq_close(client_q);
        pthread_exit((void *) -1);
    }
    pthread_exit(0);
}


void get_num_items(struct request *request) {
    /* make a local copy of the client request */
    struct request local_req;
    copy_request(&local_req, request);

    /* open client queue */
    mqd_t client_q;
    client_q = mq_open(local_req.q_name, O_WRONLY);
    if (client_q == -1) {
        perror("Can't open client queue");
        pthread_exit((void *) -1);
    }

    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int num_items = db_get_num_items();
    pthread_mutex_unlock(&mutex_db);

    /* create server reply */
    struct reply server_reply;
    server_reply.id = local_req.id;
    server_reply.op_code = local_req.op_code;

    if (num_items == -1) {
        server_reply.server_error_code = ERROR;
    } else {
        server_reply.num_items = num_items;
        server_reply.server_error_code = SUCCESS;
    }

    /* send server reply */
    if (mq_send(client_q, (char *) &server_reply, sizeof(struct reply), 0) == -1) {
        perror("Error sending server reply");
        mq_close(client_q);
        pthread_exit((void *) -1);
    }
    pthread_exit(0);
}


int main(void)
{
    mqd_t server_q;                 /* server queue */
    /* queue attributes */
    struct mq_attr q_attr;
    q_attr.mq_maxmsg = MSG_QUEUE_SIZE;
    q_attr.mq_msgsize = sizeof(struct request);

    pthread_attr_t th_attr;         /* thread attributes */

    server_q = mq_open(SERVER_QUEUE_NAME, O_CREAT | O_RDONLY, 0666, &q_attr);
    if (server_q == -1) {
        perror("Can't create server queue");
        return -1;
    } else {
        printf("Server running\n");
    }

    pthread_mutex_init(&mutex_req, NULL);
    pthread_mutex_init(&mutex_db, NULL);
    pthread_cond_init(&cond_req, NULL);
    pthread_attr_init(&th_attr);

    /* thread attributes */
    pthread_attr_setdetachstate(&th_attr, PTHREAD_CREATE_DETACHED);


    while (TRUE) {
        /* receive client request */
        struct request request;
        if (mq_receive(server_q, (char *) &request, sizeof(struct request), 0) == -1) {
            perror("server receiving error");
            continue;
        }

        /* launch thread to process client request */
        pthread_t thid;
        /* parse request */
        switch (request.op_code) {
            case INIT:
                pthread_create(&thid, &th_attr, (void *(*)(void *)) init_db, &request);
                break;
            case SET_VALUE:
                pthread_create(&thid, &th_attr, (void *(*)(void *)) insert_item, &request);
                break;
            case GET_VALUE:
                pthread_create(&thid, &th_attr, (void *(*)(void *)) get_item, &request);
                break;
            case MODIFY_VALUE:
                pthread_create(&thid, &th_attr, (void *(*)(void *)) modify_item, &request);
                break;
            case DELETE_KEY:
                pthread_create(&thid, &th_attr, (void *(*)(void *)) delete_item, &request);
                break;
            case EXIST:
                pthread_create(&thid, &th_attr, (void *(*)(void *)) item_exists, &request);
                break;
            case NUM_ITEMS:
                pthread_create(&thid, &th_attr, (void *(*)(void *)) get_num_items, &request);
                break;
            default:
                /* invalid operation */
                continue;
        }

        /* wait for launched thread to copy request */
        pthread_mutex_lock(&mutex_req);
        while (!req_copied)
            pthread_cond_wait(&cond_req, &mutex_req);
        req_copied = FALSE;
        pthread_mutex_unlock(&mutex_req);
    }   // END while
}
