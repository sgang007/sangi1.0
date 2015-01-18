TARGET=controller
SOURCE=./src
INCLUDE =./include
BINARIES=./bin
LIBS=./lib
CC=g++
CFLAGS=-std=c++11

all: libraries
	mkdir -p $(BINARIES)
	g++ $(SOURCE)/* $(CFLAGS) -I $(INCLUDE) -o $(BINARIES)/$(TARGET)

libraries:
	mkdir -p $(LIBS)
	g++ $(SOURCE)/* $(CFLAGS) -I $(INCLUDE) -c -o $(LIBS)/lib.o