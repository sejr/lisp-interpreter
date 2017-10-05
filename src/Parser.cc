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
            ExpressionTreeNode *evaluatedRoot = evaluateExpression(root);
            std::cout << printExpression(evaluatedRoot) << std::endl;
        }

    } while (scanner.getCurrentToken().getTokenType() != eof);
}

ExpressionTreeNode* Parser::evaluateExpression(ExpressionTreeNode *root) {
    // std::cout << "Inside evaluateExpression()" << std::endl;
    ExpressionTreeNode *carResult = car(root);
    ExpressionTreeNode *cdrResult = cdr(root);
    // std::cout << "> car: " << printExpression(carResult) << std::endl;
    // std::cout << "> cdr: " << printExpression(cdrResult) << std::endl;
    std::string op = carResult->atom.repr();

    if (treeToBool(atom(root))) {
        if (treeToBool(lit(root)) ||
            treeToBool(num(root)) ||
            treeToBool(null(root))){

            return root;
        } else {
            std::cerr << "\nERROR: Unexpected atom: " << printExpression(root);
            exit(EXIT_FAILURE);
        }
    } else if (op.compare("ATOM") == 0) {
        return atom(evaluateExpression(cdrResult));
    } else if (op.compare("PLUS") == 0) {
        return plus(evaluateExpression(car(cdrResult)), evaluateExpression(cdr(cdrResult)));
    } else if (op.compare("MINUS") == 0) {
        return minus(evaluateExpression(car(cdrResult)), evaluateExpression(cdr(cdrResult)));
    } else if (op.compare("TIMES") == 0) {
        return times(evaluateExpression(car(cdrResult)), evaluateExpression(cdr(cdrResult)));
    } else if (op.compare("LESS") == 0) {
        return less(evaluateExpression(car(cdrResult)), evaluateExpression(cdr(cdrResult)));
    } else if (op.compare("GREATER") == 0) {
        return greater(evaluateExpression(car(cdrResult)), evaluateExpression(cdr(cdrResult)));
    } else if (op.compare("EQ") == 0) {
        return eq(evaluateExpression(car(cdrResult)), evaluateExpression(cdr(cdrResult)));
    } else if (op.compare("CAR") == 0) {
        return car(car(evaluateExpression(cdrResult)));
    } else if (op.compare("CDR") == 0) {
        return cdr(car(evaluateExpression(cdrResult)));
    } else if (op.compare("QUOTE") == 0) {
        return car(cdrResult);
    } else if (op.compare("CONS") == 0) {
        return cons(evaluateExpression(cdrResult), evaluateExpression(cdr(cdrResult)));
    } else {
        return nodeNIL();
    }

    // if (treeToBool(atom(carResult))) {
    //     std::cout << "Hit singleton atom" << std::endl;
    //     return nodeNIL();
    // } else {
    //     if (treeToBool(atom(root))) {
    //         std::cout << "root is an atom" << std::endl;
    //         if (treeToBool(lit(root)) ||
    //             treeToBool(num(root)) ||
    //             treeToBool(null(root))){

    //             std::cout << "root is valid" << std::endl;
    //             return root;
    //         } else {
    //             std::cerr << "\nERROR: Unexpected atom: " << printExpression(root);
    //             exit(EXIT_FAILURE);
    //         }
    //     } else if (op.compare("ATOM")) {
    //         std::cout << "cdrResult: " << printExpression(cdrResult) << std::endl;
    //         std::cout << "root is (ATOM s1)" << std::endl;
    //         return atom(evaluateExpression(cdrResult));
    //     } else if (op.compare("PLUS")) {
    //         std::cout << "root is (PLUS s1 s2)" << std::endl;
    //         return plus(evaluateExpression(car(cdrResult)), evaluateExpression(cdr(cdrResult)));
    //     } else if (op.compare("MINUS")) {
    //         std::cout << "root is (MINUS s1 s2)" << std::endl;
    //         return minus(evaluateExpression(car(cdrResult)), evaluateExpression(cdr(cdrResult)));
    //     } else if (op.compare("TIMES")) {
    //         std::cout << "root is (TIMES s1 s2)" << std::endl;
    //         return times(evaluateExpression(car(cdrResult)), evaluateExpression(cdr(cdrResult)));
    //     } else if (op.compare("LESS")) {
    //         std::cout << "root is (LESS s1 s2)" << std::endl;
    //         return less(evaluateExpression(car(cdrResult)), evaluateExpression(cdr(cdrResult)));
    //     } else if (op.compare("GREATER")) {
    //         std::cout << "root is (GREATER s1 s2)" << std::endl;
    //         return greater(evaluateExpression(car(cdrResult)), evaluateExpression(cdr(cdrResult)));
    //     } else if (op.compare("EQ")) {
    //         std::cout << "root is (EQ s1 s2)" << std::endl;
    //         return eq(evaluateExpression(car(cdrResult)), evaluateExpression(cdr(cdrResult)));
    //     } else if (op.compare("CAR") == 0) {
    //         std::cout << "root is (CAR s1)" << std::endl;
    //         return car(evaluateExpression(cdrResult));
    //     } else if (op.compare("CDR") == 0) {
    //         std::cout << "root is (CDR s1)" << std::endl;
    //         return cdr(evaluateExpression(cdrResult));
    //     } else {
    //         return nodeNIL();
    //     }
    // }
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
        if (treeToBool(null(root))) {
            return result; // Handle NIL / empty
        } else if (root->leftChild) {
            result += 1;
            if (root->rightChild) {
                result += len(root->rightChild);
            }
            return result;
        } else {
            return result;
        }
    } else {
        std::cerr << "\nERROR: Function len() must be called on a list. ";
        exit(EXIT_FAILURE);
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
    if ((treeToBool(lit(a)) && treeToBool(lit(b))) ||
        (treeToBool(num(a)) && treeToBool(num(b)))) {

        ExpressionTreeNode *aToken = a->leftChild;
        ExpressionTreeNode *bToken = b->leftChild;

        bool eqFlag = aToken->atom.repr().compare(bToken->atom.repr());

        if (eqFlag == 0) {
            return nodeT();
        } else {
            return nodeNIL();
        }

    } else {
        std::cerr << "\nERROR: Call to EQ does not satisfy any existing call signature. ";
        std::cerr << "Valid options include:" << std::endl;
        std::cerr << "> (EQ <int> <int>)" << std::endl;
        std::cerr << "> (EQ <literal> <literal>)" << std::endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * T if a > b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Parser::greater(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    if (treeToBool(num(a)) && treeToBool(num(b))) {

        ExpressionTreeNode *aToken = a->leftChild;
        ExpressionTreeNode *bToken = b->leftChild;
        int aValue = aToken->atom.getNumeric();
        int bValue = bToken->atom.getNumeric();

        if (aValue > bValue) {
            return nodeT();
        } else {
            return nodeNIL();
        }

    } else {
        std::cerr << "\nERROR: Call to GREATER does not satisfy any existing call signature. ";
        std::cerr << "Valid options include:" << std::endl;
        std::cerr << "> (GREATER <int> <int>)" << std::endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * T if a < b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Parser::less(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    if (treeToBool(num(a)) && treeToBool(num(b))) {

        ExpressionTreeNode *aToken = a->leftChild;
        ExpressionTreeNode *bToken = b->leftChild;
        int aValue = aToken->atom.getNumeric();
        int bValue = bToken->atom.getNumeric();

        if (aValue < bValue) {
            return nodeT();
        } else {
            return nodeNIL();
        }

    } else {
        std::cerr << "\nERROR: Call to GREATER does not satisfy any existing call signature. ";
        std::cerr << "Valid options include:" << std::endl;
        std::cerr << "> (GREATER <int> <int>)" << std::endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * Numeric result of a + b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Parser::plus(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    if (treeToBool(num(a)) && treeToBool(num(b))) {

        ExpressionTreeNode *aToken = a->leftChild;
        ExpressionTreeNode *bToken = b->leftChild;
        int aValue = aToken->atom.getNumeric();
        int bValue = bToken->atom.getNumeric();
        int result = aValue + bValue;

        ExpressionTreeNode *resultTree = new ExpressionTreeNode();
        ExpressionTreeNode *resultValue = new ExpressionTreeNode();
        resultValue->atom = Token(result);
        resultTree->leftChild = resultValue;
        resultTree->rightChild = new ExpressionTreeNode();
        return resultTree;

    } else {
        std::cerr << "\nERROR: Call to PLUS does not satisfy any existing call signature. ";
        std::cerr << "Valid options include:" << std::endl;
        std::cerr << "> (PLUS <int> <int>)" << std::endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * Numeric result of a - b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Parser::minus(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    if (treeToBool(num(a)) && treeToBool(num(b))) {

        ExpressionTreeNode *aToken = a->leftChild;
        ExpressionTreeNode *bToken = b->leftChild;
        int aValue = aToken->atom.getNumeric();
        int bValue = bToken->atom.getNumeric();
        int result = aValue - bValue;

        ExpressionTreeNode *resultTree = new ExpressionTreeNode();
        ExpressionTreeNode *resultValue = new ExpressionTreeNode();
        resultValue->atom = Token(result);
        resultTree->leftChild = resultValue;
        resultTree->rightChild = new ExpressionTreeNode();
        return resultTree;

    } else {
        std::cerr << "\nERROR: Call to MINUS does not satisfy any existing call signature. ";
        std::cerr << "Valid options include:" << std::endl;
        std::cerr << "> (MINUS <int> <int>)" << std::endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * Numeric result of a * b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Parser::times(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    if (treeToBool(num(a)) && treeToBool(num(b))) {

        ExpressionTreeNode *aToken = a->leftChild;
        ExpressionTreeNode *bToken = b->leftChild;
        int aValue = aToken->atom.getNumeric();
        int bValue = bToken->atom.getNumeric();
        int result = aValue * bValue;

        ExpressionTreeNode *resultTree = new ExpressionTreeNode();
        ExpressionTreeNode *resultValue = new ExpressionTreeNode();
        resultValue->atom = Token(result);
        resultTree->leftChild = resultValue;
        resultTree->rightChild = new ExpressionTreeNode();
        return resultTree;

    } else {
        std::cerr << "\nERROR: Call to TIMES does not satisfy any existing call signature. ";
        std::cerr << "Valid options include:" << std::endl;
        std::cerr << "> (TIMES <int> <int>)" << std::endl;

        exit(EXIT_FAILURE);
    }
}
