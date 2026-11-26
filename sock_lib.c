/******************************************************************************
 *
 *    sock_lib.c
 *
 *    Copyright (C) 2002, 2005  Chris Nystrom
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU Lessor General Public License as published
 *    by the Free Software Foundation; either version 2.1 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but without any warranty whatsoever.
 *
 *    For more license information see:
 *
 *      http://www.gnu.org/copyleft/lessor.html
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
#ifdef WIN32
#include <winsock2.h>
#include "win32_lib.h"
#endif				/*  */
#include "nio_log.h"
#include "sock_lib.h"

/*
#ifdef WIN32
__inline void bzero(char *to, int count);
#endif
*/

PRIVATE SOCKET establish_socket(uint16 portnum);
PRIVATE SOCKET open_socket(uint16 portnum);
PRIVATE SOCKET accept_socket(SOCKET s);

PRIVATE unsigned short pn;	/* port number */

/*
#ifdef WIN32
int init_winsock(void)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
 
	wVersionRequested = MAKEWORD( 2, 2 );
 
	err = WSAStartup( wVersionRequested, &wsaData );

	if ( err != 0 ) {
	// Tell the user that we could not find a usable
	// WinSock DLL.
		nio_log_error("init_winsock():", " WSAStartup() failed.\n");
	}

	return(err);
}
#endif  // init_winsock()
*/

PRIVATE SOCKET establish_socket(unsigned short portnum)
{
	int reuse_addr = 1;
	char myname[256];
	struct sockaddr_in sa;
	struct hostent *hp;
	SOCKET s;

#ifdef CHANGE_SOCKET_BUFFER_SIZE
	int sndbufsiz;
	int rcvbufsiz;
	int len;
#endif				/*  */

	memset(&sa, 0, sizeof(struct sockaddr_in));

	if (gethostname(myname, sizeof(myname))) {
#ifdef WIN32
		int error_code;
#endif

		nio_log_error("estblish_socket():",
			      " gethostname() failed.\n");

#ifdef WIN32
		error_code = WSAGetLastError();
		nio_log_error("establish_socket():", " error_code = %d\n",
			      error_code);
#endif
		return (-1);
	}
//      nio_log_print("establish_socket(): myname = |%s|\n", myname);

	hp = gethostbyname(myname);

	if (hp == NULL) {
		nio_log_error("establish_socket():",
			      " gethostbyname() failed.\n");
		return (INVALID_SOCKET);
	}

	sa.sin_family = hp->h_addrtype;
	sa.sin_port = htons(portnum);

	s = socket(AF_INET, SOCK_STREAM, 0);

	if (s == INVALID_SOCKET)
		return INVALID_SOCKET;

	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse_addr,
		   sizeof(reuse_addr));

#ifdef CHANGE_SOCKET_BUFFER_SIZE
	sndbufsiz = TCPSENDBUFFERSIZE;
	len = sizeof(sndbufsiz);
	setsockopt(s, SOL_SOCKET, SO_SNDBUF, &sndbufsiz, len);
	rcvbufsiz = TCPRECEIVEBUFFERSIZE;
	len = sizeof(rcvbufsiz);
	setsockopt(s, SOL_SOCKET, SO_RCVBUF, &rcvbufsiz, len);
#endif				/*  */

	if (bind(s, (struct sockaddr *) &sa, sizeof(struct sockaddr_in))
#ifdef WIN32
	    == SOCKET_ERROR)
#else				/*  */
	    < 0)
#endif				/*  */
	{

#ifdef WIN32
		closesocket(s);
#else				/*  */
		close(s);
#endif				/*  */

		return (INVALID_SOCKET);
	}

	listen(s, 3);		// max # of queued connects 
	return (s);
}

PRIVATE SOCKET open_socket(unsigned short portnum)
{
	SOCKET s;

	pn = portnum;

	if ((s = establish_socket(portnum)) == INVALID_SOCKET) {
		nio_log_error("open_socket():",
			      " establish_socket() failed.\n");
		return (-1);
	}

	return (s);
}

PRIVATE SOCKET accept_socket(SOCKET s)
{
	SOCKET new_sock;

	new_sock = accept(s, NULL, NULL);

	if (s == INVALID_SOCKET) {
		nio_log_print("Error waiting for new connection!\n");
		return (INVALID_SOCKET);
	}

	return (new_sock);
}

/*************************************************************************
 * Public Functions Below
 *************************************************************************/

PUBLIC void close_socket(SOCKET s)
{
#ifdef WIN32
	closesocket(s);
#else				/*  */
	close(s);
#endif				/*  */
}

PUBLIC void puts_socket(SOCKET s, char *str)
{
	int i;

	i = strlen(str);

	nio_log_print("OUT> %s\n", str);

	if (send(s, str, i, 0) == -1) {
		nio_log_error("puts_socket():", " send() failure.\n");
	}
}

PUBLIC u_int gets_socket(SOCKET s, char *str, u_int max_len)
{
	int i;

	if ((i = recv(s, str, max_len - 1, 0)) == -1) {
		nio_log_print("gets_socket():", "recv() error.\n");
		return (-1);
	}

	str[i] = '\0';

	nio_log_print("IN> %s\n", str);

	return (strlen(str));
}

PUBLIC SOCKET init_tcp_server(int port_num)
{
	SOCKET s, s_tmp;

#ifdef WIN32
	if (init_winsock()) {
		nio_log_error("init_tcp_server():",
			      " init_winsock() failed.\n");
		return (-1);
	}
#endif

	nio_log_print("init_tcp_server(): entering...\n");

	s_tmp = open_socket(port_num);

	nio_log_print("init_tcp_server(): s_tmp = %d\n", s_tmp);

	s = accept_socket(s_tmp);

	nio_log_print("init_tcp_server(): s = %d\n", s);

	return (s);
}

PUBLIC SOCKET init_tcp_client(const char *hostname, unsigned short portnum)
{
	struct sockaddr_in sa;
	struct hostent *hp;
	SOCKET s;

#ifdef WIN32
	if (init_winsock()) {
		nio_log_error("init_tcp_server():",
			      " init_winsock() failed.\n");
		return (-1);
	}
#endif

#ifdef CHANGE_SOCKET_BUFFER_SIZE
	int sndbufsiz;
	int rcvbufsiz;
	int len;
#endif				/*  */

	hp = gethostbyname(hostname);

	if (hp == NULL)
		return INVALID_SOCKET;

	memset(&sa, 0, sizeof(sa));

	memcpy((char *) &sa.sin_addr, hp->h_addr, hp->h_length);

	sa.sin_family = hp->h_addrtype;
	sa.sin_port = htons((u_short) portnum);

	s = socket(hp->h_addrtype, SOCK_STREAM, 0);

	if (s == INVALID_SOCKET)
		return INVALID_SOCKET;

#ifdef CHANGE_SOCKET_BUFFER_SIZE
	sndbufsiz = TCPSENDBUFFERSIZE;
	len = sizeof(sndbufsiz);
	setsockopt(s, SOL_SOCKET, SO_SNDBUF, &sndbufsiz, len);
	rcvbufsiz = TCPRECEIVEBUFFERSIZE;
	len = sizeof(rcvbufsiz);
	setsockopt(s, SOL_SOCKET, SO_RCVBUF, &rcvbufsiz, len);
#endif				/*  */

	if (connect(s, (struct sockaddr *) &sa, sizeof sa)
#ifdef WIN32
	    == SOCKET_ERROR)
#else				/*  */
	    < 0)
#endif				/*  */
	{
		close_socket(s);
		return INVALID_SOCKET;
	}

	return s;
}

PUBLIC int read_socket(SOCKET s, byte * buf, int n)
{
#ifdef WIN32
	return (recv(s, buf, n, 0));
#else				/*  */
	return (read(s, buf, n));
#endif				/*  */
}

PUBLIC int read_socket_n(SOCKET s, byte * buf, int n)
{
	int bcount;		/* counts bytes read */
	int br;			/* bytes read this pass */

	bcount = 0;
	br = 0;

	while (bcount < n) {	// loop until full buffer 
		nio_log_print("bcount = %d\n", bcount);

#ifdef WIN32
		if ((br = recv(s, buf, n - bcount, 0)) > 0) {
#else				/*  */
		if ((br = read(s, buf, n - bcount)) > 0) {
#endif				/*  */
			bcount += br;	// increment byte counter 
			buf += br;	// move buffer ptr for next read 
		} else if (br < 0)	// signal an error to the caller 
			return (-1);
	}
	return (bcount);
}

PUBLIC int write_socket(SOCKET s, byte * buf, int n)
{
	int nleft, nwritten;

	nleft = n;

	while (nleft > 0) {
#ifdef WIN32
		nwritten = send(s, buf, nleft, 0);
#else				/*  */
		nwritten = write(s, buf, nleft);
#endif				/*  */

		if (nwritten <= 0)
			return (nwritten);	// error 

		nleft -= nwritten;
		buf += nwritten;
	}

	return (n - nleft);
}

PUBLIC int socket_data_available(SOCKET s, int timeout)
{
	int ret;
	struct pollfd ufds[1];

	ufds[0].fd = s;
	ufds[0].events = POLLIN;

	ret = poll(ufds, 1, timeout);

	if (-1 == ret) {
		return (ERROR);
	} else if (0 == ret) {
		return (FALSE);	/* No data available */
	}

	if (ufds[0].revents & POLLIN) {
		return (TRUE);	/* Data available */
	}

	return 0;		/* Should not get here, but... */
}

/***************************************************************************
 *  UDP Stuff below
 ***************************************************************************/

PRIVATE socklen_t addrlen;

PRIVATE void init_udp_socket(udp_sock_ptr s)
{
	s->sockid = -1;

	addrlen = sizeof(struct sockaddr_in);

	bzero((char *) &s->local_addr, addrlen);
	bzero((char *) &s->remote_addr, addrlen);

	s->connected = FALSE;
}

PUBLIC int init_udp_server(udp_sock_ptr s, int port)
{
	init_udp_socket(s);

	if ((s->sockid = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		nio_log_print("init_udp_server(): socket error: %d\n",
			      errno);
		return (-1);
	}

	s->local_addr.sin_family = AF_INET;
	s->local_addr.sin_addr.s_addr = htons(INADDR_ANY);
	s->local_addr.sin_port = htons(port);

	if ((bind(s->sockid, (struct sockaddr *) &s->local_addr, addrlen)
	     < 0)) {
		nio_log_print("init_udp_server(): bind failed: %d\n",
			      errno);
		return (-2);
	}

	return (s->sockid);
}


/* inetd handles most of setup */
PUBLIC int init_udp_inetd_server(udp_sock_ptr s)
{
	init_udp_socket(s);
	s->sockid = 0;
	return (s->sockid);
}

PUBLIC int init_udp_client(udp_sock_ptr s, const char *hostname, int port)
{
	int ret;
	struct hostent *hp;

	hp = gethostbyname(hostname);

	if (hp == NULL) {
		return (-1);
	}

	init_udp_socket(s);

	memcpy((char *) &s->remote_addr.sin_addr, hp->h_addr,
	       hp->h_length);

	s->remote_addr.sin_family = hp->h_addrtype;
	s->remote_addr.sin_port = htons(port);

	if ((s->sockid = socket(hp->h_addrtype, SOCK_DGRAM, 0)) < 0) {
		nio_log_print("init_udp_client(): socket failed: %d\n",
			      errno);
		return (-2);
	}

	s->local_addr.sin_family = AF_INET;
	s->local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s->local_addr.sin_port = htons(0);

	if ((bind(s->sockid, (struct sockaddr *) &s->local_addr, addrlen)
	     < 0)) {
		nio_log_print("init_udp_client(): bind failed: %d\n",
			      errno);
		return (-3);
	}

	/* client knows who to talk to */
	ret = connect_udp(s);

	if (ret < 0) {
		return (-4);
	}

	return (s->sockid);
}


/* must call after read_udp() on server so server knows who to connect to */
PUBLIC int connect_udp(udp_sock_ptr s)
{
	int ret;

	ret =
	    connect(s->sockid, (struct sockaddr *) &s->remote_addr,
		    addrlen);

	if (ret < 0) {
		nio_log_print("connect_udp(): connect failed: %d\n", ret);
		return (-1);
	} else {
		s->connected = TRUE;
	}

	return (ret);
}

PUBLIC int read_udp(udp_sock_ptr s, byte_ptr msg, int len)
{
	int cc;

	/* note: this call fills in the remote address info */
	cc = recvfrom(s->sockid, msg, len, 0,
		      (struct sockaddr *) &s->remote_addr, &addrlen);

	usleep(0);		// strangly this seems to help...

	return (cc);
}

PUBLIC int write_udp(udp_sock_ptr s, const byte_ptr msg, int len)
{
	int retcode;

	retcode = sendto(s->sockid, msg, len, 0,
			 (struct sockaddr *) &s->remote_addr, addrlen);

	if (retcode <= -1) {
		nio_log_print("write_udp(): sendto failed: %d\n", errno);
		return (-1);
	}

	return (retcode);
}

PUBLIC void close_udp(udp_sock_ptr s)
{
#ifdef WIN32
	closesocket(s->sockid);
#else				/*  */
	close(s->sockid);
#endif				/*  */
}

/*
#ifdef WIN32
__inline void bzero(to, count)
char *to;
int count;
{
	while (count-- > 0) {
		*to++ = 0;
	}
}

#endif
*/
