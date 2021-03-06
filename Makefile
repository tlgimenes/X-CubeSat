SHELL=/bin/bash
CC=g++

MAIN=main.cpp
OBJ=src/*.o

C_FLAGS_DEBUG=-g -O0 -Wall -std=c++11
C_FLAGS_RELEASE= -O2 -std=c++11

INCLUDE_PATH=include
SQLITE3_INCLUDE=/usr/devel/include

LIBS=`pkg-config gtkmm-3.0 vte-2.90 sqlite3 --cflags --libs`
SQLITE3_LIBS=-lsqlitewrapped -lsqlite3 -L/usr/devel/lib 
BOOST_LIBS=-lboost_system -lboost_thread -pthread

OUTPUT=XCubeSat_Controller

all:
	cd src; make
	$(CC) $(OBJ) $(MAIN) -o $(OUTPUT) $(C_FLAGS_DEBUG) -I$(SQLITE3_INCLUDE) -I$(INCLUDE_PATH) $(BOOST_LIBS) $(SQLITE3_LIBS) $(LIBS);

clean:
	rm $(OUTPUT) $(LOG) $(OBJ)

debug:
	cd src; make debug
	$(CC) $(OBJ) $(MAIN) -o $(OUTPUT) $(C_FLAGS_DEBUG) -I$(SQLITE3_INCLUDE) -I$(INCLUDE_PATH) $(SQLITE3_LIBS) $(LIBS);
