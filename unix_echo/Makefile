CC?=gcc
CFLAGS?=-g -I/usr/local/libev_4.22/include
LDFLAGS?=-L/usr/local/libev_4.22/lib
LIBS?=-lev

all:echo_client echo_server
.PHONY:all

target1 = echo_client
objects1 = unix_echo_client.o

target2 = echo_server
objects2 = unix_echo_server.o array_heap.o

$(target1):$(objects1)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

$(objects1):%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(target2):$(objects2)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

$(objects2):%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY:clean
	
clean:
	rm -rf *.o $(target1) $(target2)
