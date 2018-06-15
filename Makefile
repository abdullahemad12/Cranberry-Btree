#
# @author: Abdullah Emad
# Makefile for The binary tree test
#

test: 
	clang -ggdb -std=c11 -O0 -Wall -Werror test.c -lcunit -o test

.PHONY: test 

clean: 
	rm -f test bptree