/* test_sock_lib.c - exercises sock_lib over loopback (2026).
 * Run with `make test`. Uses fork(): the parent is the TCP server, the
 * child the TCP client. The UDP part runs in one process. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>
#include "sock_lib.h"
#include "log_lib.h"
#include "sock_lib_version.h"

static int n_pass, n_fail;
#define CHECK(c) do { if (c) { n_pass++; printf("  ok   %s\n", #c); } \
	else { n_fail++; printf("  FAIL %s  (%s:%d)\n", #c, __FILE__, __LINE__); } } while (0)
#define DONE(name) do { printf("%s: %d passed, %d failed\n", name, n_pass, n_fail); \
	return n_fail ? 1 : 0; } while (0)

#define LOG_FILE "test_sock_lib.log"

static void on_timer(int sig)
{
	(void) sig;
}

static int tcp_child(int port)
{
	SOCKET c = INVALID_SOCKET;
	char buf[64];
	int i, n;

	for (i = 0; i < 100 && c == INVALID_SOCKET; i++) {
		c = init_tcp_client("127.0.0.1", port);
		if (c == INVALID_SOCKET)
			usleep(50000);
	}
	if (c == INVALID_SOCKET)
		return 2;
	if (write_socket(c, (byte *) "ping", 4) != 4)
		return 3;
	n = gets_socket(c, buf, sizeof(buf));	/* server replies "pong" */
	close_socket(c);
	return (n == 4 && strcmp(buf, "pong") == 0) ? 0 : 4;
}

int main(void)
{
	int port = 20000 + (getpid() % 20000);
	SOCKET s;
	pid_t pid;
	int status = -1;
	byte buf[64];
	udp_sock srv, cli;

	printf("sock_lib %s (%s)\n", SOCK_LIB_VERSION, SOCK_LIB_ORIGIN);

	/* 1.1 fix: a failed accept() was not noticed (it checked the
	 * listening socket). Interrupt accept() with a timer and check that
	 * init_tcp_server() logs the error and returns INVALID_SOCKET. */
	{
		struct sigaction sa;
		struct itimerval it;
		FILE *f;
		char line[128];
		int found = 0;

		memset(&sa, 0, sizeof(sa));
		sa.sa_handler = on_timer;	/* no SA_RESTART: EINTR */
		sigaction(SIGALRM, &sa, NULL);
		memset(&it, 0, sizeof(it));
		it.it_value.tv_usec = 300000;
		log_lib_open(LOG_FILE);
		setitimer(ITIMER_REAL, &it, NULL);
		s = init_tcp_server(port + 2);	/* nobody connects */
		log_lib_close();
		signal(SIGALRM, SIG_DFL);
		CHECK(s == INVALID_SOCKET);
		f = fopen(LOG_FILE, "r");
		while (f != NULL && fgets(line, sizeof(line), f) != NULL)
			if (strstr(line, "Error waiting for new connection"))
				found = 1;
		if (f != NULL)
			fclose(f);
		CHECK(found);
		remove(LOG_FILE);
	}

	log_lib_use_stdout(FALSE);	/* quiet: library logs nowhere */
	alarm(20);

	/* TCP */
	pid = fork();
	if (pid == 0)
		_exit(tcp_child(port));
	s = init_tcp_server(port);	/* blocks in accept() */
	CHECK(s != INVALID_SOCKET && s >= 0);
	CHECK(socket_data_available(s, 5000) == TRUE);
	memset(buf, 0, sizeof(buf));
	CHECK(read_socket(s, buf, 4) == 4 && memcmp(buf, "ping", 4) == 0);
	puts_socket(s, "pong");
	waitpid(pid, &status, 0);
	CHECK(WIFEXITED(status) && WEXITSTATUS(status) == 0);
	close_socket(s);

	/* UDP */
	port++;
	CHECK(init_udp_server(&srv, port) >= 0);
	CHECK(init_udp_client(&cli, "127.0.0.1", port) >= 0);
	CHECK(write_udp(&cli, (byte_ptr) "hello", 5) == 5);
	memset(buf, 0, sizeof(buf));
	CHECK(read_udp(&srv, buf, sizeof(buf)) == 5
	      && memcmp(buf, "hello", 5) == 0);
	CHECK(write_udp(&srv, (byte_ptr) "back", 4) == 4);
	memset(buf, 0, sizeof(buf));
	CHECK(read_udp(&cli, buf, sizeof(buf)) == 4
	      && memcmp(buf, "back", 4) == 0);
	close_udp(&srv);
	close_udp(&cli);

	DONE("sock_lib");
}
