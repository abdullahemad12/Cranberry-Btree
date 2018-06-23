#####################################
# @author: Abdullah Emad 			#
# Makefile for The binary tree test #
#####################################

# compiler to use
CC = clang

# flags to pass compiler
CFLAGS = -ggdb3 -O0 -Qunused-arguments -std=c11 -Wall -Werror

# name for executable
EXE = bin/btree

# space-separated list of header files
HDRS = 

# space-separated list of libraries, if any,
# each of which should be prefixed with -l
LIBS = -lcunit

# space-separated list of source files
SRCS = src/btree.c


# Included folders
INCLUDES = include


# automatically generated list of object files
OBJS = $(SRCS:.c=.o)


# default target
default:
	
# creates the object file
compile:
	$(CC) $(CFLAGS) -c $(SRCS) -I $(INCLUDES)

#creates the library
createlib: 
	ar rc cranbtree.a objfile1.o objfile2.o objfile3.o


# dependencies 
$(OBJS): $(HDRS) Makefile




#Creates binary for the test in /bin
test: mkbin
	$(CC) $(CFLAGS) test/test.c $(LIBS) -o bin/test -I $(INCLUDES)

#Creates binary for the cli in /bin
cli: mkbin
	$(CC) $(CFLAGS) $(SRCS) cli/cli.c  -o bin/cli -I $(INCLUDES)

#creates the bin directory if it does not exist
mkbin:
	mkdir -p ../bin


# housekeeping
clean:
	rm -f core *.o *.a bin/cli bin/test

.PHONY: test cli

