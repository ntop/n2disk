#
# Makefile for the Remote ntop Wireshark Extcap Module
#

#
# Search directories
#
INCLUDE = 

#
# C compiler and flags
#
CC = gcc
CFLAGS = -Wall ${INCLUDE} -O2 # -g

#
# User and System libraries
#
LIBS = -lssh 

#
# How to make an object file
#
%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

#
# Main targets
#
TARGETS = remotentopdump

OBJS = remotentopdump.o

all: ${TARGETS}

remotentopdump: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} ${LIBS} -o $@

clean:
	@rm -f ${TARGETS} *.o *~
