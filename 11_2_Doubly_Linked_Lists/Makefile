CFLAGS  += -g
binaries = $(patsubst %.c,%,$(wildcard 0*.c))

.PHONY: all clean

all: $(binaries)

$(binaries): list.o
list.o: list.h

clean:
	@-rm -f $(binaries) *.o
