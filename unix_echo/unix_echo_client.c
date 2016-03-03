#include <stdlib.h>
#include <stdio.h>
#include <ev.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "echo_common.h"

struct ev_loop *main_loop;
ev_io stdin_watcher;
ev_io connect_w;
ev_io message_w;
int sd;
char *line = NULL;
size_t len = 0;

static void message_cb(struct ev_loop *loop, ev_io *w, int revents)
{
	if(revents & EV_WRITE){
		if(send(sd, line, len, 0) < 0){
			printf("echo send.\n");
			return;
		}

		ev_io_stop(loop, &message_w);
		ev_io_set(&message_w, sd, EV_READ);
		ev_io_start(loop, &message_w);
	}else if(revents & EV_READ){
		int n;
		char str[100] = {0};

		n = recv(sd, str, 100, 0);
		if(n <= 0){
			if(n == 0){
				puts("orderly disconnect");
				ev_io_stop(loop, &message_w);
				close(sd);
			}else if(EAGAIN == errno){
				puts("should never get in this state with libev.");
			}else{
				printf("recv");
			}

			return;
		}
		puts("[Remote]");
		puts(str);
	}
}


static void stdin_cb(struct ev_loop *loop, ev_io *w, int revents)
{
	int len2;

	len2 = getline(&line, &len, stdin);
	//ev_io_stop(loop, &message_w);
	//ev_io_set(&message_w, remote_fd, EV_READ | EV_WRITE);
	ev_io_start(loop, &message_w);
}

static void connect_cb(struct ev_loop *loop, ev_io *w, int revents)
{
	puts("connected now watching stdin!");
	ev_io_stop(loop, &connect_w);
	ev_io_start(loop, &stdin_watcher);
}

static void connection_new(void)
{
	int len;
	struct sockaddr_in addr;
	
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		printf("create socket failed.\n");
		return;
	}

	ev_io_init(&connect_w, connect_cb, sd, EV_WRITE);
	ev_io_start(main_loop, &connect_w);

	ev_io_init(&message_w, message_cb, sd, EV_READ | EV_WRITE);
	//ev_io_start(main_loop, &message_w);

	ev_io_init(&stdin_watcher, stdin_cb, 0, EV_READ);
	//ev_io_start(main_loop,  &stdin_watcher);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(22345);
	addr.sin_addr.s_addr = inet_addr("200.200.79.142");

	if(connect(sd, (struct sockaddr*)&addr, sizeof(addr)) < 0){
		perror("connected server failed.\n");
		return;
	}

	if(setnonblock(sd) < 0){
		printf("setnonblock failed.\n");
		return;
	}
}

int main(void)
{
	main_loop = ev_default_loop(0);

	setnonblock(0);

	connection_new();

	ev_run(main_loop, 0);

	return 0;
}
