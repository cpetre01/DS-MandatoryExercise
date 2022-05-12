#include <DS-MandatoryExercise/rpc.h>
#include "DS-MandatoryExercise/utils.h"

int init() {
    char *host;
    host = getenv("RPC_HOST");
    CLIENT *clnt;
    enum clnt_stat retval_1;
    int result_1;
#ifndef    DEBUG
    clnt = clnt_create(host, KEYS, KEYSVERS, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
#endif    /* DEBUG */
    retval_1 = init_1(&result_1, clnt);
    if (retval_1 != RPC_SUCCESS) {
        clnt_perror(clnt, "call failed");
        return -1;

    } else{
        return (result_1 == SRV_ERROR) ? -1 : 0;
    }
#ifndef DEBUG
    clnt_destroy (clnt);
#endif   /* DEBUG */
}


int set_value(int key, char *value1, int value2, float value3) {
    char *host;
    host = getenv("RPC_HOST");
    CLIENT *clnt;
    enum clnt_stat retval_2;
    int result_2;
#ifndef    DEBUG
    clnt = clnt_create(host, KEYS, KEYSVERS, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
#endif /* DEBUG */
    retval_2 = set_value_1(key, value1, value2, value3, &result_2, clnt);
    if (retval_2 != RPC_SUCCESS) {
        clnt_perror(clnt, "call failed");
        return -1;

    } else{
        return (result_2 == SRV_ERROR) ? -1 : 0;
    }

#ifndef DEBUG
    clnt_destroy (clnt);
#endif   /* DEBUG */
}



int get_value (int key, char *value1, int *value2, float *value3){
    char *host;

    host = getenv("RPC_HOST");

    CLIENT *clnt;
    enum clnt_stat retval_3;
    struct item result_3;
    result_3.value1 = malloc(255);

#ifndef    DEBUG
    clnt = clnt_create(host, KEYS, KEYSVERS, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        free(result_3.value1);
        exit(1);
    }
#endif    /* DEBUG */
    retval_3 = get_value_1(key, &result_3, clnt);
    if (retval_3 != RPC_SUCCESS) {
        clnt_perror(clnt, "call failed");
        return -1;

    } else {
        if (result_3.error == SRV_ERROR){
            return -1;
        }
        strcpy( value1, result_3.value1);
        *value2 = result_3.value2;
        *value3 = result_3.value3;
        free(result_3.value1);
        return 0;
    }

#ifndef DEBUG
    clnt_destroy (clnt);
#endif   /* DEBUG */
}



int modify_value(int key, char * value1, int value2, float value3){
    char *host;

    host = getenv("RPC_HOST");

    CLIENT *clnt;
    enum clnt_stat retval_4;
    int result_4;
#ifndef    DEBUG
    clnt = clnt_create(host, KEYS, KEYSVERS, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
#endif    /* DEBUG */
    retval_4 = modify_value_1(key, value1, value2, value3, &result_4, clnt);
    if (retval_4 != RPC_SUCCESS) {
        clnt_perror(clnt, "call failed");
        return -1;

    } else{
        return (result_4 == SRV_ERROR) ? -1 : 0;
    }
#ifndef DEBUG
    clnt_destroy (clnt);
#endif   /* DEBUG */
}



int delete_key (int key){
    char *host;

    host = getenv("RPC_HOST");

    CLIENT *clnt;
    enum clnt_stat retval_5;
    int result_5;
#ifndef    DEBUG
    clnt = clnt_create(host, KEYS, KEYSVERS, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
#endif    /* DEBUG */
    retval_5 = delete_key_1(key, &result_5, clnt);
    if (retval_5 != RPC_SUCCESS) {
        clnt_perror(clnt, "call failed");
        return -1;

    } else{
        return (result_5 == SRV_ERROR) ? -1 : 0;
    }
#ifndef DEBUG
    clnt_destroy (clnt);
#endif   /* DEBUG */
}



int exist(int key){
    char *host;

    host = getenv("RPC_HOST");

    CLIENT *clnt;
    enum clnt_stat retval_6;
    int result_6;
#ifndef    DEBUG
    clnt = clnt_create(host, KEYS, KEYSVERS, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
#endif    /* DEBUG */
    retval_6 = exist_1(key, &result_6, clnt);
    if (retval_6 != RPC_SUCCESS) {
        clnt_perror(clnt, "call failed");
        return -1;

    } else{
        if(result_6 == SRV_EXISTS) return SRV_EXISTS;
        return SRV_NOT_EXISTS;
    }
#ifndef DEBUG
    clnt_destroy (clnt);
#endif   /* DEBUG */
}


int num_items (){
    char *host;

    host = getenv("RPC_HOST");

    CLIENT *clnt;
    enum clnt_stat retval_7;
    int result_7;
#ifndef    DEBUG
    clnt = clnt_create(host, KEYS, KEYSVERS, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
#endif    /* DEBUG */
    retval_7 = num_items_1(&result_7, clnt);
    fprintf(stderr,"result in keys is %d\n", result_7);
    if (retval_7 != RPC_SUCCESS) {
        clnt_perror(clnt, "call failed");
        return -1;
    } else{
        if (result_7 == -1) {
            return -1;
        }
        return result_7;

    }
#ifndef DEBUG
    clnt_destroy (clnt);
#endif   /* DEBUG */
}