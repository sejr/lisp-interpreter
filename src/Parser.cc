#include "Parser.h"

/**
 * This is a helper function that will just iterate through every token that was
 * generated in the lexical analyzer (scanner), and print it off. While this was
 * more useful in earlier parts of the project, its used now only for debugging.
 */
void Parser::printTokens() {
    Token t = scanner.getCurrentToken();

    while (
        t.getTokenType() != eof &&
        t.getTokenType() != error
    ) {
        std::cout << t.repr() << std::endl;
        scanner.moveToNextToken();
        t = scanner.getCurrentToken();
    }
}

/**
 * Here we begin the process of parsing expressions in our lisp program. As long
 * as the current token isn't an EOF, we want to continue iterating through
 * expressions to build our parse tree (see parseExpression() for more info).
 */
void Parser::start() {
    do {
        ExpressionTreeNode *root = new ExpressionTreeNode();
        parseExpression(root);
        printExpression(root);
        std::cout << std::endl;
    } while (scanner.getCurrentToken().getTokenType() != eof);
}

/**
 * This method is able to parse a single expression, corresponding to our CFG:
 *
 *     <Start> ::= <Expr> <Start> | <Expr> eof
 *     <Expr>  ::= atom | ( <List> )
 *     <List>  ::= <Expr> <List>
 *
 *  The result is a binary parse tree that will allow us to traverse the program
 *  later on in our interpreter. In this binary tree, the left child of the root
 *  will be the head of a lisp statement, and the right child will be its tail.
 */
void Parser::parseExpression(ExpressionTreeNode *root) {
    ExpressionTreeNode *current = new ExpressionTreeNode();
    current = root;

    if (
        scanner.getCurrentToken().getTokenType() == atomLiteral ||
        scanner.getCurrentToken().getTokenType() == atomNumeric
    ) {
        current->atom = scanner.getCurrentToken();
        current = root->rightChild;
        scanner.moveToNextToken(); // CONSUME ATOM
    } else if (scanner.getCurrentToken().getTokenType() == parenOpen) {
        ExpressionTreeNode *temp = new ExpressionTreeNode();
        current->leftChild = temp;
        scanner.moveToNextToken(); // CONSUME OPEN PARENTHESIS
        while (scanner.getCurrentToken().getTokenType() != parenClose) {
            parseExpression(temp);
        }
        current->rightChild = new ExpressionTreeNode();
        scanner.moveToNextToken(); // CONSUME CLOSE PARENTHESIS
    } else {
        std::cout << "ERROR: Parse error on token "
                  << scanner.getCurrentToken().repr()
                  << std::endl;
        exit(EXIT_FAILURE);
    }
}

/**
 * Traverses through a parse tree generated by parseExpression() which allows
 * us to print an expression in a way that is understandable to users.
 */
void Parser::printExpression(ExpressionTreeNode *root) {
    if (!root->leftChild && !root->rightChild) {
        std::cout << root->atom.repr();
    } else {
        std::cout << "(";
        printExpression(root->leftChild);
        std::cout << " . ";
        printExpression(root->rightChild);
        std::cout << ")";
    }
}

/* EXPRESSION TREE NODE *******************************************************/
