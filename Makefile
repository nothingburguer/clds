CC=gcc
CFLAGS=-O2 -Wall
LUA_FLAGS=$(shell pkg-config --cflags --libs lua)

PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin

CLI = clds
SERVER = clds-server

all: $(CLI) $(SERVER)

$(CLI):
	$(CC) clds.c -o $(CLI)

$(SERVER):
	$(CC) engine/server.c -o $(SERVER) $(LUA_FLAGS)

install: $(CLI) $(SERVER)
	install -Dm755 $(CLI) $(BINDIR)/$(CLI)
	install -Dm755 $(SERVER) $(BINDIR)/$(SERVER)

uninstall:
	rm -f $(BINDIR)/$(CLI)
	rm -f $(BINDIR)/$(SERVER)

clean:
	rm -f $(CLI) $(SERVER)

