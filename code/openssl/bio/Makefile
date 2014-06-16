CC = gcc
LDFLAGS = `pkg-config openssl --libs`
CFLAGS = -Wall `pkg-config openssl --cflags`

all: cat1 cat2

%: %.c
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
