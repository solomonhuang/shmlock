#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/mman.h>

#include "shmlock.h"


int main(int argc, char *argv[])
{
    int fd, i;
    int count = 1000000;
    struct my_share_mem *my_shared_memory;

    daemon(1, 1);

    fd = shm_open(MY_SHARE_OBJ, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    if (argc > 1) {
        count = atoi(argv[1]);
    }

    if (fd == -1) {
        printf("shm_open\n");
        return -1;
    }

    my_shared_memory = mmap(NULL, sizeof(struct my_share_mem),
                            PROT_READ | PROT_WRITE, MAP_SHARED , fd, 0);

    if (my_shared_memory == MAP_FAILED) {
        printf("mmap\n");
        return -1;
    }

    close(fd);

    for (i = 0; i < count; i++) {
#ifdef USE_LOCK
        pthread_mutex_lock(&my_shared_memory->lock);
#endif
        my_shared_memory->value = my_shared_memory->value + 1;
#ifdef USE_LOCK
        pthread_mutex_unlock(&my_shared_memory->lock);
#endif
    }

    printf("user[%5d]: add %lu\n", getpid(), count);

    munmap(my_shared_memory, sizeof(struct my_share_mem));
    return 0;
}
