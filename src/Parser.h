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
#include "Evaluator.h"
#include "LexicalAnalyzer.h"
#include "ExpressionTreeNode.h"

using namespace std;

class Evaluator;

class Parser {
private:
    LexicalAnalyzer scanner;
    Evaluator evaluator;
    bool isAtom(Token t);

    // Parsing helper function
    void _parseExpression(ExpressionTreeNode *root);

    // Basic test suite (test/TestParser.h)
    void printExpression_test1();
    void printExpression_test2();
    void printExpression_test3();
    void printExpression_test4();
    void printExpression_test5();

public:
    Parser(LexicalAnalyzer &l, Evaluator &e) : scanner(l), evaluator(e) {}

    void start();
    void runTests();
    void printTokens();
    ExpressionTreeNode* parseExpression(ExpressionTreeNode *root);
    static std::string printExpression(ExpressionTreeNode *root);
    std::string printList(ExpressionTreeNode *root);
};

#endif
