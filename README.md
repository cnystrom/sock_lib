# sock_lib: simple TCP and UDP sockets

Version 1.0 (2026). Split from NewIO 0.12, https://github.com/cnystrom/newio

sock_lib wraps BSD sockets for NewIO: a one-call TCP server (bind, listen,
accept), a TCP client, line and block reads and writes, a poll for waiting
data, and a small UDP client/server.

## API

From `sock_lib.h`:

| Function | What it does |
|---|---|
| `SOCKET init_tcp_server(int port_num)` | Binds every interface on `port_num`, waits for one connection and returns it. Blocks. |
| `SOCKET init_tcp_client(const char *hostname, unsigned short portnum)` | Connects. Returns `INVALID_SOCKET` (-1) on failure. |
| `int socket_data_available(SOCKET s, int timeout)` | `poll()` with a timeout in ms: `TRUE`, `FALSE`, or `ERROR`. |
| `void puts_socket(SOCKET s, char *str)` / `u_int gets_socket(SOCKET s, char *str, u_int max_len)` | Send a string, or receive one (NUL terminated). |
| `int read_socket(SOCKET s, byte *buf, int n)` / `int write_socket(SOCKET s, byte *buf, int n)` | Raw read; write that loops until all `n` bytes are sent. |
| `void close_socket(SOCKET s)` | Close. |
| `int init_udp_server(udp_sock_ptr s, int port)` / `int init_udp_inetd_server(udp_sock_ptr s)` | UDP server on a port, or on fd 0 under inetd. |
| `int init_udp_client(udp_sock_ptr s, const char *hostname, int port)` | UDP client, already connected to the server. |
| `int connect_udp(udp_sock_ptr s)` | Server side: connect to whoever sent the last datagram. |
| `int read_udp(...)`, `int write_udp(...)`, `void close_udp(...)` | Datagram I/O. |

`sock_lib.c` also has a public `read_socket_n()` (read exactly `n` bytes)
that is not in the header. `sock_lib_version.h` defines `SOCK_LIB_VERSION`
("1.0") and `SOCK_LIB_ORIGIN`.

## Dependencies

- **log_lib** (sibling project) for log messages. The `Makefile` looks in
  `../log_lib` (override with `LOG_LIB_DIR=`). Link order:
  `sock_lib.a log_lib.a`.
- Otherwise only the C library. On Linux, nothing extra is needed: the old
  `-lnsl` from NewIO is not used.

## Build

Check out log_lib next to this one, then:

```sh
make            # builds sock_lib.a
make test       # builds log_lib if needed, then runs test_sock_lib
make examples   # Chris's original interactive test programs, in examples/
make CC=clang test
make clean
```

When unpacked from the release tarballs, the directory names include the
version, so give the sibling paths:

```sh
make LOG_LIB_DIR=../log_lib-1.0 test
```

`make test` forks a TCP server and client on a loopback port (20000-39999,
chosen from the process ID), then sends UDP datagrams both ways.

## License

LGPL 2.1 or later, the same as NewIO. See `LICENSE` (GNU LGPL 2.1). `COPYING`
holds the GNU GPL 2, as in NewIO. Copyright Chris Nystrom; see the header of
each file for its years.

## Origin

Split from NewIO 0.12, https://github.com/cnystrom/newio (tag `newio-0.12`,
commit `ce54c5b`), directory `common/sock_lib/`. The library code is Chris
Nystrom's original, copied unchanged except for the lines listed under
"Changes from NewIO" below.

Contact: Chris Nystrom <cnystrom@gmail.com>

## Changes from NewIO

- `sock_lib.c:32`: `#include "nio_log.h"` became `#include "log_lib.h"`.
- `sock_lib.c:173`: bug fix, see "Fixed in 1.0".
- Removed two old personal contact lines (a web-log URL and a chat handle) from the file headers, at Chris's request (2026). The rest of each header and the authorship are unchanged, so header-area line numbers are 2 lower than in NewIO. This includes the `examples/`.
- `Makefile:28-29`: `-I.. -I../nio_log -I../win32_lib` became `-I.
  -I$(LOG_LIB_DIR)`. `win32_lib` was third-party code removed from NewIO
  (see below).
- `examples/`: NewIO's `sock_lib/test/*.c`, otherwise unchanged (Chris's
  public-domain test programs). The `make examples` rule replaces their old
  `Makefile`. They build with some `-Wpointer-sign` and
  `-Wstringop-truncation` warnings, left as they are.
- Added a copy of NewIO's `common/nio_types.h` (unchanged),
  `sock_lib_version.h`, `test_sock_lib.c`, the rules at the end of the
  `Makefile`, this README, `LICENSE` and `COPYING`.
- Not copied: `archive.sh` and `VisualC++/` (old Visual Studio projects that
  point into the NewIO tree).

## Fixed in 1.0

- **`accept_socket()` checked the wrong socket** (`sock_lib.c:173`). After
  `accept()` it tested the listening socket instead of the new one, so a
  failed `accept()` wasn't logged. It now checks the new socket.
  `test_sock_lib.c` interrupts `accept()` with a timer and checks for the
  log message.

## Known issues

Kept as in NewIO 0.12:

- `init_tcp_server()` never closes its listening socket. Each call accepts
  one client and leaks the listener.
- `sock_lib.c:103`: `establish_socket()` looks up the machine's own hostname,
  so a server fails on a machine whose hostname doesn't resolve.
- `gets_socket()` returns `u_int` but returns -1 on error.
- **Windows build is blocked:** `sock_lib.c:30` includes `win32_lib.h`, the
  third-party Downhill code removed from NewIO. Windows needs a small
  replacement, or winsock calls directly.
