SHELL=/bin/bash
CC=g++
C_FLAGS=-c -g -O0 -Wall

SRC=log.cpp port.cpp menu.cpp
OBJ=$(SRC:.cpp=.o)

INCLUDE_PATH=../include

LIBS=`pkg-config gtkmm-3.0 --cflags --libs`

all: $(OBJ)

%.o: %.cpp
	$(CC) $(C_FLAGS) $(LIBS) -I$(INCLUDE_PATH) $< -o ../obj/$@;
