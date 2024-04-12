# CFLAGS  += -g -DNDEBUG
CFLAGS  += -g -fsanitize=address

binaries = $(patsubst %.c,%,$(wildcard 11_2_*.c))

.PHONY: all clean

all: $(binaries)

$(binaries): list.o
list.o: list.h

clean:
	@-rm -f $(binaries) *.o
