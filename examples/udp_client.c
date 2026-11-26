/******************************************************************************
 *
 *    udp_client.c
 *
 *    A UDP client to test the udp_lib.
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
	udp_sock s;
	int ret;
	char message[12] = "Hello world";
	char hostname[HOSTNAME_SIZE];
	if (argc != 2) {
		fprintf(stderr, "usage: %s <hostname>\n", argv[0]);
		exit(1);
	}
	strncpy(hostname, argv[1], HOSTNAME_SIZE);
	init_udp_client(&s, hostname, PORT);
	ret = write(s.sockid, message, (strlen(message) + 1));
	if (ret < 0) {
		printf("write error\n");
	} else {
		printf("sent %d bytes\n", ret);
	}
	ret = read(s.sockid, message, 12);
	if (ret > 0) {
		printf("message = |%s|\n", message);
	} else {
		printf("no message back\n");
	}
	ret = write(s.sockid, "Thanks!", 8);
	if (ret < 0) {
		printf("write error\n");
	} else {
		printf("sent %d bytes\n", ret);
	}
	close_udp(&s);
	exit(0);
}
