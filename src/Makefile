SHELL=/bin/bash
CC=g++
C_FLAGS=-c -g -O0 -Wall

SRC=log.cpp port.cpp
OBJ=$(SRC:.cpp=.o)

INCLUDE_PATH=../include

all: $(OBJ)

%.o: %.cpp
	$(CC) $(C_FLAGS) -I$(INCLUDE_PATH) $< -o ../obj/$@;
