#ifndef _ECHO_COMMON_H
#define _ECHO_COMMON_H

static int setnonblock(int fd)
{
	int flags;

	flags = fcntl(fd, F_GETFL);
	flags |= O_NONBLOCK;
	return fcntl(fd, F_SETFL, flags);
}

#endif
