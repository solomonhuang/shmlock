
#define MY_SHARE_OBJ "/myshareobj"

struct my_share_mem {
    uint64_t value;
    pthread_mutex_t lock;
};
