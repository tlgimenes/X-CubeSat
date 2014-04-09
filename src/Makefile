SHELL=/bin/bash
CC=g++
C_FLAGS=-c -g -O0 -Wall -std=c++11

SQLITE3_INCLUDE=/usr/devel/include
SQLITE3_LIBS=-lsqlitewrapped -lsqlite3 -L/usr/devel/lib 

SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)

INCLUDE_PATH=../include

LIBS=`pkg-config gtkmm-3.0 sqlite3 --cflags --libs`

all: $(OBJ)

%.o: %.cpp
	$(CC) $(C_FLAGS) -I$(SQLITE3_INCLUDE) -I$(INCLUDE_PATH) $(SQLITE3_LIBS) $(LIBS) $< -o $@;
