#
# @author: Abdullah Emad
# Makefile for The binary tree test
#


# compiler to use
CC = clang

# flags to pass compiler
CFLAGS = -ggdb3 -O0 -Qunused-arguments -std=c11 -Wall -Werror

# name for executable
EXE = ../bin/test

# space-separated list of header files
HDRS = 

# space-separated list of libraries, if any,
# each of which should be prefixed with -l
LIBS = -lcunit

# space-separated list of source files
SRCS = test.c 

# Included folders
INCLUDES = ../include

# automatically generated list of object files
OBJS = $(SRCS:.c=.o)

# default target
test: mkbin Makefile
	$(CC) $(CFLAGS) -o $(EXE) $(SRCS) $(LIBS) -I $(INCLUDES)

#creates the bin directory if it does not exist
mkbin:
	mkdir -p ../bin

# dependencies 
$(OBJS): $(HDRS) Makefile

# housekeeping
clean:
	rm -f core $(EXE) 


.PHONY: test
