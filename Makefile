#
# @author: Abdullah Emad
# Makefile for The binary tree test
#

test: 
	clang -ggdb -std=c11 -O0 -Wall -Werror test.c -lcunit -o test
cli: 
	clang -ggdb -std=c11 -O0 -Wall -Werror btree.c cli.c  -o cli

.PHONY: test cli

clean: 
	rm -f test bptree