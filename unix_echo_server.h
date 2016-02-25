#ifndef _UNIX_SERVER_ECHO_H
#define _UNIX_SERVER_ECHO_H

#include "array_heap.h"

struct sock_ev_serv{
	ev_io eio;
	int fd;
	struct sockaddr_in addr_in;
	int socket_len;
	array clients;
};

struct sock_ev_client{
	ev_io eio;
	int fd;
	int index;
	struct sock_ev_server *server;
};
#endif
