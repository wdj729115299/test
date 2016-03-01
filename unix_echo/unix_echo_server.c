#include <stdlib.h>
#include <stdio.h>
#include <ev.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include "unix_echo_server.h"
#include "array_heap.h"
#include "echo_common.h"

static void non_blocked(struct ev_loop *loop, ev_periodic *w, int revents)
{
	
}

static void client_cb(struct ev_loop *loop, ev_io *w, int revents)
{
	int n;
	
	struct sock_ev_client *client = (struct sock_ev_client*)w;
	char str[100] = ".\0";

	printf("[r]");
	n = recv(client->fd, str, 100, 0);
	if(n <= 0){
		if(n == 0){
			puts("orderly disconnect");
			ev_io_stop(loop, &client->eio);
			close(client->fd);
		}else if(EAGAIN == errno){
			puts("should nerver get in this state with libev");
		}else{
			printf("recv");
		}

		return;
	}

	printf("socket client said %s", str);

	if(send(client->fd, str, n, 0) < 0){
		printf("send failed.\n");
	}
}

inline static struct sock_ev_client *client_new(int fd)
{
	struct sock_ev_client *client;

	client = calloc(1, sizeof(struct sock_ev_client));
	client->fd = fd;
	setnonblock(client->fd);
	ev_io_init(&client->eio, client_cb, client->fd, EV_READ);

	return client;
}

static void server_cb(struct ev_loop *loop, ev_io *w, int revents)
{
	puts("unix stream socket has become readable");

	int client_fd;
	struct sock_ev_client *client;
	struct sock_ev_serv *server = (struct sock_ev_serv*)w;

	while(1){
		client_fd = accept(server->fd, NULL, NULL);
		if(client_fd < 0){
			if(errno != EAGAIN && errno != EWOULDBLOCK)
			{
				printf("accept failed errno = %d %s", errno, strerror(errno));
				exit(EXIT_FAILURE);
			}
			break;
		}
		puts("accept a client");
		client = client_new(client_fd);
		client->server = server;
		client->index = array_push(&server->clients, (void*)client);
		ev_io_start(loop, &client->eio);
	}
}

static int unix_socket_init(struct sockaddr_in *addr_in, int maxqueue)
{
	int fd;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1){
		printf("socket failed");
		exit(EXIT_FAILURE);
	}

	if(setnonblock(fd) < 0){
		printf("echo server socket ");
		exit(EXIT_FAILURE);
	}

	addr_in->sin_family = AF_INET;
	addr_in->sin_port = htons(22345);
	addr_in->sin_addr.s_addr = htonl(INADDR_ANY);

	return fd;
}

static int server_init(struct sock_ev_serv *server, int max_queue)
{
	server->fd = unix_socket_init(&server->addr_in, max_queue);
	server->socket_len = sizeof(struct sockaddr_in);

	array_init(&server->clients, 128);

	if(bind(server->fd, (struct sockaddr*)&server->addr_in, server->socket_len) < 0){
		printf("bind server socket failed.\n");
		exit(EXIT_FAILURE);
	}

	if(listen(server->fd, max_queue) < 0){
		printf("listen failed.\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}

int main(void)
{
	struct sock_ev_serv server;
	struct ev_periodic every_few_seconds;
	
	struct ev_loop *main_loop = ev_default_loop(0);
	int max_queue = 128;

	server_init(&server, max_queue);
	
	ev_periodic_init(&every_few_seconds, non_blocked, 0, 5, 0);
	ev_periodic_start(main_loop, &every_few_seconds);

	ev_io_init(&server.eio, server_cb, server.fd, EV_READ);
	ev_io_start(main_loop, &server.eio);

	puts("unix_echo_socket starting...\n");
	ev_loop(main_loop, 0);

	close(server.fd);
	return EXIT_SUCCESS;
}
