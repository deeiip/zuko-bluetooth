SHELL = /bin/sh
CURRENT_DIR = $(shell pwd)
CC    = gcc
FLAGS        = -std=gnu99 -Iinclude
CFLAGS       = -I $(CURRENT_DIR)/include -fPIC -g #-pedantic -Wall -Wextra -ggdb3
LDFLAGS      = -shared -lpthread -lbluetooth
DEBUGFLAGS   = -O0 -D _DEBUG
RELEASEFLAGS = -O2 -D NDEBUG -combine -fwhole-program

TARGET  = libzuko.so
SOURCES = $(shell echo src/*.c)
HEADERS = $(shell echo include/*.h)
OBJECTS = $(SOURCES:.c=.o)

PREFIX = $(DESTDIR)/usr/local
BINDIR = $(PREFIX)/bin
BINARIES = libzuko.so



all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -I"include" $(LDFLAGS) $(FLAGS) $(CFLAGS) $(DEBUGFLAGS) -o $(TARGET) $(OBJECTS)
clean:
	$(RM) *.o *.so*
install:
	mkdir -p $(CURRENT_DIR)/install
	cp $(TARGET) $(CURRENT_DIR)/install
