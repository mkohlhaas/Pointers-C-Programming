CFLAGS  += -g
binaries = $(patsubst %.c,%,$(wildcard 13_*.c))

.PHONY: all clean

all: $(binaries)

list.o: list.h
dynarray.o: dynarray.h
13_2_higher-order-functions 13_5_stack: list.o
13_5_stack: dynarray.o

clean:
	@-rm -f $(binaries) *.o
