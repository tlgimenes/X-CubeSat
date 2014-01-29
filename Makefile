SHELL=/bin/bash
CC=g++

MAIN=main.cpp
OBJ=src/*.o

C_FLAGS=-g -O0 -Wall -std=c++11
INCLUDE_PATH=include
LIBS=`pkg-config gtkmm-3.0 --cflags --libs`

OUTPUT=XCubeSat_Controler

LOG=$(OUTPUT:=.txt)

all:
	cd src; make
	$(CC) $(C_FLAGS) -I$(INCLUDE_PATH) $(LIBS) $(OBJ) $(MAIN) -o $(OUTPUT);
	touch $(LOG)

clean:
	rm $(OUTPUT) $(LOG) $(OBJ)
