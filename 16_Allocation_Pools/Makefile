CFLAGS  += -g

binaries = $(patsubst %.c,%,$(wildcard 1*.c))

.PHONY: all clean

all: $(binaries)

clean:
	@-rm -f $(binaries)
