CFLAGS  += -g
binaries = $(patsubst %.c,%,$(wildcard 1*.c))

.PHONY: all clean

all: $(binaries)

$(binaries): list.o stree.o
list.o: list.h
stree.o: stree.h

clean:
	@-rm -f $(binaries) *.o
