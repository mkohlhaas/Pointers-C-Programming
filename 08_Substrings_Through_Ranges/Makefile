CFLAGS  += -g

binaries = $(patsubst %.c,%,$(wildcard 8_*.c))

.PHONY: all clean

all: $(binaries)

$(binaries): substr.o
substr.o list.o: substr.h

clean:
	@-rm -f $(binaries) *.o
