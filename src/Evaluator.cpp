#include <vector>
#include "Evaluator.h"

ExpressionTreeNode* Evaluator::evaluateExpression(ExpressionTreeNode *root) {
    // cout << "LEN(" << printExpression(root) << ") = " << len(root) << endl;
    if (treeToBool(atom(root))) {
        if (treeToBool(cons(root, new ExpressionTreeNode)) ||
            treeToBool(num(cons(root, new ExpressionTreeNode()))) ||
            treeToBool(null(cons(root, new ExpressionTreeNode())))) {

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

        // cout << "checking if atom: " << printExpression(car(cdrResult)) << endl;
        return car(atom(evaluateExpression(car(cdrResult))));
    } else if (op.compare("NULL")    == 0) {
        if (len(root) != 2) {
            cerr << "\nERROR: Call to NULL does not satisfy any existing call signature. ";
            cerr << "Valid options include:" << endl;
            cerr << "> (NULL <s1>)" << endl;
            exit(EXIT_FAILURE);
        }
        // cout << "checking for null: " << printExpression(car(cdrResult)) << endl;
        return car(null(evaluateExpression(car(cdrResult))));
    } else if (op.compare("INT")     == 0) {
        if (len(root) != 2) {
            cerr << "\nERROR: Call to INT does not satisfy any existing call signature. ";
            cerr << "Valid options include:" << endl;
            cerr << "> (INT <s1>)" << endl;
            exit(EXIT_FAILURE);
        }
        // cout << "checking if int: " << printExpression(car(cdrResult)) << endl;
        return car(num(evaluateExpression(car(cdrResult))));
    } else if (op.compare("QUOTE")   == 0) {
        if (len(root) != 2) {
            cerr << "\nERROR: Call to QUOTE does not satisfy any existing call signature. ";
            cerr << "Valid options include:" << endl;
            cerr << "> (QUOTE <s1>)" << endl;
            exit(EXIT_FAILURE);
        }
        return car(cdrResult);
    } else if (op.compare("CONS")    == 0) {
        if (len(root) != 3) {
            cerr << "\nERROR: Call to CONS does not satisfy any existing call signature. ";
            cerr << "Valid options include:" << endl;
            cerr << "> (CONS <s1> <s2>)" << endl;
            exit(EXIT_FAILURE);
        }
        return cons(evaluateExpression(car(cdrResult)), evaluateExpression(car(cdr(cdrResult))));
    } else if (op.compare("CAR")     == 0) {
        if (len(root) != 2) {
            cerr << "\nERROR: Call to CAR does not satisfy any existing call signature. ";
            cerr << "Valid options include:" << endl;
            cerr << "> (CAR <s1>)" << endl;
            exit(EXIT_FAILURE);
        }

        return car(evaluateExpression(car(cdrResult)));
    } else if (op.compare("CDR")     == 0) {
        if (len(root) != 2) {
            cerr << "\nERROR: Call to CDR does not satisfy any existing call signature. ";
            cerr << "Valid options include:" << endl;
            cerr << "> (CDR <s1>)" << endl;
            exit(EXIT_FAILURE);
        }
        return cdr(evaluateExpression(car(cdrResult)));
    } else if (op.compare("PLUS")    == 0) {
        if (len(root) != 3) {
            cerr << "\nERROR: Call to PLUS does not satisfy any existing call signature. ";
            cerr << "Valid options include:" << endl;
            cerr << "> (PLUS <s1> <s2)" << endl;
            exit(EXIT_FAILURE);
        }
        ExpressionTreeNode *leftOperand = evaluateExpression(car(cons(car(cdrResult), new ExpressionTreeNode())));
        ExpressionTreeNode *rightOperand = evaluateExpression(car(cdr(cdrResult)));
        return plus(evaluateExpression(leftOperand), evaluateExpression(rightOperand));
    } else if (op.compare("MINUS")   == 0) {
        if (len(root) != 3) {
            cerr << "\nERROR: Call to MINUS does not satisfy any existing call signature. ";
            cerr << "Valid options include:" << endl;
            cerr << "> (MINUS <s1> <s2)" << endl;
            exit(EXIT_FAILURE);
        }
        ExpressionTreeNode *leftOperand = evaluateExpression(car(cons(car(cdrResult), new ExpressionTreeNode())));
        ExpressionTreeNode *rightOperand = evaluateExpression(car(cdr(cdrResult)));
        return minus(evaluateExpression(leftOperand), evaluateExpression(rightOperand));
    } else if (op.compare("TIMES")   == 0) {
        if (len(root) != 3) {
            cerr << "\nERROR: Call to TIMES does not satisfy any existing call signature. ";
            cerr << "Valid options include:" << endl;
            cerr << "> (TIMES <s1> <s2)" << endl;
            exit(EXIT_FAILURE);
        }
        ExpressionTreeNode *leftOperand = evaluateExpression(car(cons(car(cdrResult), new ExpressionTreeNode())));
        ExpressionTreeNode *rightOperand = evaluateExpression(car(cdr(cdrResult)));
        return times(evaluateExpression(leftOperand), evaluateExpression(rightOperand));
    } else if (op.compare("LESS")    == 0) {
        if (len(root) != 3) {
            cerr << "\nERROR: Call to LESS does not satisfy any existing call signature. ";
            cerr << "Valid options include:" << endl;
            cerr << "> (LESS <s1> <s2)" << endl;
            exit(EXIT_FAILURE);
        }
        ExpressionTreeNode *leftOperand = evaluateExpression(car(cons(car(cdrResult), new ExpressionTreeNode())));
        ExpressionTreeNode *rightOperand = evaluateExpression(car(cdr(cdrResult)));
        return less(evaluateExpression(leftOperand), evaluateExpression(rightOperand));
    } else if (op.compare("GREATER") == 0) {
        if (len(root) != 3) {
            cerr << "\nERROR: Call to GREATER does not satisfy any existing call signature. ";
            cerr << "Valid options include:" << endl;
            cerr << "> (GREATER <s1> <s2)" << endl;
            exit(EXIT_FAILURE);
        }
        ExpressionTreeNode *leftOperand = evaluateExpression(car(cons(car(cdrResult), new ExpressionTreeNode())));
        ExpressionTreeNode *rightOperand = evaluateExpression(car(cdr(cdrResult)));
        return greater(evaluateExpression(leftOperand), evaluateExpression(rightOperand));
    } else if (op.compare("EQ")      == 0) {
        if (len(root) != 3) {
            cerr << "\nERROR: Call to PLUS does not satisfy any existing call signature. ";
            cerr << "Valid options include:" << endl;
            cerr << "> (PLUS <s1> <s2)" << endl;
            exit(EXIT_FAILURE);
        }
        ExpressionTreeNode *leftOperand = evaluateExpression(car(cons(car(cdrResult), new ExpressionTreeNode())));
        ExpressionTreeNode *rightOperand = evaluateExpression(car(cdr(cdrResult)));
        return eq(leftOperand, rightOperand);
    } else if (op.compare("COND")    == 0) {
        // Here we are evaluating a sequence of conditional expressions that
        // could be any length.

        return cond(cdrResult);

    } else {
        cerr << "\nERROR: Invalid expression: " << printExpression(root) << endl;
        exit(EXIT_FAILURE);
    }

    // return root;
}

int Evaluator::len(ExpressionTreeNode *root) {
    int result = 0;

    if (root->leftChild) {
        result += 1;
    }

    if (root->rightChild) {
        return result + len(root->rightChild);
    } else {
        return result;
    }
}

bool Evaluator::treeToBool(ExpressionTreeNode *root) {
    if (root) {
        if (root->leftChild) {
            ExpressionTreeNode *left = root->leftChild;
            if (left->atom.getTokenType() == atomLiteral ||
                left->atom.getTokenType() == atomNumeric) {
                if (left->atom.getTokenType() == atomLiteral) {
                    if (left->atom.repr().compare("T") == 0) {
                        return true;
                    } else {
                        return false;
                    }
                } else {
                    return true;
                }
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
ExpressionTreeNode* Evaluator::nodeT() {
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
ExpressionTreeNode* Evaluator::nodeNIL() {
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
ExpressionTreeNode* Evaluator::car(ExpressionTreeNode *root) {
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
ExpressionTreeNode* Evaluator::cdr(ExpressionTreeNode *root) {
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
ExpressionTreeNode* Evaluator::cons(ExpressionTreeNode *left, ExpressionTreeNode *right) {
    ExpressionTreeNode *result = new ExpressionTreeNode();
    result->leftChild = left;
    result->rightChild = right;
    return result;
}

/**
 * T if root contains atom; NIL otherwise.
 */
ExpressionTreeNode* Evaluator::atom(ExpressionTreeNode *root) {
    if (root) {
        if (len(root) == 0) {
            return nodeT();
        } else if (len(root) == 1) {
            ExpressionTreeNode *carResult = car(root);
            if (carResult->atom.getTokenType() == atomLiteral) {
                if (carResult->atom.repr().compare("T") == 0) {
                    return nodeT();
                } else {
                    return nodeNIL();
                }
            } else if (carResult->atom.getTokenType() == atomNumeric) {
                return nodeT();
            } else if (carResult->atom.getTokenType() == nil) {
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
}

/**
 * T if root contains numeric atom; NIL otherwise.
 */
ExpressionTreeNode* Evaluator::num(ExpressionTreeNode *root) {
    if (root) {
        if (len(root) == 0) {
            if (root->atom.getTokenType() == atomNumeric) {
                return nodeT();
            } else {
                return nodeNIL();
            }
        } else if (len(root) == 1) {
            ExpressionTreeNode *carResult = car(root);
            if (carResult->atom.getTokenType() == atomNumeric) {
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
}

ExpressionTreeNode* Evaluator::lit(ExpressionTreeNode *root) {
    if (root) {
        if (len(root) == 0) {
            if (root->atom.getTokenType() == atomLiteral) {
                return nodeT();
            } else {
                return nodeNIL();
            }
        } else if (len(root) == 1) {
            ExpressionTreeNode *carResult = car(root);
            if (carResult->atom.getTokenType() == atomLiteral) {
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
}

/**
 * T if root contains NIL; NIL otherwise.
 */
ExpressionTreeNode* Evaluator::null(ExpressionTreeNode *root) {
    if (root) {
        if (len(root) == 0) {
            if (root->atom.getTokenType() == nil) {
                return nodeT();
            } else {
                return nodeNIL();
            }
        } else if (len(root) == 1) {
            ExpressionTreeNode *carResult = car(root);
            if (carResult->atom.getTokenType() == nil) {
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
}

/**
 * T if a and b are equal (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Evaluator::eq(ExpressionTreeNode *a, ExpressionTreeNode *b) {
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
        cerr << "\nInputs provided were: " << printExpression(car(a)) << ", " << printExpression(car(b)) << endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * T if a > b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Evaluator::greater(ExpressionTreeNode *a, ExpressionTreeNode *b) {
    if (len(a) == 0) a = cons(a, new ExpressionTreeNode());
    if (len(b) == 0) b = cons(b, new ExpressionTreeNode());

    if (treeToBool(num(a)) && treeToBool(num(b))) {
        ExpressionTreeNode *aToken = a->leftChild;
        ExpressionTreeNode *bToken = b->leftChild;
        int aValue = aToken->atom.getNumeric();
        int bValue = bToken->atom.getNumeric();

        if (aValue > bValue) {
            return car(nodeT());
        } else {
            return car(nodeNIL());
        }

    } else {
        cerr << "\nERROR: Call to GREATER does not satisfy any existing call signature. ";
        cerr << "Valid options include:" << endl;
        cerr << "> (GREATER <int> <int>)" << endl;
        cerr << "\nInputs provided were: " << printExpression(car(a)) << ", " << printExpression(car(b)) << endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * T if a < b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Evaluator::less(ExpressionTreeNode *a, ExpressionTreeNode *b) {
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
        cerr << "\nERROR: Call to LESS does not satisfy any existing call signature. ";
        cerr << "Valid options include:" << endl;
        cerr << "> (LESS <int> <int>)" << endl;
        cerr << "\nInputs provided were: " << printExpression(car(a)) << ", " << printExpression(car(b)) << endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * Numeric result of a + b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Evaluator::plus(ExpressionTreeNode *a, ExpressionTreeNode *b) {
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
        cerr << "\nInputs provided were: " << printExpression(car(a)) << ", " << printExpression(car(b)) << endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * Numeric result of a - b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Evaluator::minus(ExpressionTreeNode *a, ExpressionTreeNode *b) {
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
        cerr << "\nInputs provided were: " << printExpression(car(a)) << ", " << printExpression(car(b)) << endl;

        exit(EXIT_FAILURE);
    }
}

/**
 * Numeric result of a * b (checking for numeric atoms); NIL otherwise.
 */
ExpressionTreeNode* Evaluator::times(ExpressionTreeNode *a, ExpressionTreeNode *b) {
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
        cerr << "\nInputs provided were: " << printExpression(car(a)) << ", " << printExpression(car(b)) << endl;

        exit(EXIT_FAILURE);
    }
}

ExpressionTreeNode* Evaluator::cond(ExpressionTreeNode *root) {
    ExpressionTreeNode *condition = car(car(root));
    // cout << "Entering the cond function with condition: " << printExpression(condition) << endl;
    ExpressionTreeNode *conditionResult = evaluateExpression(condition);
    // cout << printExpression(conditionResult) << endl;
    if (treeToBool(cons(conditionResult, new ExpressionTreeNode()))) {
        // cout << "condition evaluates to TRUE!" << endl;

        // cout << printExpression(car(cdr(car(root)))) << endl;
        // cout << printExpression(evaluateExpression(car(cdr(car(root))))) << endl;

        return evaluateExpression(car(cdr(car(root))));
    } else {
        // cout << "Inside else" << endl;
        if (root->rightChild) {
            // cout << "Inside root->rightChild" << endl;
            return cond(cdr(root));
        } else {
            cerr << "\nERROR: Reached end of conditional expression without valid execution path." << endl;
            exit(EXIT_FAILURE);
        }

        return root;
    }
}

/**
 * Traverses through a parse tree generated by parseExpression() which allows
 * us to print an expression in a way that is understandable to users.
 */
string Evaluator::printExpression(ExpressionTreeNode *root) {

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
