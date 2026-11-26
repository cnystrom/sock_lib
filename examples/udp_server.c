/******************************************************************************
 *
 *    udp_server.c
 *
 *    A UDP server to test the udp_lib.
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
int main()
{
	udp_sock s;
	int ret;
	char message[MAX_SIZE];
	init_udp_server(&s, PORT);
	ret = read_udp(&s, message, MAX_SIZE);
	if (ret > 0) {
		printf("message is |%s|\n", message);
	} else {
		printf("error. nothing read\n");
	}

	/* must call after first read_udp() so server knows who to connect to */
	connect_udp(&s);
	ret = write(s.sockid, "got it!", 8);
	if (ret < 0) {
		printf("write error\n");
	} else {
		printf("sent %d bytes\n", ret);
	}
	ret = read(s.sockid, message, MAX_SIZE);
	if (ret > 0) {
		printf("message is |%s|\n", message);
	} else {
		printf("error. nothing read\n");
	}
	close_udp(&s);
	exit(0);
}
