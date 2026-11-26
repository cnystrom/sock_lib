/******************************************************************************
 *
 *    tcp_client.c
 *
 *    A TCP client to test the sock_lib.
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

#include <string.h>
#include "sock_lib.h"

#define HOSTNAME_SIZE  (128)
#define PORT  (6090)

int main(int argc, char *argv[])
{
	int ret;
	char message[12] = "Hello world";
	char hostname[HOSTNAME_SIZE];
	SOCKET s;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <hostname>\n", argv[0]);
		exit(1);
	}

	strncpy(hostname, argv[1], HOSTNAME_SIZE);

	s = init_tcp_client(hostname, PORT);

	sleep(10);

	ret = write_socket(s, message, (strlen(message) + 1));

	printf("client: %d bytes written\n", ret);

	close_socket(s);

	exit(0);
}
