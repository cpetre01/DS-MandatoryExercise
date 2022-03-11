#ifndef DS_MANDATORY_EXERCISE_1_H
#define DS_MANDATORY_EXERCISE_1_H

#define MAXSIZE 255
#define SERVER_QUEUE_NAME "/server_queue"
#define GENERAL_NAME_CLIENT_QUEUE "/client_"

//struct request {
//    int a;                    /* op. 1 */
//    int b;                    /* op. 2 */
//    char q_name[MAXSIZE];     /* client queue name - this is
// *                            where the server sends the reply to */
//};

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
    char msg_code;              /* message code that indicates the client API
 *                              function called*/
    struct item *item;        /* pointer to struct containing all required
 *                              elements of an item*/
    char q_name[MAXSIZE];       /* client queue name - this is
 *                              where the server sends the reply to */

};

struct reply {
    /* server reply */
    int id;                     /* transaction ID */
    char msg_code;              /* message code that indicates the client API
 *                              function called*/
    char server_error_code;     /* error code returned by the server;
 *                              client API interprets it to figure out
 *                              whether the transaction was successful*/
    int num_items;              /* total number of items stored;
 *                              filled in case of num_items API call */
    struct item *item;        /* pointer to struct containing all required
 *                              elements of an item*/
};

#endif //DS_MANDATORY_EXERCISE_1_H
