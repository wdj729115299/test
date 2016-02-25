#include <stdio.h>
#include <stdlib.h>
#include "ev.h"

static void stdin_callback(struct ev_loop *loop, ev_io *w, int revents)
{
	char str[1024];

	if(revents & EV_READ){
		printf("there is something to read:\n");
		scanf("%s", str);
		ev_io_stop(loop, w);
	}else if(revents & EV_WRITE){
		printf("there is something to write:\n");
		
	}
}

int main(int argc, char *argv[])
{
	struct ev_loop * main_loop = ev_default_loop(0);

	ev_io stdin_watcher;

	ev_init(&stdin_watcher, stdin_callback);
	ev_io_set(&stdin_watcher, STDIN_FILENO, EV_READ|EV_WRITE);
	ev_io_start(main_loop, &stdin_watcher);

	ev_set_io_collect_interval(main_loop, 2);
	ev_run(main_loop, 0);
	printf("main:%d\n", ev_is_active(&stdin_watcher));

	return 0;
}
