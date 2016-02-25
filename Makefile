CC?=gcc
CFLAGS?=-g
LDFLAGS?=
LIBS?=

all: io_test

io_test:io_test.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o io_test io_test.o $(LIBS)

clean:
	rm -rf *.o io_test