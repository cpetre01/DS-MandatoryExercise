#ifndef DISTRIBUTED_SYSTEMS_MESSAGE_H
#define DISTRIBUTED_SYSTEMS_MESSAGE_H
struct message{
    char msg_code;
    int *key;
    char *value1;
    int *value2;
    float *value3;
    char server_error_code;
};
#endif //DISTRIBUTED_SYSTEMS_MESSAGE_H
