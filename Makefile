
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
		-I.. \
		-I../nio_log \
		-I../win32_lib \
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
 
