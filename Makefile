ifeq ($(OS),Windows_NT)
	CC = mingw32-gcc
else
	CC = gcc
endif

SHELL = bash

GTK := $(shell pkg-config --cflags --libs gtk+-3.0)
SQLITE := $(-l sqlite3)
CFLAGS := -g -Wall $(GTK) $(SQLITE)

TARGET = deans2

all:
	$(CC) main.c $(CFLAGS) -o bin/Release/$(TARGET)

Debug:
	$(CC) main.c $(CFLAGS) -o bin/Debug/$(TARGET)

deans2:
	mkdir -vp bin/Release
	$(CC) main.c $(CFLAGS) -o bin/Release/$(TARGET)

clean:
	rm bin/Debug/$(TARGET)
	rm bin/Release/$(TARGET)
