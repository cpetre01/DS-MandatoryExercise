#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include "utils.h"
#include "dbms.h"

/* prototypes */
void *service_thread(void *args);
void set_server_error_code_std(reply_t *reply, int req_error_code);

/* sending functions */
int send_reply_header(int client_socket, reply_t *reply);
int send_num_items(int client_socket, reply_t *reply);

/* receiving functions */
int recv_request_header(int client_socket, request_t *request);
int recv_key(int client_socket, request_t *request);
int recv_item(int client_socket, request_t *request);

/* services */
void init_db(reply_t *reply);
void insert_item(request_t *request, reply_t *reply);
void get_item(request_t *request, reply_t *reply);
void modify_item(request_t *request, reply_t *reply);
void delete_item(request_t *request, reply_t *reply);
void item_exists(request_t *request, reply_t *reply);
void get_num_items(reply_t *reply);


/* connection queue */
int conn_q[MAX_CONN_BACKLOG];   /* array of client sockets; used as a connection queue */
int conn_q_size = 0;            /* current number of backlogged connections */
int service_th_pos = 0;         /* connection queue position used by service threads to handle connections */

/* mutex and cond vars for conn_q access */
pthread_mutex_t mutex_conn_q;
pthread_cond_t cond_conn_q_not_empty;
pthread_cond_t cond_conn_q_not_full;

pthread_mutex_t mutex_db;                   /* mutex for atomic operations on the DB */
pthread_attr_t th_attr;                     /* service thread attributes */
pthread_t thread_pool[THREAD_POOL_SIZE];    /* array of service threads */


void set_server_error_code_std(reply_t *reply, const int req_error_code) {
    /* most services follow this error code model */
    switch (req_error_code) {
        case 0: reply->server_error_code = SUCCESS; break;
        case -1: reply->server_error_code = ERROR; break;
        default: break;
    }
}


int send_reply_header(const int client_socket, reply_t *reply) {
    /* function that sends transaction ID, op_code & server_error_code fields to client_socket */
    reply->id = htonl(reply->id);
    if (send_msg(client_socket, (char *) &reply->id, sizeof(uint32_t)) == -1) {
        perror("Send transaction ID error");
        close(client_socket); return -1;
    }

    if (send_msg(client_socket, &reply->op_code, 1) == -1) {
        perror("Send op_code error");
        close(client_socket); return -1;
    }

    reply->server_error_code = (int32_t) htonl(reply->server_error_code);
    if (send_msg(client_socket, (char *) &reply->server_error_code, sizeof(int32_t)) == -1) {
        perror("Send server_error_code error");
        close(client_socket); return -1;
    }

    return 0;
}


int send_num_items(const int client_socket, reply_t *reply) {
    /* function that sends num_items to client_socket */
    reply->num_items = htonl(reply->num_items);
    if (send_msg(client_socket, (char *) &reply->num_items, sizeof(uint32_t)) == -1) {
        perror("Send num_items error");
        close(client_socket); return -1;
    }

    return 0;
}


int send_item(const int client_socket, reply_t *reply) {
    /* function that sends an entire item to client_socket */

    /* send key */
    reply->item.key = (int32_t) htonl(reply->item.key);
    if (send_msg(client_socket, (char *) &reply->item.key, sizeof(int32_t)) == -1) {
        perror("Send key error");
        close(client_socket); return -1;
    }

    /* send value1 */
    if (send_msg(client_socket, reply->item.value1, (int) (strlen(reply->item.value1) + 1)) == -1) {
        perror("Send value1 error");
        close(client_socket); return -1;
    }

    /* send value2 */
    reply->item.value2 = (int32_t) htonl(reply->item.value2);
    if (send_msg(client_socket, (char *) &reply->item.value2, sizeof(int32_t)) == -1) {
        perror("Send value2 error");
        close(client_socket); return -1;
    }

    /* send value3 */
    reply->item.value3 = (float) htonl((uint32_t) reply->item.value3);
    if (send_msg(client_socket, (char *) &reply->item.value3, sizeof(float)) == -1) {
        perror("Send value3 error");
        close(client_socket); return -1;
    }

    return 0;
}


int recv_request_header(const int client_socket, request_t *request) {
    /* function that receives transaction ID & op_code fields from client_socket */

    /* receive transaction ID */
    if (recv_msg(client_socket, (char *) &request->id, sizeof(uint32_t)) == -1) {
        perror("Receive transaction ID error");
        close(client_socket); return -1;
    }
    request->id = ntohl(request->id);

    /* receive op_code */
    if (recv_msg(client_socket, &request->op_code, 1) == -1) {
        perror("Receive op_code error");
        close(client_socket); return -1;
    }

    return 0;
}


int recv_key(const int client_socket, request_t *request) {
    /* function that receives the key attribute from client_socket */
    if (recv_msg(client_socket, (char *) &request->item.key, sizeof(int32_t)) == -1) {
        perror("Receive key error");
        close(client_socket); return -1;
    }
    request->item.key = (int32_t) ntohl(request->item.key);

    return 0;
}


int recv_item(const int client_socket, request_t *request) {
    /* function that receives an entire item from client_socket */
    if (recv_key(client_socket, request) == -1) return -1;

    /* receive value1 */
    if (read_line(client_socket, request->item.value1, VALUE1_MAX_STR_SIZE) == -1) {
        perror("Receive value1 error");
        close(client_socket); return -1;
    }

    /* receive value2 */
    if (recv_msg(client_socket, (char *) &request->item.value2, sizeof(int32_t)) == -1) {
        perror("Receive value2 error");
        close(client_socket); return -1;
    }
    request->item.value2 = (int32_t) ntohl(request->item.value2);

    /* receive value3 */
    if (recv_msg(client_socket, (char *) &request->item.value3, sizeof(float)) == -1) {
        perror("Receive value3 error");
        close(client_socket); return -1;
    }
    request->item.value3 = (float) ntohl((uint32_t) request->item.value3);

    return 0;
}


void * service_thread(void *args) {
    while (TRUE) {
        int client_socket;
        /* copy client socket descriptor and free the original */
        pthread_mutex_lock(&mutex_conn_q);

        /* there are no connections to handle, so sleep */
        while (conn_q_size == 0)
            pthread_cond_wait(&cond_conn_q_not_empty, &mutex_conn_q);

        client_socket = conn_q[service_th_pos];
        service_th_pos = (service_th_pos + 1) % MAX_CONN_BACKLOG;
        conn_q_size -= 1;

        if (conn_q_size == MAX_CONN_BACKLOG - 1)
            pthread_cond_signal(&cond_conn_q_not_full);

        pthread_mutex_unlock(&mutex_conn_q);

        /* handle connection now */
        request_t request;

        /* receive transaction ID & op_code */
        if (recv_request_header(client_socket, &request) == -1) continue;

        /* set up server reply */
        reply_t reply;
        reply.id = request.id;
        reply.op_code = request.op_code;

        /* check whether client request is valid and execute it */
        switch (request.op_code) {
            case INIT:
                /* execute client request */
                init_db(&reply);

                /* send server reply */
                if (send_reply_header(client_socket, &reply) == -1) continue;
                break;
            case SET_VALUE:
                /* receive rest of client request */
                if (recv_item(client_socket, &request) == -1) continue;

                /* execute client request */
                insert_item(&request, &reply);

                /* send server reply */
                if (send_reply_header(client_socket, &reply) == -1) continue;
                break;
            case GET_VALUE:
                /* receive rest of client request */
                if (recv_key(client_socket, &request) == -1) continue;

                /* execute client request */
                get_item(&request, &reply);

                /* send server reply */
                if (send_reply_header(client_socket, &reply) == -1 ||
                send_item(client_socket, &reply) == -1) continue;
                break;
            case MODIFY_VALUE:
                /* receive rest of client request */
                if (recv_item(client_socket, &request) == -1) continue;

                /* execute client request */
                modify_item(&request, &reply);

                /* send server reply */
                if (send_reply_header(client_socket, &reply) == -1) continue;
                break;
            case DELETE_KEY:
                /* receive rest of client request */
                if (recv_key(client_socket, &request) == -1) continue;

                /* execute client request */
                delete_item(&request, &reply);

                /* send server reply */
                if (send_reply_header(client_socket, &reply) == -1) continue;
                break;
            case EXIST:
                /* receive rest of client request */
                if (recv_key(client_socket, &request) == -1) continue;

                /* execute client request */
                item_exists(&request, &reply);

                /* send server reply */
                if (send_reply_header(client_socket, &reply) == -1) continue;
                break;
            case NUM_ITEMS:
                /* execute client request */
                get_num_items(&reply);

                /* send server reply */
                if (send_reply_header(client_socket, &reply) == -1 ||
                send_num_items(client_socket, &reply) == -1) continue;
                break;
            default:    /* invalid operation */
                fprintf(stderr, "Requested invalid operation\n");
                close(client_socket); continue;
        } // end switch
        close(client_socket);
    } // end outer while
}


void init_db(reply_t *reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);

    int req_error_code = db_empty_db();

    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    set_server_error_code_std(reply, req_error_code);
}


void insert_item(request_t *request, reply_t *reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);

    int req_error_code = db_write_item(request->item.key, request->item.value1,
                                       &(request->item.value2),&(request->item.value3), CREATE);

    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    set_server_error_code_std(reply, req_error_code);
}


void get_item(request_t *request, reply_t *reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);

    int req_error_code = db_read_item(request->item.key, reply->item.value1,
                                      &(reply->item.value2), &(reply->item.value3));

    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    reply->item.key = request->item.key;
    set_server_error_code_std(reply, req_error_code);
//    switch (req_error_code) {
//        case 0:
//            reply->server_error_code = SUCCESS;
////            strncpy(reply->item.value1, request->item.value1, VALUE1_MAX_STR_SIZE);
////            reply->item.value2 = request->item.value2;
////            reply->item.value3 = request->item.value3;
//            break;
//        case -1: reply->server_error_code = ERROR; break;
//        default: break;
//    }
}


void modify_item(request_t *request, reply_t *reply){
    /* execute client request */
    pthread_mutex_lock(&mutex_db);

    int req_error_code = db_write_item(request->item.key, request->item.value1,
                                       &(request->item.value2), &(request->item.value3), MODIFY);

    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    set_server_error_code_std(reply, req_error_code);
}


void delete_item(request_t *request, reply_t *reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);

    int req_error_code = db_delete_item(request->item.key);

    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    set_server_error_code_std(reply, req_error_code);
}


void item_exists(request_t *request, reply_t *reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);

    int req_error_code = db_item_exists(request->item.key);

    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    switch (req_error_code) {
        case 1: reply->server_error_code = EXISTS; break;
        case 0: reply->server_error_code = NOT_EXISTS; break;
        default: break;
    }
}


void get_num_items(reply_t *reply) {
    /* execute client request */
    pthread_mutex_lock(&mutex_db);

    int num_items = db_get_num_items();

    pthread_mutex_unlock(&mutex_db);

    /* fill server reply */
    if (num_items == -1) reply->server_error_code = ERROR;
    else {
        reply->server_error_code = SUCCESS;
        reply->num_items = num_items;
    }
}


void shutdown_server() {
    /* destroy server resources before shutting it down */
    pthread_mutex_destroy(&mutex_conn_q);
    pthread_cond_destroy(&cond_conn_q_not_empty);
    pthread_cond_destroy(&cond_conn_q_not_full);
    pthread_mutex_destroy(&mutex_db);
    pthread_attr_destroy(&th_attr);
    exit(0);
}


int main(int argc, char **argv) {
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int server_sd, client_sd;
    int val = 1;

    if (argc != 2) {
        fprintf(stderr, "Usage server <PORT>\n"); return -1;
    }

    int server_port;
    if (str_to_num(argv[1], (void *) &server_port, INT) == -1) {
        perror("Invalid server port"); return -1;
    }

    /* set up connection queue */
    pthread_mutex_init(&mutex_conn_q, NULL);
    pthread_cond_init(&cond_conn_q_not_empty, NULL);
    pthread_cond_init(&cond_conn_q_not_full, NULL);
    /* conn_q position used by main thread to enqueue connections */
    int producer_pos = 0;

    /* make service threads detached */
    pthread_attr_init(&th_attr);
    pthread_attr_setdetachstate(&th_attr, PTHREAD_CREATE_DETACHED);

    pthread_mutex_init(&mutex_db, NULL);    /* for atomic DB operations */

    /* set up SIGINT (CTRL+C) signal handler to shut down server */
    struct sigaction keyboard_interrupt;
    keyboard_interrupt.sa_handler = shutdown_server;
    keyboard_interrupt.sa_flags = 0;
    sigemptyset(&keyboard_interrupt.sa_mask);
    sigaction(SIGINT, &keyboard_interrupt, NULL);

    /* get server up & running */
    if ((server_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Can't create server socket"); return -1;
    }

    setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));

    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(server_port);

    if (bind(server_sd, (struct sockaddr *) &server_addr, sizeof server_addr) == -1) {
        perror("Server socket binding error"); return -1;
    }

    if (listen(server_sd, MAX_CONN_BACKLOG) == -1) {
        perror("Server listen error"); return -1;
    }

    /* now create thread pool */
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_create(&thread_pool[i], &th_attr, service_thread, NULL);
    }

    while (TRUE) {
        printf("Press Ctrl + C to shut down server\n");
        printf("Waiting for connections...\n");

        client_sd = accept(server_sd, (struct sockaddr *) &client_addr, &addr_size);
        if (client_sd == -1) {
            perror("Server accept error"); return -1;
        }

        printf("Accepted connection IP: %s    Port: %d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        /* add connection to conn_q backlog */
        pthread_mutex_lock(&mutex_conn_q);

        /* if the connection queue is full, the main server thread sleeps:
         * no new connections can be opened until one is processed */
        while (conn_q_size == MAX_CONN_BACKLOG)
            pthread_cond_wait(&cond_conn_q_not_full, &mutex_conn_q);

        /* enqueue new connection */
        conn_q[producer_pos] = client_sd;
        producer_pos = (producer_pos + 1) % MAX_CONN_BACKLOG;
        conn_q_size += 1;

        /* signal that there are connections to handle */
        if (conn_q_size == 1)
            pthread_cond_signal(&cond_conn_q_not_empty);

        pthread_mutex_unlock(&mutex_conn_q);
    }   // END while
}
