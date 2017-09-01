# This is a very simple makefile for building the Lisp interpreter
# project when using C++ on stdlinux. Feel free to add any improvements:
# e.g. pattern rules, automatic tracking of dependencies, etc. There
# is a lot of info about "make" on the web.

CXX = g++
CXXFLAGS = -Wall -std=c++0x
COMPILE = $(CXX) $(CXXFLAGS) 
SRC = src/
BIN = bin/
BUILD = build/
EXE = interpreter

TESTINPUT = examples/ex1.lisp
TESTOUTPUT = examples/ex1.result.lisp

all: interpreter

interpreter:
	mkdir $(BUILD)
	$(COMPILE) $(SRC)main.cc -o $(BUILD)$(EXE)

test: interpreter
	./$(BUILD)$(EXE) < $(TESTINPUT) > $(TESTOUTPUT)

bat: clean test
	cat $(TESTOUTPUT)

clean:
	rm -rf build