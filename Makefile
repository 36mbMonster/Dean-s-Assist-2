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
	mkdir -vp bin/Debug
	cp main_win.glade bin/Debug/main_win.glade
	$(CC) main.c $(CFLAGS) -o bin/Debug/$(TARGET)

deans2:
	mkdir -vp bin/Release
	cp main_win.glade bin/Release/main_win.glade
	$(CC) main.c $(CFLAGS) -o bin/Release/$(TARGET)

clean:
	rm bin/Debug/$(TARGET)
	rm bin/Release/$(TARGET)
