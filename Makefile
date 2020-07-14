CSCI_DIR=$(HOME)/proj/fg_proxy

SRC_DIR=$(CSCI_DIR)/src
INC_DIR=$(CSCI_DIR)/inc
OBJ_DIR=$(CSCI_DIR)/obj
LIB_DIR=$(CSCI_DIR)/lib
BIN_DIR=$(CSCI_DIR)/bin

INCLUDE = -I$(HOME)/FlightGear/include -I$(HOME)/FlightGear/src/flightgear-flightgear/src/Network/ -I$(INC_DIR)

SRC_NAME= \
main.cpp

OBJ= $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC_NAME))
SRC= $(patsubst %.cpp, $(SRC_DIR)/%.cpp, $(SRC_NAME))

CC=g++
CFLAGS=-g -Wall -std=gnu++11  $(INCLUDE)
LFLAGS=

PRG=fg_proxy
LIB=lib$(PRG).a

all: create_dir $(PRG) $(LIB)

$(LIB):
	@# quickfix: lib should not include main symbol, rename main.o temporary...
	@mv -f $(OBJ_DIR)/main.o $(OBJ_DIR)/main.x
	ar ruv $(LIB_DIR)/$(LIB) $(OBJ_DIR)/*.o 
	ranlib $(LIB_DIR)/$(LIB) 
	@mv -f $(OBJ_DIR)/main.x $(OBJ_DIR)/main.o
	chmod 755 $(LIB_DIR)/$(LIB)

$(PRG): $(OBJ)
	$(CC) -o $(BIN_DIR)/$(PRG) $(OBJ) $(CFLAGS) $(LFLAGS)

clean:
	rm -f $(OBJ_DIR)/* $(OBJ) $(BIN_DIR)/$(PRG) $(LIB_DIR)/$(LIB) $(SRC_DIR)/*~ $(ALI)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY : create_dir
create_dir :
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(BIN_DIR)
