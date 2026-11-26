
TARGETS =	$(SOCK_LIB_BIN)

#==============================================================================

SOCK_LIB_PATH =	.
SOCK_LIB_BIN =	sock_lib.a

SOCK_LIB_H =     \
                sock_lib.h \
                $(NULL)

SOCK_LIB_C =     \
                sock_lib.c \
                $(NULL)

SOCK_LIB_OBJS =  \
                ${SOCK_LIB_C:.c=.o} \
                $(NULL)

#==============================================================================

CC =		gcc

COPTS =		-O3 -g -Wall 

INCLUDES = 	\
		-I. \
		-I$(LOG_LIB_DIR) \
		$(NULL)

LIBDIRS = 	\
		$(NULL)

ifndef $(TARGET_OS)
TARGET_OS =     UNIX
endif
 
CFLAGS =	$(COPTS) $(INCLUDES) -DDEBUG -D$(TARGET_OS)

LFLAGS =	$(LIBDIRS)

#default compilation rules
%.o:		%.c
		$(CC) -c $< $(CFLAGS) -o $@

all: 		$(TARGETS)

default:	$(TARGETS)

new:		clean default

#==============================================================================

$(SOCK_LIB_BIN): $(SOCK_LIB_OBJS) $(SOCK_LIB_H) $(SOCK_LIB_C)
		rm -f $(SOCK_LIB_BIN)
		ar rcs $(SOCK_LIB_BIN) $(SOCK_LIB_OBJS)
		ranlib $(SOCK_LIB_BIN)

indent:
		dos2unix *.h *.c
		indent -kr -i4 *.h *.c 

clean:
		rm -f $(TARGETS) *.o *.a core a.out *~ .*.swp

#==============================================================================
# Standalone build (2026): sibling projects and `make test`

LOG_LIB_DIR ?=	../log_lib

$(LOG_LIB_DIR)/log_lib.a:
		$(MAKE) -C $(LOG_LIB_DIR)

TEST_BIN =	test_sock_lib

$(TEST_BIN):	$(TEST_BIN).c $(SOCK_LIB_BIN) $(LOG_LIB_DIR)/log_lib.a
		$(CC) $(CFLAGS) $(TEST_BIN).c $(SOCK_LIB_BIN) $(LOG_LIB_DIR)/log_lib.a -o $(TEST_BIN)

test:		$(TEST_BIN)
		./$(TEST_BIN)

clean:		clean_test

clean_test:
		rm -f $(TEST_BIN) test_*.log

# Chris's original sock_lib test programs from NewIO (interactive, run by hand)
EXAMPLES =	examples/tcp_server examples/tcp_client examples/udp_server \
		examples/udp_client examples/udp_client2

examples:	$(EXAMPLES)

examples/%:	examples/%.c $(SOCK_LIB_BIN) $(LOG_LIB_DIR)/log_lib.a
		$(CC) $(CFLAGS) $< $(SOCK_LIB_BIN) $(LOG_LIB_DIR)/log_lib.a -o $@

clean_examples:
		rm -f $(EXAMPLES)

clean:		clean_examples

.PHONY:		all default new clean clean_test clean_examples test examples indent
