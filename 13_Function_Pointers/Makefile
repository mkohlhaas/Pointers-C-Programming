CFLAGS  += -g
binaries = $(patsubst %.c,%,$(wildcard [01]*.c))

.PHONY: all clean

all: $(binaries)

list.o: list.h
dynarray.o: dynarray.h
098_higher-order-functions 101_stack: list.o
101_stack: dynarray.o

clean:
	@-rm -f $(binaries) *.o
