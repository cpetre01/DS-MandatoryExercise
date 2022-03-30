#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include "utils.h"
#include "dbms.h"


/* prototypes */
void copy_request(request_t *local_req, const request_t *request);
void set_server_error_code_std(reply_t *server_reply, int req_error_code);
void service_thread(request_t *request);
/* services */
void init_db(reply_t *server_reply);
void insert_item(request_t *request, reply_t *server_reply);
void get_item(request_t *request, reply_t *server_reply);
void modify_item(request_t *request, reply_t *server_reply);
void delete_item(request_t *request, reply_t *server_reply);
void item_exists(request_t *request, reply_t *server_reply);
void get_num_items(reply_t *server_reply);


//mqd_t server_q;             /* server queue */
//struct mq_attr q_attr;      /* queue attributes */

/* mutex and condition variables for the message copy */
pthread_mutex_t mutex_req;
int req_copied = FALSE;
pthread_cond_t cond_req;

pthread_mutex_t mutex_db;       /* mutex for atomic operations on the DB */
pthread_attr_t th_attr;         /* service thread attributes */


/* internal functions */
void copy_request(request_t *local_req, const request_t *request) {
    /* thread copies request to local_req */
    pthread_mutex_lock(&mutex_req);
    local_req->id = request->id;
    local_req->op_code = request->op_code;
    memcpy(&(local_req->item), &(request->item), sizeof(item_t));
    strncpy(local_req->q_name,request->q_name, MAX_STR_SIZE);

    /* wake up server */
    req_copied = TRUE;
    pthread_cond_signal(&cond_req);
    pthread_mutex_unlock(&mutex_req);
}


void set_server_error_code_std(reply_t *server_reply, const int req_error_code) {
    /* most services follow this error code model */
    switch (req_error_code) {
        case 0: server_reply->server_error_code = SUCCESS; break;
        case -1: server_reply->server_error_code = ERROR; break;
        default: break;
    }
}


/* required functions */
void service_thread(request_t *request) {
    /* make a local copy of the client request */
    request_t local_req;
    copy_request(&local_req, request);

//    /* open client queue */
//    mqd_t client_q;
//    client_q = mq_open(local_req.q_name, O_WRONLY);
//    if (client_q == -1) {
//        perror("Can't open client queue");
//        pthread_exit((void *) -1);
//    }

    /* set up server reply */
    reply_t server_reply;
    server_reply.id = local_req.id;
    server_reply.op_code = local_req.op_code;

    /* execute client request */
    switch (local_req.op_code) {
        case INIT: init_db(&server_reply); break;
        case SET_VALUE: insert_item(&local_req, &server_reply); break;
        case GET_VALUE: get_item(&local_req, &server_reply); break;
        case MODIFY_VALUE: modify_item(&local_req, &server_reply); break;
        case DELETE_KEY: delete_item(&local_req, &server_reply); break;
        case EXIST: item_exists(&local_req, &server_reply); break;
        case NUM_ITEMS: get_num_items(&server_reply); break;
        default: break;     /* invalid operation */
    }

//    /* send server reply */
//    if (mq_send(client_q, (char *) &server_reply, sizeof(reply_t), 0) == -1) {
//        perror("Error sending server reply");
//        mq_close(client_q);
//        pthread_exit((void *) -1);
//    }
    pthread_exit(0);
}


void init_db(reply_t *server_reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_empty_db();
    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    set_server_error_code_std(server_reply, req_error_code);
}


void insert_item(request_t *request, reply_t *server_reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_write_item(request->item.key, request->item.value1,
                                       &(request->item.value2),&(request->item.value3), CREATE);
    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    set_server_error_code_std(server_reply, req_error_code);
}


void get_item(request_t *request, reply_t *server_reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_read_item(request->item.key, request->item.value1,
                                      &(request->item.value2), &(request->item.value3));
    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    switch (req_error_code) {
        case 0:
            server_reply->server_error_code = SUCCESS;
            server_reply->item.key = request->item.key;
            strncpy(server_reply->item.value1, request->item.value1, VALUE1_MAX_STR_SIZE);
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
    set_server_error_code_std(server_reply, req_error_code);
}


void delete_item(request_t *request, reply_t *server_reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_delete_item(request->item.key);
    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    set_server_error_code_std(server_reply, req_error_code);
}


void item_exists(request_t *request, reply_t *server_reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int req_error_code = db_item_exists(request->item.key);
    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    switch (req_error_code) {
        case 1: server_reply->server_error_code = EXISTS; break;
        case 0: server_reply->server_error_code = NOT_EXISTS; break;
        default: break;
    }
}


void get_num_items(reply_t *server_reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);
    int num_items = db_get_num_items();
    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    if (num_items == -1) server_reply->server_error_code = ERROR;
    else {
        server_reply->server_error_code = SUCCESS;
        server_reply->num_items = num_items;
    }
}


void shutdown_server() {
    /* destroy server resources before shutting it down */
    pthread_mutex_destroy(&mutex_req);
    pthread_mutex_destroy(&mutex_db);
    pthread_cond_destroy(&cond_req);
    pthread_attr_destroy(&th_attr);
//    mq_close(server_q);
//    mq_unlink(SERVER_QUEUE_NAME);
    exit(0);
}


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage server <PORT>\n");
        return -1;
    }
//    /* queue attributes */
//    q_attr.mq_maxmsg = MSG_QUEUE_SIZE;
//    q_attr.mq_msgsize = sizeof(request_t);

    int server_port;
    if (str_to_num(argv[1], (void *) &server_port, INT) == -1) {
        perror("Invalid server port"); return -1;
    }

    struct sockaddr_in server_addr, client_addr;
    socklen_t size;
    int server_sd, client_sd;
    int val;

    pthread_t thid;                 /* service thread id */

    /* get server up & running */
    if ((server_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Can't create server socket"); return -1;
    }

    val = 1;
    setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));

    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(server_port);

    if (bind(server_sd, (const struct sockaddr *) &server_addr, sizeof server_addr) == -1) {
        perror("Server socket binding error"); return -1;
    }

    if (listen(server_sd, SOMAXCONN) == -1) {
        perror("Server listen error"); return -1;
    }

    size = sizeof client_addr;

//    server_q = mq_open(SERVER_QUEUE_NAME, O_CREAT | O_RDONLY, 0666, &q_attr);
//    if (server_q == -1) {
//        perror("Can't create server queue");
//        return -1;
//    } else {
//        printf("Press Ctrl + C to shut down server\n");
//    }

    pthread_mutex_init(&mutex_req, NULL);
    pthread_cond_init(&cond_req, NULL);
    pthread_mutex_init(&mutex_db, NULL);

    /* make service threads detached */
    pthread_attr_init(&th_attr);
    pthread_attr_setdetachstate(&th_attr, PTHREAD_CREATE_DETACHED);

    /* set up SIGINT (CTRL+C) signal handler to shut down server */
    struct sigaction keyboard_interrupt;
    keyboard_interrupt.sa_handler = shutdown_server;
    keyboard_interrupt.sa_flags = 0;
    sigemptyset(&keyboard_interrupt.sa_mask);
    sigaction(SIGINT, &keyboard_interrupt, NULL);

    while (TRUE) {
        printf("Press Ctrl + C to shut down server\n");
        printf("Waiting connection...\n");

        client_sd = accept(server_sd, (struct sockaddr *) &client_addr, (socklen_t *) &size);
        if (client_sd == -1) {
            perror("Server accept error"); return -1;
        }

        printf("Accepted connection IP: %s   Port: %d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        /* receive client request */
        request_t request;
//        if (mq_receive(server_q, (char *) &request, sizeof(request), 0) == -1) {
//            perror("server receiving error");
//            continue;
//        }

        /* check whether client request is valid */
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
