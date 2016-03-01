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
ev_io remote_w;
ev_io send_w;
int remote_fd;
char *line = NULL;
size_t len = 0;

static void send_cb(struct ev_loop *loop, ev_io *w, int revents)
{
	if(revents & EV_WRITE){
		if(send(remote_fd, line, len, 0) < 0){
			printf("echo send.\n");
			return;
		}

		ev_io_stop(loop, &send_w);
		ev_io_set(&send_w, remote_fd, EV_READ);
		ev_io_start(loop, &send_w);
	}else if(revents & EV_READ){
		int n;
		char str[100] = ".\0";

		printf("[r, remote]");

		n = recv(remote_fd, str, 100, 0);
		if(n <= 0){
			if(n == 0){
				puts("orderly disconnect");
				ev_io_stop(loop, &send_w);
				close(remote_fd);
			}else if(EAGAIN == errno){
				puts("should never get in this state with libev.");
			}else{
				printf("recv");
			}

			return;
		}
		printf("socket client said:%s", str);
	}
}

static void stdin_cb(struct ev_loop *loop, ev_io *w, int revents)
{
	int len2;
	puts("stdin written to, reading...");

	len2 = getline(&line, &len, stdin);
	ev_io_stop(loop, &send_w);
	ev_io_set(&send_w, remote_fd, EV_READ | EV_WRITE);
	ev_io_start(loop, &send_w);
}

static void remote_cb(struct ev_loop *loop, ev_io *w, int revents)
{
	puts("connected now watching stdin");

	ev_io_start(loop,  &stdin_watcher);
	ev_io_stop(loop,  &remote_w);
}

static void connection_new(void)
{
	int len;
	struct sockaddr_in remote;
	
	remote_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(remote_fd < 0){
		printf("create socket failed.\n");
		return;
	}

	if(setnonblock(remote_fd) < 0){
		printf("setnonblock failed.\n");
		return;
	}

	ev_io_init(&remote_w, remote_cb, remote_fd, EV_WRITE);
	ev_io_start(main_loop, &remote_w);

	ev_io_init(&send_w, send_cb, remote_fd, EV_READ);
	ev_io_start(main_loop, &send_w);

	remote.sin_family = AF_INET;
	remote.sin_port = htons(22345);
	remote.sin_addr.s_addr = inet_addr("200.200.79.142");

	if(connect(remote_fd, (struct sockaddr*)&remote, sizeof(remote)) < 0){
		perror("connected server failed.\n");
		return;
	}
	
}

int main(void)
{
	main_loop = ev_default_loop(0);

	setnonblock(0);

	ev_io_init(&stdin_watcher, stdin_cb, 0, EV_READ);

	connection_new();

	ev_run(main_loop, 0);

	return 0;
}
