SHELL=/bin/bash
CC=g++

MAIN=main.cpp
OBJ=obj/*.o

C_FLAGS=-g -O0 -Wall
INCLUDE_PATH=include

OUTPUT=XCubeSat_Controler

LOG=$(OUTPUT:=.txt)

all:
	cd src; make
	$(CC) $(C_FLAGS) -I$(INCLUDE_PATH) $(OBJ) $(MAIN) -o $(OUTPUT);
	touch $(LOG)

clean:
	rm $(OUTPUT) $(LOG) $(OBJ)
