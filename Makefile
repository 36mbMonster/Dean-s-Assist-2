ifeq ($(OS),Windows_NT)
	CC = mingw32-gcc
	BIN = "bin\"
	DIRCHK = if exist bin rd /s /q bin
	CPGLADE = copy main_win.glade bin\Release
	RM = rmdir bin
	SQLITE = -dll sqlite3.dll
else
	CC = gcc
	BIN = -vp bin/
	RM = rm -rf bin
	SQLITE = -l sqlite3 -ldl -lpthread
endif

SHELL = bash

GTK := $(shell pkg-config --cflags --libs gtk+-3.0)

CFLAGS := -g -Wall $(GTK) $(SQLITE)

TARGET = deans2

all:
	$(DIRCHK)
	mkdir $(BIN)Debug
	mkdir $(BIN)Release
	$(CPGLADE)
	$(CC) main.c $(CFLAGS) -o bin/Release/$(TARGET)

Debug:
	mkdir $(BIN)Debug
	$(CPGLADE)
	$(CC) main.c $(CFLAGS) -o bin/Debug/$(TARGET)

deans2:
	mkdir $(BIN)Release
	$(CPGLADE)
	$(CC) main.c $(CFLAGS) -o bin/Release/$(TARGET)

clean:
	$(RM)
