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

struct ExpressionTreeNode {
    Token atom;
    ExpressionTreeNode *leftChild;
    ExpressionTreeNode *rightChild;

    operator bool() const {
        return (bool) atom;
    }
};

class Parser {
private:
    LexicalAnalyzer scanner;
    bool isAtom(Token t);

    // Parsing helper function
    void _parseExpression(ExpressionTreeNode *root);

    // List length utility
    int len(ExpressionTreeNode *root);

    // List operators
    ExpressionTreeNode* car(ExpressionTreeNode *root);
    ExpressionTreeNode* cdr(ExpressionTreeNode *root);
    ExpressionTreeNode* atom(ExpressionTreeNode *root);
    ExpressionTreeNode* num(ExpressionTreeNode *root);
    ExpressionTreeNode* lit(ExpressionTreeNode *root);
    ExpressionTreeNode* null(ExpressionTreeNode *root);
    ExpressionTreeNode* cons(ExpressionTreeNode *left, 
        ExpressionTreeNode *right);

    // Numeric operators
    ExpressionTreeNode* plus(ExpressionTreeNode *a, ExpressionTreeNode *b);
    ExpressionTreeNode* minus(ExpressionTreeNode *a, ExpressionTreeNode *b);
    ExpressionTreeNode* times(ExpressionTreeNode *a, ExpressionTreeNode *b);

    // Boolean operators
    ExpressionTreeNode* eq(ExpressionTreeNode *a, ExpressionTreeNode *b);
    ExpressionTreeNode* less(ExpressionTreeNode *a, ExpressionTreeNode *b);
    ExpressionTreeNode* greater(ExpressionTreeNode *a, ExpressionTreeNode *b);

    ExpressionTreeNode *nodeT();
    ExpressionTreeNode *nodeNIL();

    bool treeToBool(ExpressionTreeNode *root);

    // Basic test suite (test/TestParser.h)
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
    ExpressionTreeNode* parseExpression(ExpressionTreeNode *root);
    std::string printExpression(ExpressionTreeNode *root);
};

#endif
