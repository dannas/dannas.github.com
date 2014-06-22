CC = gcc
RM = rm
LDFLAGS = `pkg-config openssl --libs`
CFLAGS = -Wall `pkg-config openssl --cflags`
PROGRAMS = cat1 cat2 daytimeclient daytimeserver echoserver echoclient daytimeclient2

all: $(PROGRAMS)

%: %.c
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(PROGRAMS)
