
CFLAGS := -lrt -lpthread

lock: shmlock.h shm_provider.c shm_user.c
	gcc shm_provider.c -o provider -DUSE_LOCK $(CFLAGS)
	gcc shm_user.c -o user -DUSE_LOCK $(CFLAGS)

nolock: shmlock.h shm_provider.c shm_user.c
	gcc shm_provider.c -o provider $(CFLAGS)
	gcc shm_user.c -o user $(CFLAGS)

clean:
	-rm -rf provider user *.orig

beauty:
	astyle -xi \
	--style=linux -s -f -p -H -U -k3 -y -j -xC78 -r "*.h" "*.c"

.PHONY: lock nolock clean beauty
