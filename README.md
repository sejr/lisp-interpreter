# C++ Lisp Interpreter

This is a rudimentary lisp interpreter written in C++ 11. It was built for the
Foundations of Programming Languages course at The Ohio State University, during
the Autumn 2017 term. Version 4 implements user-defined functions, removes some
of the old, arbitrary test cases, and is a rewrite of many different portions of
the parser and evaluator. I've tried to make the code easier to digest.

## Project Structure

```
lisp-interpreter/
├── LICENSE
├── Makefile
├── README.md
├── Runfile
├── src
│   ├── LexicalAnalyzer.cc
│   ├── LexicalAnalyzer.h
│   ├── main.cc
│   ├── Parser.cc
│   ├── Parser.h
│   ├── Token.cc
│   ├── Token.h
│   ├── TokenProfile.cc
│   └── TokenProfile.h
└── test
    ├── compare.test.lisp
    ├── errors.test.lisp
    ├── mem.test.lisp
    ├── name.test.lisp
    └── pow.test.lisp
```

## Modules

### Parser

The parser reads an input file and generates a tree of tokens, representing
our lisp program in a way that is usable and understandable. We are also able
to print out formatted code for the user's benefit.

### Evaluator

This takes a binary expression tree and recursively evaluates the built-in lisp
functions. This also provides a list printer, which is slightly different from
the printer in the Parser module.

### Printers

Extra functions that print the Token binary tree in different ways, with
varying degrees of usability.