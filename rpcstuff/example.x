struct item{
    string value1<256>;
    int value2;
    float value3;
    int error;
    unsigned num_items;
};


program KEYS {
    version KEYSVERS {
        struct item INIT() = 1;
        struct item SET_VALUE(int key) = 2;
        struct item GET_VALUE(int key) = 3;
        struct item MODIFY_VALUE(int key) = 4;
        struct item DELETE_KEY(int key) = 5;
        struct item EXIST(int key) = 6;
        struct item NUM_ITEMS() = 7;
    }=1;
}=99;