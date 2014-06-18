CC = gcc
RM = rm
LDFLAGS = `pkg-config openssl --libs`
CFLAGS = -Wall `pkg-config openssl --cflags`
PROGRAMS = cat1 cat2 daytimeclient daytimeserver

all: $(PROGRAMS)

%: %.c
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(PROGRAMS)
