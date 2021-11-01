 CFLAGS  += -g
LDFLAGS += -lm

binaries = $(patsubst %.c,%,$(wildcard *.c))

.PHONY: all clean

all: $(binaries)

clean:
	@-rm -f $(binaries)
