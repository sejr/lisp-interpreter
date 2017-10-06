#ifndef EVALUATOR_H
#define EVALUATOR_H

// The Evaluator, as its name implies, evaluates the lisp expressions and,
// in that sense, is the meat of our interpreter.

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "Token.h"
#include "LexicalAnalyzer.h"
#include "ExpressionTreeNode.h"

using namespace std;

class Evaluator {
private:

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

public:
    ExpressionTreeNode* evaluateExpression(ExpressionTreeNode *root);
    string printExpression(ExpressionTreeNode *root);
};

#endif
