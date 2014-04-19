SHELL=/bin/bash
CC=g++

C_FLAGS_DEBUG=-g3 -c -O0 -Wall -std=c++11
C_FLAGS_RELEASE=-c -O2 -std=c++11

SQLITE3_INCLUDE=/usr/devel/include
SQLITE3_LIBS=-lsqlitewrapped -lsqlite3 -L/usr/devel/lib 

BOOST_LIBS=-lboost_system -lboost_thread -pthread

SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)

INCLUDE_PATH=../include

LIBS=`pkg-config gtkmm-3.0 sqlite3 vte-2.90 --cflags --libs`

all: $(OBJ)

%.o: %.cpp
	$(CC) $(C_FLAGS_DEBUG) -I$(SQLITE3_INCLUDE) -I$(INCLUDE_PATH) $(BOOST_LIBS) $(SQLITE3_LIBS) $(LIBS) $< -o $@;

