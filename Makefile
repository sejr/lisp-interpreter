# This is a very simple makefile for building the Lisp interpreter
# project when using C++ on stdlinux. Feel free to add any improvements:
# e.g. pattern rules, automatic tracking of dependencies, etc. There
# is a lot of info about "make" on the web.

CXX = g++
CXXFLAGS = -Wall
COMPILE = $(CXX) $(CXXFLAGS) 
SOURCEDIR = src/
BUILDDIR = build/

# Name of the executable; should match the contents of Runfile
EXE = interpreter

all:
	mkdir $(BUILDDIR)
	$(COMPILE) $(SOURCEDIR)main.cc -o $(BUILDDIR)$(EXE)

clean:
	rm -rf build
