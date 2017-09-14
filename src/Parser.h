#ifndef PARSER_H
#define PARSER_H

// The Parser takes a standard vector of Tokens and transforms it into a binary
// tree reprentation of our list program. This allows for pretty printing and,
// later on, execution of our programs.

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "Token.h"
#include "LexicalAnalyzer.h"

#define IS_ATOM true
#define IS_NOT_ATOM false

typedef struct ExpressionTreeNode {
    Token atom;
    ExpressionTreeNode *leftChild;
    ExpressionTreeNode *rightChild;
} ExpressionTreeNode;

class Parser {
private:
    bool isList;
    LexicalAnalyzer scanner;
    bool isAtom(Token t);
    void printExpression_test1();
    void printExpression_test2();
    void printExpression_test3();
    void printExpression_test4();
    void printExpression_test5();

public:
    Parser(LexicalAnalyzer &l) : scanner(l) {}

    void start();
    void runTests();
    void printTokens();
    void parseExpression(ExpressionTreeNode *root);
    std::string printExpression(ExpressionTreeNode *root, bool isList);
};

#endif
