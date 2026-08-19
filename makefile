CC = gcc
CCOPTS = -Wall -Wpedantic

all: bin/main

bin/main: src/main.c
	${CC} ${CCOPTS} ./src/main.c -o ./bin/main

clean:
	rm bin/*
