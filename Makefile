SHELL=/bin/bash
CC=g++

MAIN=main.cpp
OBJ=src/*.o

C_FLAGS=-g -O0 -Wall -std=c++11
INCLUDE_PATH=include
SQLITE3_INCLUDE=/usr/devel/include

LIBS=`pkg-config gtkmm-3.0 --cflags --libs`
SQLITE3_LIBS=-lsqlitewrapped -lsqlite3 -L/usr/devel/lib 

OUTPUT=XCubeSat_Controler

all:
	cd src; make
	$(CC) $(OBJ) $(MAIN) -o $(OUTPUT) $(C_FLAGS) -I$(SQLITE3_INCLUDE) -I$(INCLUDE_PATH) $(SQLITE3_LIBS) $(LIBS);

clean:
	rm $(OUTPUT) $(LOG) $(OBJ)
