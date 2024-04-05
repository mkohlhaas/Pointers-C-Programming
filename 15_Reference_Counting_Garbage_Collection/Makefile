CFLAGS  += -g -pg

binaries = $(patsubst %.c,%,$(wildcard 15_*.c))

.PHONY: all clean

all: $(binaries)

refcount.o: refcount.h strees.h
15_4_strees: refcount.o

clean:
	@-rm -f $(binaries) *.o
