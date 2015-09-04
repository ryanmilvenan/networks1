CC = gcc
CFLAGS = -Wall -g -v

OBJS = nethelp.o server.o client.o 

all: server client 

nethelp.o: nethelp.c
	$(CC) $(CFLAGS) -c nethelp.c

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

server: server.o nethelp.o
	$(CC) $(LDFLAGS) -o $@ server.o nethelp.o

client: client.o nethelp.o
	$(CC) $(LDFLAGS) -o $@ client.o nethelp.o

clean:
	rm -f server client *.o *~ core
