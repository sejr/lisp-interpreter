#ifndef PARSER_H
#define PARSER_H

// The Parser takes a standard vector of Tokens and transforms it into a binary
// tree reprentation of our list program. This allows for pretty printing and,
// later on, execution of our programs.

#include <vector>
#include <iostream>
#include "Token.h"
#include "LexicalAnalyzer.h"

struct ExpressionTreeNode {
    Token atom;
    ExpressionTreeNode *leftChild;
    ExpressionTreeNode *rightChild;
};

class Parser {
private:
    LexicalAnalyzer scanner;

public:
    Parser(LexicalAnalyzer &l) : scanner(l) {}

    void start();
    void printTokens();
    void parseExpression(ExpressionTreeNode *root);
    void printExpression(ExpressionTreeNode *root);
};

#endif
