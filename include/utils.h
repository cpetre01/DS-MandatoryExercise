/* utilities */
#define TRUE 1
#define FALSE 0
#define MAX_STR_SIZE 255

/* message queue stuff */
#define MSG_QUEUE_SIZE 5
#define SERVER_QUEUE_NAME "/server_queue"
#define CLIENT_QUEUE_NAME_TEMPLATE "/client_"

/* operation codes */
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

/* db key file modes */
#define READ 'r'
#define CREATE 'c'
#define MODIFY 'm'

/* number casting stuff */
#define INT 'i'
#define FLOAT 'f'
int cast_value(const char *value_str, void *value, char type);

struct item {
    /* item to be stored */
    int key;                    /* key attribute */
    char *value1;               /* string attribute */
    int value2;                 /* int attribute */
    float value3;               /* float attribute */
};


struct request {
    /* client request */
    int id;                     /* transaction ID */
    char op_code;              /* operation code that indicates the client API
 *                              function called*/
    struct item *item;        /* pointer to struct containing all required
 *                              elements of an item*/
    char q_name[MAX_STR_SIZE];       /* client queue name - this is
 *                              where the server sends the reply to */

};

struct reply {
    /* server reply */
    int id;                     /* transaction ID */
    char op_code;              /* operation code that indicates the client API
 *                              function called*/
    char server_error_code;     /* error code returned by the server;
 *                              client API interprets it to figure out
 *                              whether the transaction was successful*/
    int num_items;              /* total number of items stored;
 *                              filled in case of num_items API call */
    struct item *item;        /* pointer to struct containing all required
 *                              elements of an item*/
};
