
TARGETS =	$(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5)

#==============================================================================

FILENAME1 =     tcp_server
TARGET1 =       $(FILENAME1)

HEADERS1 =      \
                $(NULL)

CCFILES1 =      \
                $(FILENAME1).c \
                $(NULL)

OBJS1 =         ${CCFILES1:.c=.o}

LIBS1 =         \
                $(SOCK_LIB_BIN) \
                $(NIO_LOG_BIN) \
                $(NULL)

#==============================================================================

FILENAME2 =     tcp_client
TARGET2 =       $(FILENAME2)

HEADERS2 =      \
                $(NULL)

CCFILES2 =      \
                $(FILENAME2).c \
                $(NULL)

OBJS2 =         ${CCFILES2:.c=.o}

LIBS2 =         \
                $(SOCK_LIB_BIN) \
                $(NIO_LOG_BIN) \
                $(NULL)

#==============================================================================

FILENAME3 =     udp_server
TARGET3 =       $(FILENAME3)

HEADERS3 =      \
                $(NULL)

CCFILES3 =      \
                $(FILENAME3).c \
                $(NULL)

OBJS3 =         ${CCFILES3:.c=.o}

LIBS3 =         \
                $(SOCK_LIB_BIN) \
                $(NIO_LOG_BIN) \
                $(NULL)

#==============================================================================

FILENAME4 =     udp_client
TARGET4 =       $(FILENAME4)

HEADERS4 =      \
                $(NULL)

CCFILES4 =      \
                $(FILENAME4).c \
                $(NULL)

OBJS4 =         ${CCFILES4:.c=.o}

LIBS4 =         \
                $(SOCK_LIB_BIN) \
                $(NIO_LOG_BIN) \
                $(NULL)

#==============================================================================

FILENAME5 =     udp_client2
TARGET5 =       $(FILENAME5)

HEADERS5 =      \
                $(NULL)

CCFILES5 =      \
                $(FILENAME5).c \
                $(NULL)

OBJS5 =         ${CCFILES5:.c=.o}

LIBS5 =         \
                $(SOCK_LIB_BIN) \
                $(NIO_LOG_BIN) \
                $(NULL)

#==============================================================================

SOCK_LIB =      sock_lib
SOCK_LIB_PATH = ../../$(SOCK_LIB)
SOCK_LIB_BIN =  $(SOCK_LIB_PATH)/$(SOCK_LIB).a
SOCK_LIB_OBJ =  $(SOCK_LIB_PATH)/$(SOCK_LIB).o
#==============================================================================

STR_LIB =      str_lib
STR_LIB_PATH = ../../$(STR_LIB)
STR_LIB_BIN =  $(STR_LIB_PATH)/$(STR_LIB).a
STR_LIB_OBJ =  $(STR_LIB_PATH)/$(STR_LIB).o

#==============================================================================

SSH2_LIB =      ssh2_lib
SSH2_LIB_PATH = ../../$(SSH2_LIB)
SSH2_LIB_BIN =  $(SSH2_LIB_PATH)/$(SSH2_LIB).a
SSH2_LIB_OBJ =  $(SSH2_LIB_PATH)/$(SSH2_LIB).o

#==============================================================================

NIO_LOG_LIB =	nio_log
NIO_LOG_PATH = ../../$(NIO_LOG_LIB)
NIO_LOG_BIN =  $(NIO_LOG_PATH)/$(NIO_LOG_LIB).a
NIO_LOG_OBJ =  $(NIO_LOG_PATH)/$(NIO_LOG_LIB).o
#==============================================================================

MSG_LIB =	msg_lib
MSG_LIB_PATH = ..
MSG_LIB_BIN =  $(MSG_LIB_PATH)/$(MSG_LIB).a
MSG_LIB_OBJ =  $(MSG_LIB_PATH)/$(MSG_LIB).o

#==============================================================================

CC =		gcc

COPTS =		-O3 -g -Wall

INCLUDES = 	\
		-I.. \
		-I../.. \
		-I../../nio_log \
		-I../../sock_lib \
		-I../../str_lib \
		-I../../ssh2_lib \
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

$(TARGET1):	$(OBJS1) $(HEADERS1) $(LIBS1)
		$(CC) $(LFLAGS) $(OBJS1) $(LIBS1) -o $@

$(TARGET2):	$(OBJS2) $(HEADERS2) $(LIBS2)
		$(CC) $(LFLAGS) $(OBJS2) $(LIBS2) -o $@

$(TARGET3):	$(OBJS3) $(HEADERS3) $(LIBS3)
		$(CC) $(LFLAGS) $(OBJS3) $(LIBS3) -o $@

$(TARGET4):	$(OBJS4) $(HEADERS4) $(LIBS4)
		$(CC) $(LFLAGS) $(OBJS4) $(LIBS4) -o $@

$(TARGET5):	$(OBJS5) $(HEADERS5) $(LIBS5)
		$(CC) $(LFLAGS) $(OBJS5) $(LIBS5) -o $@

$(NIO_LOG_BIN):
		cd $(NIO_LOG_PATH); make

$(SOCK_LIB_BIN):
		cd $(SOCK_LIB_PATH); make

$(STR_LIB_BIN):
		cd $(STR_LIB_PATH); make

$(SSH2_LIB_BIN):
		cd $(SSH2_LIB_PATH); make

$(MSG_LIB_BIN):
		cd $(MSG_LIB_PATH); make

indent:
		dos2unix *.h *.c
		indent -kr -i8 *.h *.c 

clean:
		rm -f $(TARGETS) *.o *.a core a.out *~ .*.swp
 
