/******************************************************************************
 *
 *    tcp_server.c
 *
 *    A TCP server to test the sock_lib.
 *
 *    Copyright (C) 2005  Chris Nystrom
 *
 *    This program is pulbic domain.
 *
 *    Contact Author at:
 *
 *    Chris Nystrom
 *
 *    E-Mail: cnystrom@gmail.com
 *
 *    Soli Deo Gloria
 *
 */

#include "sock_lib.h"

#define PORT  (6090)
#define MAX_SIZE     (50)

int main(void)
{
	int i = 0, ret;
	char message[MAX_SIZE];
	SOCKET s;

	s = init_tcp_server(PORT);

	while (!socket_data_available(s, 1000)) {
		printf("waiting for data: %d\n", i++);
	}

	ret = read_socket(s, message, MAX_SIZE);

	if (ret > 0) {
		printf("server: message is |%s|\n", message);
	} else {
		printf("server: error. nothing read\n");
	}

	close_socket(s);

	exit(0);
}
