# C++ Lisp Interpreter

This is a rudimentary lisp interpreter written in C++ 11. It was built for the
Foundations of Programming Languages course at The Ohio State University, during
the Autumn 2017 term. 

## Project Structure

```
lisp-interpreter/
├── examples
│   ├── ex1.lisp
│   ├── ex1.lisp.result
│   ├── ex2.lisp
│   ├── ex2.lisp.result
│   ├── ex3.lisp
│   ├── ex3.lisp.result
│   ├── ex4.lisp
│   ├── ex4.lisp.result
│   ├── ex5.lisp
│   ├── ex5.lisp.result
│   ├── ex6.lisp
│   └── ex6.lisp.result
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
    ├── TestParser.h
    └── Tookit.h
```

## Modules

### Lexical Analyzer

Also known as the scanner, this reads in input from a file one character at a
time and generates valid Tokens.

### Parser

This takes a vector of Tokens and generates a binary parse tree, representing
our lisp program in a way that is usable and understandable. We are also able
to print out formatted code for the user's benefit.

## Evaluator

This takes a binary expression tree and recursively evaluates the built-in lisp
functions. This also provides a list printer, which is slightly different from
the printer in the Parser module.

### Toolkit

This is a helper class that provides helpful methods for writing tests.