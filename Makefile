
ifeq ($(OS),Windows_NT)
	CC = mingw32-gcc
	BIN = "bin\"
	DIRCHK = if exist bin rd /s /q bin
	CPGLADE = copy main_win.glade "bin\"
	RM = rmdir /S /Q bin
	SQLITE = sqlite3.c
	DEBUG = bin\Debug
	RELEASE = bin\Release
else
	CC = gcc
	BIN = -vp bin/
	RM = rm -rf bin
	SQLITE = -l sqlite3 -ldl -lpthread
	DEBUG = bin/Debug
	RELEASE = bin/Release
endif

SHELL = bash

GTK := $(shell pkg-config --cflags --libs gtk+-3.0)

CFLAGS := -g -Wall -static-libgcc -lm $(GTK) $(SQLITE)

TARGET = deans2

all:
	$(DIRCHK)
	mkdir $(BIN)Debug
	mkdir $(BIN)Release
	$(CPGLADE) $(DEBUG)
	$(CPGLADE) $(RELEASE)
	$(CC) main.c $(CFLAGS) -o bin/Release/$(TARGET)
	$(CC) main.c $(CFLAGS) -o bin/Debug/$(TARGET)

Debug:
	$(DIRCHK)
	mkdir $(BIN)Debug
	$(CPGLADE) $(DEBUG)
	$(CC) main.c $(CFLAGS) -o bin/Debug/$(TARGET)

Release:
	$(DIRCHK)
	mkdir $(BIN)Release
	$(CPGLADE) $(RELEASE)
	$(CC) main.c $(CFLAGS) -o bin/Release/$(TARGET)

clean:
	$(RM)
