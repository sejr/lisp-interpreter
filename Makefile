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
OBJS = Token.o TokenProfile.o LexicalAnalyzer.o Parser.o

TESTINPUT = examples/ex1.lisp
TESTOUTPUT = examples/ex1.result.lisp

all: $(OBJS)
	mkdir bin
	$(CXX) $(SRC)/main.cc $(BUILD)/Token.o $(BUILD)/TokenProfile.o $(BUILD)/LexicalAnalyzer.o $(BUILD)/Parser.o -o $(BIN)/$(EXE)

Token.o: $(SRC)/Token.cc
	mkdir $(BUILD)
	$(COMPILE) -c -o $(BUILD)/$@ $<

TokenProfile.o: $(SRC)/TokenProfile.cc
	$(COMPILE) -c -o $(BUILD)/$@ $<

LexicalAnalyzer.o: $(SRC)/LexicalAnalyzer.cc
	$(COMPILE) -c -o $(BUILD)/$@ $<

Parser.o: $(SRC)/Parser.cc
	$(COMPILE) -c -o $(BUILD)/$@ $<

clean:
	rm -rf build
	rm -rf bin
