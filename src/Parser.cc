#include "Parser.h"
#include "../test/Tookit.h"
#include "../test/TestParser.h"

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
        if (isAtom(scanner.getCurrentToken())) {
            // std::cout << scanner.getCurrentToken().repr() << std::endl;
            scanner.moveToNextToken();
        } else {
            ExpressionTreeNode *root = parseExpression(new ExpressionTreeNode());
            std::cout << printExpression(root) << std::endl << std::endl;

            std::cout << "> len:        " << len(root) << std::endl;

            ExpressionTreeNode *carResult  = car(root);
            ExpressionTreeNode *cdrResult  = cdr(root);
            ExpressionTreeNode *consResult = cons(carResult, cdrResult);

            std::cout << "> car:        " << printExpression(carResult) << std::endl;
            std::cout << "> cdr:        " << printExpression(cdrResult) << std::endl;
            std::cout << "> cons:       " << printExpression(consResult) << std::endl;

            ExpressionTreeNode *rootAtomResult = atom(root);
            ExpressionTreeNode *carAtomResult = atom(carResult);
            ExpressionTreeNode *cdrAtomResult = atom(carResult);

            ExpressionTreeNode *nullAtomResult = null(root);
            ExpressionTreeNode *litAtomResult = lit(root);
            ExpressionTreeNode *numAtomResult = num(root);

            if (treeToBool(rootAtomResult)) {
                std::cout << "> atom" << std::endl;
            }

            if (treeToBool(nullAtomResult)) {
                std::cout << "> null atom" << std::endl;
            }

            if (treeToBool(litAtomResult)) {
                std::cout << "> literal atom" << std::endl;
            }

            if (treeToBool(numAtomResult)) {
                std::cout << "> numeric atom" << std::endl;
            }
            
            std::cout << std::endl;
        }

    } while (scanner.getCurrentToken().getTokenType() != eof);
}

ExpressionTreeNode* Parser::parseExpression(ExpressionTreeNode *root) {
    _parseExpression(root);
    root = root->leftChild;
    return root;
}

/**
 * This method is able to parse a single expression, corresponding to our CFG:
 *
 *     <Start> ::= <Expr> <Start> | <Expr> eof
 *     <Expr>  ::= atom | ( <List> )
 *     <List>  ::= <Expr> <List>
 *
 * The result is a binary parse tree that will allow us to traverse the program
 * later on in our interpreter. In this binary tree, the left child of the root
 * will be the head of a lisp statement, and the right child will be its tail.
 */
void Parser::_parseExpression(ExpressionTreeNode *root) {
    // std::cout << scanner.getCurrentToken().repr() << std::endl;
    ExpressionTreeNode *nodeNIL = new ExpressionTreeNode();

    // Scaffold out an empty node.
    ExpressionTreeNode *current = root;
    current->leftChild = nodeNIL;
    current->rightChild = nodeNIL;

    if (isAtom(scanner.getCurrentToken())) {
        // In this case, we have an atom. Construct a ETN with the left child
        // containing the atom itself; the right child remains NIL.
        ExpressionTreeNode *newAtomicNode = new ExpressionTreeNode();
        newAtomicNode->atom = scanner.getCurrentToken();
        current->leftChild = newAtomicNode;
        scanner.moveToNextToken();
    } else if (scanner.getCurrentToken().getTokenType() == parenOpen) {
        ExpressionTreeNode *temp = current;
        scanner.moveToNextToken(); // Consume opening parenthesis.
        if (scanner.getCurrentToken().getTokenType() == parenClose) {
            // Here, we have a NIL token.
            scanner.moveToNextToken();
            temp = temp->leftChild;
        } else {
            // We have a LIST.
            // std::cout << "beginlist\n";
            ExpressionTreeNode *newList = new ExpressionTreeNode();
            temp->leftChild = newList;
            while (scanner.getCurrentToken().getTokenType() != parenClose) {
                _parseExpression(newList);
                newList = newList->rightChild;
            }

            // std::cout << "endlist\n";
            scanner.moveToNextToken();
        }


    } else {
        std::cout << "Parse error: expected atom (numeric, literal) or list: "
                  << scanner.getCurrentToken().repr()
                  << std::endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * Traverses through a parse tree generated by parseExpression() which allows
 * us to print an expression in a way that is understandable to users.
 */
std::string Parser::printExpression(ExpressionTreeNode *root) {
    std::string result = "";

    if (root) {
        if (root->leftChild) {
            result.append("(");
            result.append(printExpression(root->leftChild));
            result.append(" . ");
            if (root->rightChild) {
                result.append(printExpression(root->rightChild));
            } else {
                result.append("NIL");
            }
            result.append(")");
        } else {
            result.append(root->atom.repr());
        }
    }

    return result;
}

bool Parser::isAtom(Token t) {
    return t.getTokenType() == atomLiteral ||
           t.getTokenType() == atomNumeric ||
           t.getTokenType() == nil;
}

int Parser::len(ExpressionTreeNode *root) {
    int result = 0;
    if (root) {
        if (root->leftChild && root->leftChild->atom.getTokenType() != nil) {
            result += 1;
            if (root->rightChild) {
                result += len(root->rightChild);
            }
            return result;
        } else {
            return result;
        }
    } else {
        return result;
    }
}

bool Parser::treeToBool(ExpressionTreeNode *root) {
    if (root) {
        if (root->leftChild) {
            ExpressionTreeNode *left = root->leftChild;
            if (left->atom.getTokenType() == atomLiteral ||
                left->atom.getTokenType() == atomNumeric) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

/**
 * Returns a new T node.
 */
ExpressionTreeNode* Parser::nodeT() {
    ExpressionTreeNode *result = new ExpressionTreeNode();
    ExpressionTreeNode *value = new ExpressionTreeNode();
    value->atom = Token("T");
    result->leftChild = value;
    result->rightChild = new ExpressionTreeNode();
    return result;
}

/**
 * Returns a new NIL node.
 */
ExpressionTreeNode* Parser::nodeNIL() {
    ExpressionTreeNode *result = new ExpressionTreeNode();
    ExpressionTreeNode *value = new ExpressionTreeNode();
    value->atom = Token("nil");
    result->leftChild = value;
    result->rightChild = new ExpressionTreeNode();
    return result;
}

/**
 * Implementation of the Lisp CAR function which returns the head of a list.
 */
ExpressionTreeNode* Parser::car(ExpressionTreeNode *root) {
    if (root) {
        if (root->leftChild) {
            return root->leftChild;
        } else {
            ExpressionTreeNode *undefined = new ExpressionTreeNode();
            return undefined;
        }
    } else {
        ExpressionTreeNode *undefined = new ExpressionTreeNode();
        return undefined;
    }
}

/**
 * Implementation of the lisp CDR function which returns the tail of a list.
 */
ExpressionTreeNode* Parser::cdr(ExpressionTreeNode *root) {
    if (root) {
        if (root->rightChild) {
            return root->rightChild;
        } else {
            ExpressionTreeNode *undefined = new ExpressionTreeNode();
            return undefined;
        }  
    } else {
        ExpressionTreeNode *undefined = new ExpressionTreeNode();
        return undefined;
    }  
}

/**
 * Implementation of the lisp CONS function, which returns a binary tree where
 * the left child is the first parameter, and the right child is the second.
 */
ExpressionTreeNode* Parser::cons(ExpressionTreeNode *left, ExpressionTreeNode *right) {
    ExpressionTreeNode *result = new ExpressionTreeNode();
    result->leftChild = left;
    result->rightChild = right;
    return result;
}

/**
 * T if root contains atom; NIL otherwise.
 */
ExpressionTreeNode* Parser::atom(ExpressionTreeNode *root) {
    if (root) {
        // std::cout << "root" << std::endl;
        if (root->leftChild) {
            // std::cout << "root->leftChild" << std::endl;
            ExpressionTreeNode *left = root->leftChild;
            if (isAtom(left->atom)) {
                // std::cout << "left->atom" << std::endl;
                if (root->rightChild) {
                    // std::cout << "root->rightChild" << std::endl;
                    ExpressionTreeNode *right = root->rightChild;
                    if (right->atom.getTokenType() == nil) {
                        if (!right->leftChild && !right->rightChild) {
                            return nodeT();
                        } else {
                            return nodeNIL();
                        }
                    } else {
                        return nodeNIL();
                    }
                } else {
                    return nodeNIL();
                }
            } else {
                return nodeNIL();
            }
        } else {
            return nodeNIL();
        }
    } else {
        return nodeNIL();
    }
}

/**
 * T if root contains numeric atom; NIL otherwise.
 */
ExpressionTreeNode* Parser::num(ExpressionTreeNode *root) {
    if (root) {
        // std::cout << "root" << std::endl;
        if (root->leftChild) {
            // std::cout << "root->leftChild" << std::endl;
            ExpressionTreeNode *left = root->leftChild;
            if (left->atom.getTokenType() == atomNumeric) {
                // std::cout << "left->atom" << std::endl;
                if (root->rightChild) {
                    // std::cout << "root->rightChild" << std::endl;
                    ExpressionTreeNode *right = root->rightChild;
                    if (right->atom.getTokenType() == nil) {
                        if (!right->leftChild && !right->rightChild) {
                            return nodeT();
                        } else {
                            return nodeNIL();
                        }
                    } else {
                        return nodeNIL();
                    }
                } else {
                    return nodeNIL();
                }
            } else {
                return nodeNIL();
            }
        } else {
            return nodeNIL();
        }
    } else {
        return nodeNIL();
    }
}

ExpressionTreeNode* Parser::lit(ExpressionTreeNode *root) {
    if (root) {
        // std::cout << "root" << std::endl;
        if (root->leftChild) {
            // std::cout << "root->leftChild" << std::endl;
            ExpressionTreeNode *left = root->leftChild;
            if (left->atom.getTokenType() == atomLiteral) {
                // std::cout << "left->atom" << std::endl;
                if (root->rightChild) {
                    // std::cout << "root->rightChild" << std::endl;
                    ExpressionTreeNode *right = root->rightChild;
                    if (right->atom.getTokenType() == nil) {
                        if (!right->leftChild && !right->rightChild) {
                            return nodeT();
                        } else {
                            return nodeNIL();
                        }
                    } else {
                        return nodeNIL();
                    }
                } else {
                    return nodeNIL();
                }
            } else {
                return nodeNIL();
            }
        } else {
            return nodeNIL();
        }
    } else {
        return nodeNIL();
    }
}

/**
 * T if root contains NIL; NIL otherwise.
 */
ExpressionTreeNode* Parser::null(ExpressionTreeNode *root) {
    if (root) {
        // std::cout << "root" << std::endl;
        if (root->leftChild) {
            // std::cout << "root->leftChild" << std::endl;
            ExpressionTreeNode *left = root->leftChild;
            if (left->atom.getTokenType() == nil) {
                // std::cout << "left->atom" << std::endl;
                if (root->rightChild) {
                    // std::cout << "root->rightChild" << std::endl;
                    ExpressionTreeNode *right = root->rightChild;
                    if (right->atom.getTokenType() == nil) {
                        if (!right->leftChild && !right->rightChild) {
                            return nodeT();
                        } else {
                            return nodeNIL();
                        }
                    } else {
                        return nodeNIL();
                    }
                } else {
                    return nodeNIL();
                }
            } else {
                return nodeNIL();
            }
        } else {
            return nodeNIL();
        }
    } else {
        return nodeNIL();
    }
}

/**
 * T if a and b are equal (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Parser::eq(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    return nodeNIL();
}

/**
 * T if a > b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Parser::greater(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    return nodeNIL();
}

/**
 * T if a < b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Parser::less(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    return nodeNIL();
}

/**
 * Numeric result of a + b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Parser::plus(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    return nodeNIL();
}

/**
 * Numeric result of a - b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Parser::minus(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    return nodeNIL();
}

/**
 * Numeric result of a * b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Parser::times(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    return nodeNIL();
}
