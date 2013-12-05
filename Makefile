
CFLAGS := -lrt -lpthread

lock: shmlock.h shm_provider.c shm_user.c
	gcc shm_provider.c -o provider -DUSE_LOCK $(CFLAGS)
	gcc shm_user.c -o user -DUSE_LOCK $(CFLAGS)

nolock: shmlock.h shm_provider.c shm_user.c
	gcc shm_provider.c -o provider-nolock $(CFLAGS)
	gcc shm_user.c -o user-nolock $(CFLAGS)

clean:
	-rm -rf provider user provider-nolock user-nolock *.orig

test: lock
	./provider &
	sleep 1
	./user; ./user; ./user

test-nolock: nolock
	./provider-nolock &
	sleep 1
	./user-nolock; ./user-nolock; ./user-nolock

beauty:
	astyle -xi \
	--style=linux -s -f -p -H -U -k3 -y -j -xC78 -r "*.h" "*.c"

.PHONY: lock nolock clean beauty
