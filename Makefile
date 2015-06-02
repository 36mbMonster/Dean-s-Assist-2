
ifeq ($(OS),Windows_NT)
	CC = mingw32-gcc
	BIN = "bin\"
	DIRCHK = if exist bin rd /s /q bin
	CPGLADE = copy main_win.glade bin\
	CPSQL = copy sqlite3.c bin\
	RM = rmdir /S /Q bin
	SQLITE = sqlite3.c
	# Workaround vars for preventing issues in Linux
	DEBUG = Debug
	RELEASE = Release
else
	CC = gcc
	BIN = -vp bin/
	RM = rm -rf bin
	SQLITE = -l sqlite3 -ldl -lpthread
endif

SHELL = bash

GTK := $(shell pkg-config --cflags --libs gtk+-3.0)

CFLAGS := -g -Wall -static-libgcc -lm $(GTK) $(SQLITE)

TARGET = deans2

all:
	$(DIRCHK)
	mkdir $(BIN)Debug
	mkdir $(BIN)Release
	$(CPGLADE)$(Debug)
	$(CPGLADE)$(Release)
	$(CPSQL)$(Debug)
	$(CPSQL)$(Release)
	$(CC) main.c $(CFLAGS) -o bin/Release/$(TARGET)
	$(CC) main.c $(CFLAGS) -o bin/Debug/$(TARGET)

Debug:
	$(DIRCHK)
	mkdir $(BIN)Debug
	$(CPGLADE)$(Debug)
	$(CPSQL)$(Debug)
	$(CC) main.c $(CFLAGS) -o bin/Debug/$(TARGET)

Release:
	$(DIRCHK)
	mkdir $(BIN)Release
	$(CPGLADE)$(Release)
	$(CPSQL)$(Release)
	$(CC) main.c $(CFLAGS) -o bin/Release/$(TARGET)

clean:
	$(RM)
