CFLAGS  += -g -pg

binaries = $(patsubst %.c,%,$(wildcard 1*.c))

.PHONY: all clean

all: $(binaries)

refcount.o: refcount.h strees.h
112_strees: refcount.o

clean:
	@-rm -f $(binaries) *.o
