# This is a very simple makefile for building the Lisp interpreter
# project when using C++ on stdlinux. Feel free to add any improvements:
# e.g. pattern rules, automatic tracking of dependencies, etc. There
# is a lot of info about "make" on the web.

CXX = g++
CXXFLAGS = -Wall -std=c++0x
COMPILE = $(CXX) $(CXXFLAGS)

SRC = src
BIN = bin
BUILD = build
EXE = interpreter
OBJS = main.o parser.o

all: $(OBJS)
	mkdir bin
	$(CXX) $(SRC)/main.cpp $(BUILD)/parser.o -o $(BIN)/$(EXE)

main.o: $(SRC)/main.cpp
	mkdir $(BUILD)
	$(COMPILE) -c -o $(BUILD)/$@ $<

parser.o: $(SRC)/Parser.cpp
	$(COMPILE) -c -o $(BUILD)/$@ $<

clean:
	rm -rf build
	rm -rf bin
