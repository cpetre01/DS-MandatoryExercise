#ifndef UTILS_H
#define UTILS_H

/* utilities */
#define TRUE 1
#define FALSE 0
#define MAX_STR_SIZE 512            /* generic string size */
#define VALUE1_MAX_STR_SIZE 255     /* size of value1 string */
#define DB_NAME "db"                /* database directory name */

/* server stuff */
#define THREAD_POOL_SIZE 5          /* max number of service threads running */

/* services: operation codes */
#define INIT 'a'
#define SET_VALUE 'b'
#define GET_VALUE 'c'
#define MODIFY_VALUE 'd'
#define DELETE_KEY 'e'
#define EXIST 'f'
#define NUM_ITEMS 'g'
#define END_CONN 'h'

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

/* control variable states for the service thread loop that handles client connections */
#define ON 1
#define OFF 0

/* number casting stuff */
#define INT 'i'
#define FLOAT 'f'
int str_to_num(const char *value_str, void *value, char type);

/* file & socket stuff */
int send_msg(int d, char *buffer, int len);
int recv_msg(int d, char *buffer, int len);
ssize_t read_line(int d, char *buffer, int buf_space);


/* types */
#include <stdint.h>

/* type used to represent the actual elements to be stored */
typedef struct {
    int32_t key;                        /* key attribute */
    char value1[VALUE1_MAX_STR_SIZE];   /* string attribute */
    int32_t value2;                     /* int attribute */
    float value3;                       /* float attribute */
} item_t;

/* types used for process communication */
typedef struct {
    /* common header */
    uint32_t id;                /* transaction ID */
    char op_code;               /* operation code that indicates the client API function called */
} header_t;

typedef struct {
    /* client request */
    header_t header;
    item_t item;                /* struct containing all required elements of an item */
} request_t;

typedef struct {
    /* server reply */
    header_t header;
    int32_t server_error_code;  /* error code returned by the server; client API interprets it
 *                              to figure out whether the transaction was successful */
    uint32_t num_items;         /* total number of items stored; filled in case of num_items API call */
    item_t item;                /* struct containing all required elements of an item */
} reply_t;

#endif //UTILS_H