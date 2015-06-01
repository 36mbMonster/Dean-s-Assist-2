ifeq ($(OS),Windows_NT)
	CC = mingw32-gcc
	BIN = "bin\"
	DIRCHK = if exist bin rd /s /q bin
	CPGLADE = copy main_win.glade bin\Release
	RM = rmdir /S /Q bin
	SQLITE = -dll sqlite3.dll
	DLL = copy /Y sqlite3.dll bin\Debug && copy /Y sqlite3.dll bin\Release
else
	CC = gcc
	BIN = -vp bin/
	RM = rm -rf bin
	SQLITE = -l sqlite3 -ldl -lpthread
	DLL = rm -f sqlite3.dll
endif

SHELL = bash

GTK := $(shell pkg-config --cflags --libs gtk+-3.0)

CFLAGS := -g -Wall -static-libgcc -lm $(GTK) $(SQLITE)

TARGET = deans2

all:
	$(DIRCHK)
	mkdir $(BIN)Debug
	mkdir $(BIN)Release
	$(CPGLADE)
	$(CC) main.c $(CFLAGS) -o bin/Release/$(TARGET)
	$(DLL)

Debug:
	$(DIRCHK)
	mkdir $(BIN)Debug
	$(CPGLADE)
	$(CC) main.c $(CFLAGS) -o bin/Debug/$(TARGET)
	$(DLL)

Release:
	$(DIRCHK)
	mkdir $(BIN)Release
	$(CPGLADE)
	$(CC) main.c $(CFLAGS) -o bin/Release/$(TARGET)
	$(DLL)

clean:
	$(RM)
