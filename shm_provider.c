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

bool stop = false;


void term(int sig)
{
    stop = true;
}

int main(int argc, char *argv[])
{
    int fd;
    struct my_share_mem *my_shared_memory;
	pthread_mutexattr_t attr;

    fd = shm_open(MY_SHARE_OBJ, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

    if (fd == -1) {
        printf("shm_open\n");
        return -1;
    }

    ftruncate(fd, sizeof(struct my_share_mem));
    my_shared_memory = mmap(NULL, sizeof(struct my_share_mem),
                            PROT_READ | PROT_WRITE, MAP_SHARED , fd, 0);

    if (my_shared_memory == MAP_FAILED) {
        printf("mmap\n");
        return -1;
    }

    close(fd);
#ifdef USE_LOCK
    pthread_mutex_init(&my_shared_memory->lock, &attr);
#endif
    my_shared_memory->value = 0;

    signal(SIGINT, term);
    signal(SIGTERM, term);

    while (!stop) {
        sleep(1);
#ifdef USE_LOCK
        pthread_mutex_lock(&my_shared_memory->lock);
#endif
        printf("provider[%5d]: %lu\n", getpid(), my_shared_memory->value);
#ifdef USE_LOCK
        pthread_mutex_unlock(&my_shared_memory->lock);
#endif
    }

    munmap(my_shared_memory, sizeof(struct my_share_mem));
    shm_unlink(MY_SHARE_OBJ);
    return 0;
}
