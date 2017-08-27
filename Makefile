# This is a very simple makefile for building the Lisp interpreter
# project when using C++ on stdlinux. Feel free to add any improvements:
# e.g. pattern rules, automatic tracking of dependencies, etc. There
# is a lot of info about "make" on the web.

CXX = g++
CXXFLAGS = -Wall
COMPILE = $(CXX) $(CXXFLAGS) 
SRC = src/
BIN = bin/
BUILD = build/
EXE = interpreter

all: interpreter

interpreter:
	mkdir $(BUILD)
	$(COMPILE) $(SRC)main.cc -o $(BUILD)$(EXE)

clean:
	rm -rf build
