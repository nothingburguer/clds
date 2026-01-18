CC=gcc
CFLAGS=-O2 -Wall
LUA_FLAGS=$(shell pkg-config --cflags --libs lua)

all: clds server

clds:
	$(CC) clds.c -o clds

server:
	$(CC) engine/server.c -o server $(LUA_FLAGS)

clean:
	rm -f clds server

