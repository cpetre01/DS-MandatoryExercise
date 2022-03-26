#ifndef UTILS_H
#define UTILS_H

/* utilities */
#define TRUE 1
#define FALSE 0
#define MAX_STR_SIZE 512            /* generic string size */
#define VALUE1_MAX_STR_SIZE 255     /* size of value1 string */
#define DB_NAME "db"                /* database directory name */

/* message queue stuff */
#define MSG_QUEUE_SIZE 5
#define SERVER_QUEUE_NAME "/server_queue"
#define CLIENT_QUEUE_NAME_TEMPLATE "/client_queue"

/* services: operation codes */
#define INIT 'a'
#define SET_VALUE 'b'
#define GET_VALUE 'c'
#define MODIFY_VALUE 'd'
#define DELETE_KEY 'e'
#define EXIST 'f'
#define NUM_ITEMS 'g'

/* server error codes */
#define ERROR 0
#define SUCCESS 1
#define INVALID_OP -1
/* these two are used for the "exist" service */
#define EXISTS 1
#define NOT_EXISTS 0

/* DB key file opening modes */
#define READ 'r'
#define CREATE 'c'
#define MODIFY 'm'

/* number casting stuff */
#define INT 'i'
#define FLOAT 'f'
int cast_value(const char *value_str, void *value, char type);

/* file stuff */
ssize_t read_line(int fd, void *buffer, size_t n);


/* types */

/* type used to represent the actual elements to be stored */
typedef struct {
    int key;                            /* key attribute */
    char value1[VALUE1_MAX_STR_SIZE];   /* string attribute */
    int value2;                         /* int attribute */
    float value3;                       /* float attribute */
} item_t;

/* types used for process communication */
typedef struct {
    /* client request */
    int id;                     /* transaction ID */
    char op_code;               /* operation code that indicates the client API function called */
    item_t item;                /* struct containing all required elements of an item */
    char q_name[MAX_STR_SIZE];  /* client queue name - this is where the server sends the reply to */
} request_t;

typedef struct {
    /* server reply */
    int id;                     /* transaction ID */
    char op_code;               /* operation code that indicates the client API function called */
    int server_error_code;      /* error code returned by the server; client API interprets it
 *                              to figure out whether the transaction was successful */
    int num_items;              /* total number of items stored; filled in case of num_items API call */
    item_t item;                /* struct containing all required elements of an item */
} reply_t;

#endif //UTILS_H