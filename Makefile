CC = gcc
SHELL = bash

GTK := $(shell pkg-config --cflags --libs gtk+-3.0)
CFLAGS := -g -Wall $(GTK)

TARGET = deans2

all: $(TARGET)
	$(CC) main.c $(CFLAGS) -o bin/Debug/$(TARGET)

Debug: $(TARGET)
	$(CC) main.c $(CFLAGS) -o bin/Debug/$(TARGET)

deans2: $(TARGET)
	$(CC) main.c $(CFLAGS) -o bin/Debug/$(TARGET)

clean:
	rm Debug/$(TARGET)
