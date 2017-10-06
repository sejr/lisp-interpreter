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
        cout << t.repr() << endl;
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
            // cout << scanner.getCurrentToken().repr() << endl;
            scanner.moveToNextToken();
        } else {
            ExpressionTreeNode *root = parseExpression(new ExpressionTreeNode());
            ExpressionTreeNode *evaluatedRoot = evaluateExpression(root);
            cout << printList(evaluatedRoot) << endl;
        }

    } while (scanner.getCurrentToken().getTokenType() != eof);
}

ExpressionTreeNode* Parser::evaluateExpression(ExpressionTreeNode *root) {
    // cout << "Inside evaluateExpression(): " << printExpression(root) << endl;
    // cout << "input tree length: " << len(root) << endl;

    if (treeToBool(atom(root))) {
        if (treeToBool(lit(root)) ||
            treeToBool(num(root)) ||
            treeToBool(null(root))) {
            return root;
        } else {
            cerr << "\nERROR: Unexpected atom: " << printExpression(root);
            exit(EXIT_FAILURE);
        }
    }

    ExpressionTreeNode *carResult = car(root);
    ExpressionTreeNode *cdrResult = cdr(root);
    string op = carResult->atom.repr();

    if (op.compare("ATOM") == 0) {
        if (len(root) != 2) {
            cerr << "\nERROR: Call to ATOM does not satisfy any existing call signature. ";
            cerr << "Valid options include:" << endl;
            cerr << "> (ATOM <s1>)" << endl;
            exit(EXIT_FAILURE);
        }
        return atom(evaluateExpression(cdrResult));
    } else if (op.compare("NULL") == 0) {
        if (len(root) != 2) {
            cerr << "\nERROR: Call to NULL does not satisfy any existing call signature. ";
            cerr << "Valid options include:" << endl;
            cerr << "> (NULL <s1>)" << endl;
            exit(EXIT_FAILURE);
        }
        cout << "len of NULL: " << len(root) << endl;
        return null(evaluateExpression(cdrResult));
    } else if (op.compare("INT") == 0) {
        if (len(root) != 2) {
            cerr << "\nERROR: Call to INT does not satisfy any existing call signature. ";
            cerr << "Valid options include:" << endl;
            cerr << "> (INT <s1>)" << endl;
            exit(EXIT_FAILURE);
        }
        // cout << "len of INT: " << len(root) << endl;
        return num(evaluateExpression(cdrResult));
    } else if (op.compare("QUOTE") == 0) {
        // cout << "len of QUOTE: " << len(root) << endl;
        return car(cdrResult);
    } else if (op.compare("CONS") == 0) {
        // cout << "len of CONS: " << len(root) << endl;
        return cons(evaluateExpression(car(cdrResult)), evaluateExpression(car(cdr(cdrResult))));
    } else if (op.compare("CAR") == 0) {
        // cout << "len of CAR: " << len(root) << endl;
        return car(car(evaluateExpression(cdrResult)));
    } else if (op.compare("CDR") == 0) {
        // cout << "len of CDR: " << len(root) << endl;
        return car(cdr(car(evaluateExpression(cdrResult))));
    } else if (op.compare("PLUS") == 0) {
        ExpressionTreeNode *leftOperand = evaluateExpression(car(cons(car(cdrResult), new ExpressionTreeNode())));
        ExpressionTreeNode *rightOperand = evaluateExpression(car(cdr(cdrResult)));
        return plus(evaluateExpression(leftOperand), evaluateExpression(rightOperand));
    } else if (op.compare("MINUS") == 0) {
        ExpressionTreeNode *leftOperand = evaluateExpression(car(cons(car(cdrResult), new ExpressionTreeNode())));
        ExpressionTreeNode *rightOperand = evaluateExpression(car(cdr(cdrResult)));
        return minus(evaluateExpression(leftOperand), evaluateExpression(rightOperand));
    } else if (op.compare("TIMES") == 0) {
        ExpressionTreeNode *leftOperand = evaluateExpression(car(cons(car(cdrResult), new ExpressionTreeNode())));
        ExpressionTreeNode *rightOperand = evaluateExpression(car(cdr(cdrResult)));
        return times(evaluateExpression(leftOperand), evaluateExpression(rightOperand));
    } else if (op.compare("LESS") == 0) {
        ExpressionTreeNode *leftOperand = evaluateExpression(car(cons(car(cdrResult), new ExpressionTreeNode())));
        ExpressionTreeNode *rightOperand = evaluateExpression(car(cdr(cdrResult)));
        return less(evaluateExpression(leftOperand), evaluateExpression(rightOperand));
    } else if (op.compare("GREATER") == 0) {
        ExpressionTreeNode *leftOperand = evaluateExpression(car(cons(car(cdrResult), new ExpressionTreeNode())));
        ExpressionTreeNode *rightOperand = evaluateExpression(car(cdr(cdrResult)));
        return greater(evaluateExpression(leftOperand), evaluateExpression(rightOperand));
    } else if (op.compare("EQ") == 0) {
        ExpressionTreeNode *leftOperand = evaluateExpression(car(cons(car(cdrResult), new ExpressionTreeNode())));
        ExpressionTreeNode *rightOperand = evaluateExpression(car(cdr(cdrResult)));
        return eq(evaluateExpression(leftOperand), evaluateExpression(rightOperand));
    }

    return root;
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
    // cout << scanner.getCurrentToken().repr() << endl;
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
            // cout << "beginlist\n";
            ExpressionTreeNode *newList = new ExpressionTreeNode();
            temp->leftChild = newList;
            while (scanner.getCurrentToken().getTokenType() != parenClose) {
                _parseExpression(newList);
                newList = newList->rightChild;
            }

            // cout << "endlist\n";
            scanner.moveToNextToken();
        }


    } else {
        cout << "Parse error: expected atom (numeric, literal) or list: "
                  << scanner.getCurrentToken().repr()
                  << endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * Traverses through a parse tree generated by parseExpression() which allows
 * us to print an expression in a way that is understandable to users.
 */
string Parser::printExpression(ExpressionTreeNode *root) {
    string result = "";

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

string Parser::printList(ExpressionTreeNode *root) {
    string result = "";

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
        cerr << "\nERROR: Function len() must be called on a list. ";
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
        if (len(root) == 1) {
            return nodeT();
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
        if (root->leftChild) {
            // cout << "root->leftChild" << endl;
            ExpressionTreeNode *left = root->leftChild;
            if (left->atom.getTokenType() == atomNumeric) {
                // cout << "left->atom" << endl;
                if (root->rightChild) {
                    // cout << "root->rightChild" << endl;
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
        if (root->leftChild) {
            ExpressionTreeNode *left = root->leftChild;
            if (left->atom.getTokenType() == atomLiteral) {
                if (root->rightChild) {
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
        // cout << "root" << endl;
        if (root->leftChild) {
            // cout << "root->leftChild" << endl;
            ExpressionTreeNode *left = root->leftChild;
            if (left->atom.getTokenType() == nil) {
                // cout << "left->atom" << endl;
                if (root->rightChild) {
                    // cout << "root->rightChild" << endl;
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
    if (len(a) == 0) a = cons(a, new ExpressionTreeNode());
    if (len(b) == 0) b = cons(b, new ExpressionTreeNode());

    if ((treeToBool(lit(a)) && treeToBool(lit(b))) ||
        (treeToBool(num(a)) && treeToBool(num(b)))) {
        ExpressionTreeNode *aToken = a->leftChild;
        ExpressionTreeNode *bToken = b->leftChild;

        bool eqFlag = aToken->atom.repr().compare(bToken->atom.repr());

        if (eqFlag == 0) {
            return car(nodeT());
        } else {
            return car(nodeNIL());
        }

    } else {
        cerr << "\nERROR: Call to EQ does not satisfy any existing call signature. ";
        cerr << "Valid options include:" << endl;
        cerr << "> (EQ <int> <int>)" << endl;
        cerr << "> (EQ <literal> <literal>)" << endl;
        cerr << "\nInputs provided were: " << printExpression(a) << ", " << printExpression(b) << endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * T if a > b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Parser::greater(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    if (len(a) == 0) a = cons(a, new ExpressionTreeNode());
    if (len(b) == 0) b = cons(b, new ExpressionTreeNode());

    if (treeToBool(num(a)) && treeToBool(num(b))) {
        ExpressionTreeNode *aToken = a->leftChild;
        ExpressionTreeNode *bToken = b->leftChild;
        int aValue = aToken->atom.getNumeric();
        int bValue = bToken->atom.getNumeric();

        if (aValue < bValue) {
            return car(nodeT());
        } else {
            return car(nodeNIL());
        }

    } else {
        cerr << "\nERROR: Call to GREATER does not satisfy any existing call signature. ";
        cerr << "Valid options include:" << endl;
        cerr << "> (GREATER <int> <int>)" << endl;
        cerr << "\nInputs provided were: " << printExpression(a) << ", " << printExpression(b) << endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * T if a < b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Parser::less(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    if (len(a) == 0) a = cons(a, new ExpressionTreeNode());
    if (len(b) == 0) b = cons(b, new ExpressionTreeNode());

    if (treeToBool(num(a)) && treeToBool(num(b))) {
        ExpressionTreeNode *aToken = a->leftChild;
        ExpressionTreeNode *bToken = b->leftChild;
        int aValue = aToken->atom.getNumeric();
        int bValue = bToken->atom.getNumeric();

        if (aValue < bValue) {
            return car(nodeT());
        } else {
            return car(nodeNIL());
        }

    } else {
        cerr << "\nERROR: Call to GREATER does not satisfy any existing call signature. ";
        cerr << "Valid options include:" << endl;
        cerr << "> (GREATER <int> <int>)" << endl;
        cerr << "\nInputs provided were: " << printExpression(a) << ", " << printExpression(b) << endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * Numeric result of a + b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Parser::plus(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    if (len(a) == 0) a = cons(a, new ExpressionTreeNode());
    if (len(b) == 0) b = cons(b, new ExpressionTreeNode());

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
        return car(resultTree);

    } else {
        cerr << "\nERROR: Call to PLUS does not satisfy any existing call signature. ";
        cerr << "Valid options include:" << endl;
        cerr << "> (PLUS <int> <int>)" << endl;
        cerr << "\nInputs provided were: " << printExpression(a) << ", " << printExpression(b) << endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * Numeric result of a - b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Parser::minus(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    if (len(a) == 0) a = cons(a, new ExpressionTreeNode());
    if (len(b) == 0) b = cons(b, new ExpressionTreeNode());

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
        return car(resultTree);

    } else {
        cerr << "\nERROR: Call to MINUS does not satisfy any existing call signature. ";
        cerr << "Valid options include:" << endl;
        cerr << "> (MINUS <int> <int>)" << endl;
        cerr << "\nInputs provided were: " << printExpression(a) << ", " << printExpression(b) << endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * Numeric result of a * b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Parser::times(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    if (len(a) == 0) a = cons(a, new ExpressionTreeNode());
    if (len(b) == 0) b = cons(b, new ExpressionTreeNode());

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
        cerr << "\nERROR: Call to TIMES does not satisfy any existing call signature. ";
        cerr << "Valid options include:" << endl;
        cerr << "> (TIMES <int> <int>)" << endl;
        cerr << "\nInputs provided were: " << printExpression(a) << ", " << printExpression(b) << endl;

        exit(EXIT_FAILURE);
    }
}
