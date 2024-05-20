CC=gcc
CFLAGS=-Wall -Werror -pthread
LDFLAGS=-pthread

all: echo_server echo_client call_center client

echo_server:Task1/echo_server.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o echo_server Task1/echo_server.c

echo_client:Task1/echo_client.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o echo_client Task1/echo_client.c

call_center:Task2/call_center.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o call_center Task2/call_center.c

client:Task2/client.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o client Task2/client.c

clean:
	rm -f echo_server echo_client call_center client
