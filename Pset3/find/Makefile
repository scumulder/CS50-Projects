#
# Makefile: this file tells your what commands are execute when each of the following are "made"
# i.e., make find, make all, make generate, make clean.
# if you type 'make all' I think it'll be the same as saying 'make find' and 'make generate' except in a single command
# CS50
#

all: find generate

find: find.c helpers.c helpers.h
	clang -ggdb3 -O0 -std=c11 -Wall -Werror -o find find.c helpers.c -lcs50 -lm

generate: generate.c
	clang -ggdb3 -O0 -std=c11 -Wall -Werror -o generate generate.c

clean:
	rm -f *.o a.out core find generate
