CC = gcc
SHELL = bash

GTK := $(shell pkg-config --cflags --libs gtk+-3.0)
CFLAGS := -g -Wall $(GTK)

TARGET = deans2

all:
	$(CC) main.c $(CFLAGS) -o bin/Debug/$(TARGET)

Debug:
	$(CC) main.c $(CFLAGS) -o bin/Debug/$(TARGET)

deans2:
	mkdir -vp bin/Debug
	$(CC) main.c $(CFLAGS) -o bin/Debug/$(TARGET)

clean:
	rm Debug/$(TARGET)
