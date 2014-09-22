CC = gcc
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
	mkdir -vp bin/Debug
	$(CC) main.c $(CFLAGS) -o bin/Release/$(TARGET)

clean:
	rm Debug/$(TARGET)
	rm Release/$(TARGET)
