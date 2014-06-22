CC = gcc
RM = rm
LDFLAGS = `pkg-config openssl --libs`
CFLAGS = -Wall `pkg-config openssl --cflags`
PROGRAMS:= $(patsubst %.c, %, $(wildcard *.c))

all: $(PROGRAMS)

%: %.c
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(PROGRAMS)
