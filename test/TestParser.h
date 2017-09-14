#include "../src/Parser.h"

void Parser::runTests() {
    printExpression_test1(); // TEST1 TEST2 TEST3
    std::cout << std::endl;
    printExpression_test2(); // (ROOT TEST1 TEST2 TEST3)
    std::cout << std::endl;
    printExpression_test3(); // (3 5 (XYZ) 7)
    std::cout << std::endl;
    printExpression_test4(); // (NIL 5 () (()) 7 (() 9 ()) )
    std::cout << std::endl;
    printExpression_test5(); // (DEFUN F23 (X) (PLUS X 12 15))
    std::cout << std::endl;
}

/**
 * TEST WITH INPUT: TEST1 TEST2 TEST3
 * EXPECTED RESULT: Separate expressions; each in atom (not list) form.
 */
void Parser::printExpression_test1() {
    Toolkit::info("Parser::printExpression() TEST ONE");
    Toolkit::info("TEST1 TEST2 TEST3");
    ExpressionTreeNode *root = new ExpressionTreeNode();
    ExpressionTreeNode *rootLeaf0 = new ExpressionTreeNode();
    rootLeaf0->atom = Token("TEST1");
    root->leftChild = rootLeaf0;

    std::string result = printExpression(root, IS_ATOM);

    std::string expected = "TEST1";
    Toolkit::debug(result + "\n");
    if (result.compare(expected) != 0) {
        Toolkit::error("FAILURE");
        Toolkit::errorDetail("Expected: " + expected);
        std::cout << std::endl;
        return;
    }
    std::cout << std::endl;

    rootLeaf0->atom = Token("TEST2");
    result = printExpression(root, IS_ATOM);
    expected = "TEST2";
    Toolkit::debug(result + "\n");
    if (result.compare(expected) != 0) {
        Toolkit::error("FAILURE");
        Toolkit::errorDetail("Expected: " + expected);
        std::cout << std::endl;
        return;
    }
    std::cout << std::endl;

    rootLeaf0->atom = Token("TEST3");
    result = printExpression(root, IS_ATOM);
    expected = "TEST3";
    Toolkit::debug(result + "\n");
    if (result.compare(expected) != 0) {
        Toolkit::error("FAILURE");
        Toolkit::errorDetail("Expected: " + expected);
        std::cout << std::endl;
        return;
    }
    std::cout << std::endl;

    Toolkit::success("SUCCESS");
}

/**
 * TEST WITH INPUT: (ROOT TEST1 TEST2 TEST3)
 * EXPECTED RESULT: (ROOT . (TEST1 . (TEST2 . (TEST3 . NIL))))
 */
void Parser::printExpression_test2() {
    Toolkit::info("Parser::printExpression() TEST TWO");
    Toolkit::info("(ROOT TEST1 TEST2 TEST3)");
    ExpressionTreeNode *root = new ExpressionTreeNode();

    ExpressionTreeNode *rootLeaf0 = new ExpressionTreeNode();
    ExpressionTreeNode *testLeaf1 = new ExpressionTreeNode();
    ExpressionTreeNode *testLeaf2 = new ExpressionTreeNode();
    ExpressionTreeNode *testLeaf3 = new ExpressionTreeNode();

    rootLeaf0->atom = Token("ROOT");
    testLeaf1->atom = Token("TEST1");
    testLeaf2->atom = Token("TEST2");
    testLeaf3->atom = Token("TEST3");

    root->leftChild = rootLeaf0;
    ExpressionTreeNode *firstChild = new ExpressionTreeNode();
    firstChild->leftChild = testLeaf1;
    ExpressionTreeNode *secondChild = new ExpressionTreeNode();
    secondChild->leftChild = testLeaf2;
    ExpressionTreeNode *thirdChild = new ExpressionTreeNode();
    thirdChild->leftChild = testLeaf3;

    secondChild->rightChild = thirdChild;
    firstChild->rightChild = secondChild;
    root->rightChild = firstChild;

    std::string result = printExpression(root, IS_NOT_ATOM);
    std::string expected = "(ROOT . (TEST1 . (TEST2 . (TEST3 . NIL))))";

    Toolkit::debug(result);
    std::cout << std::endl;
    if (result.compare(expected) == 0) {
        Toolkit::success("SUCCESS");
    } else {
        Toolkit::error("FAILURE");
        Toolkit::errorDetail("Expected: " + expected);
    }
}

/**
 * TEST WITH INPUT: (3 5 (XYZ) 7)
 * EXPECTED RESULT: (3 . (5 . ((XYZ . NIL) . (7 . NIL))))
 */
void Parser::printExpression_test3() {
    Toolkit::info("Parser::printExpression() TEST THREE");
    Toolkit::info("(3 5 (XYZ) 7)");

    ExpressionTreeNode *Node3 = new ExpressionTreeNode();
    ExpressionTreeNode *Node5 = new ExpressionTreeNode();
    ExpressionTreeNode *Node7 = new ExpressionTreeNode();
    ExpressionTreeNode *NodeXYZ = new ExpressionTreeNode();
    ExpressionTreeNode *NodeNIL = new ExpressionTreeNode();

    Node3->atom = Token(3);
    Node5->atom = Token(5);
    Node7->atom = Token(7);
    NodeXYZ->atom = Token("XYZ");

    ExpressionTreeNode *Parent_XYZ_NIL = new ExpressionTreeNode();
    Parent_XYZ_NIL->leftChild = NodeXYZ;
    Parent_XYZ_NIL->rightChild = NodeNIL;

    ExpressionTreeNode *Parent_7_NIL = new ExpressionTreeNode();
    Parent_7_NIL->leftChild = Node7;
    Parent_7_NIL->rightChild = NodeNIL;

    ExpressionTreeNode *Parent_XYZ_7 = new ExpressionTreeNode();
    Parent_XYZ_7->leftChild = Parent_XYZ_NIL;
    Parent_XYZ_7->rightChild = Parent_7_NIL;

    ExpressionTreeNode *Parent_5_PXYZ7 = new ExpressionTreeNode();
    Parent_5_PXYZ7->leftChild = Node5;
    Parent_5_PXYZ7->rightChild = Parent_XYZ_7;

    ExpressionTreeNode *root = new ExpressionTreeNode();
    root->leftChild = Node3;
    root->rightChild = Parent_5_PXYZ7;

    std::string result = printExpression(root, IS_NOT_ATOM);
    std::string expected = "(3 . (5 . ((XYZ . NIL) . (7 . NIL))))";

    Toolkit::debug(result);
    std::cout << std::endl;
    if (result.compare(expected) == 0) {
        Toolkit::success("SUCCESS");
    } else {
        Toolkit::error("FAILURE");
        Toolkit::errorDetail("Expected: " + expected);
    }
}

/**
 * TEST WITH INPUT: (NIL 5 () (()) 7 (() 9 ()) )
 * EXPECTED RESULT: (NIL . (5 . (NIL . ((NIL . NIL) . (7 . ((NIL . (9 . (NIL . NIL))) . NIL))))))
 */
void Parser::printExpression_test4() {
    Toolkit::info("Parser::printExpression() TEST FOUR");
    Toolkit::info("(NIL 5 () (()) 7 (() 9 ()) )");

    ExpressionTreeNode *nodeNIL = new ExpressionTreeNode();
    ExpressionTreeNode *node5 = new ExpressionTreeNode();
    ExpressionTreeNode *node7 = new ExpressionTreeNode();
    ExpressionTreeNode *node9 = new ExpressionTreeNode();

    node5->atom = Token(5);
    node7->atom = Token(7);
    node9->atom = Token(9);

    ExpressionTreeNode *P_NIL_NIL = new ExpressionTreeNode();
    P_NIL_NIL->leftChild = nodeNIL;
    P_NIL_NIL->rightChild = nodeNIL;

    ExpressionTreeNode *P_9_PNILNIL = new ExpressionTreeNode();
    P_9_PNILNIL->leftChild = node9;
    P_9_PNILNIL->rightChild = P_NIL_NIL;

    ExpressionTreeNode *P_NIL_P9NILNIL = new ExpressionTreeNode();
    P_NIL_P9NILNIL->leftChild = nodeNIL;
    P_NIL_P9NILNIL->rightChild = P_9_PNILNIL;

    ExpressionTreeNode *P_PNIL9NILNIL_NIL = new ExpressionTreeNode();
    P_PNIL9NILNIL_NIL->leftChild = P_NIL_P9NILNIL;
    P_PNIL9NILNIL_NIL->rightChild = nodeNIL;

    ExpressionTreeNode *P_7_PNIL9NILNILNIL = new ExpressionTreeNode();
    P_7_PNIL9NILNILNIL->leftChild = node7;
    P_7_PNIL9NILNILNIL->rightChild = P_PNIL9NILNIL_NIL;

    ExpressionTreeNode *P_PNILNIL_P7NIL9NILNILNIL = new ExpressionTreeNode();
    P_PNILNIL_P7NIL9NILNILNIL->leftChild = P_NIL_NIL;
    P_PNILNIL_P7NIL9NILNILNIL->rightChild = P_7_PNIL9NILNILNIL;

    ExpressionTreeNode *P_NIL_PNILNILP7NIL_ETC = new ExpressionTreeNode();
    P_NIL_PNILNILP7NIL_ETC->leftChild = nodeNIL;
    P_NIL_PNILNILP7NIL_ETC->rightChild = P_PNILNIL_P7NIL9NILNILNIL;

    ExpressionTreeNode *P_5_ETC = new ExpressionTreeNode();
    P_5_ETC->leftChild = node5;
    P_5_ETC->rightChild = P_NIL_PNILNILP7NIL_ETC;

    ExpressionTreeNode *root = new ExpressionTreeNode();
    root->leftChild = nodeNIL;
    root->rightChild = P_5_ETC;

    std::string result = printExpression(root, IS_NOT_ATOM);
    std::string expected = "(NIL . (5 . (NIL . ((NIL . NIL) . (7 . ((NIL . (9 . (NIL . NIL))) . NIL))))))";

    Toolkit::debug(result);
    std::cout << std::endl;
    if (result.compare(expected) == 0) {
        Toolkit::success("SUCCESS");
    } else {
        Toolkit::error("FAILURE");
        Toolkit::errorDetail("Expected: " + expected);
    }
}

/**
 * TEST WITH INPUT: (DEFUN F23 (X) (PLUS X 12 15))
 * EXPECTED RESULT: (DEFUN . (F23 . ((X . NIL) . ((PLUS . (X . (12 . (55 . NIL)))) . NIL))))
 */
void Parser::printExpression_test5() {
    Toolkit::info("Parser::printExpression() TEST FIVE");
    Toolkit::info("(DEFUN F23 (X) (PLUS X 12 15))");

    ExpressionTreeNode *nodeNIL = new ExpressionTreeNode();
    ExpressionTreeNode *nodeDEFUN = new ExpressionTreeNode();
    ExpressionTreeNode *nodeF23 = new ExpressionTreeNode();
    ExpressionTreeNode *nodeX = new ExpressionTreeNode();
    ExpressionTreeNode *nodePLUS = new ExpressionTreeNode();
    ExpressionTreeNode *node12 = new ExpressionTreeNode();
    ExpressionTreeNode *node55 = new ExpressionTreeNode();

    nodeDEFUN->atom = Token("DEFUN");
    nodeF23->atom = Token("F23");
    nodeX->atom = Token("X");
    nodePLUS->atom = Token("PLUS");
    node12->atom = Token(12);
    node55->atom = Token(55);

    ExpressionTreeNode *P_55_NIL = new ExpressionTreeNode();
    P_55_NIL->leftChild = node55;
    P_55_NIL->rightChild = nodeNIL;

    ExpressionTreeNode *P_12_P55NIL = new ExpressionTreeNode();
    P_12_P55NIL->leftChild = node12;
    P_12_P55NIL->rightChild = P_55_NIL;

    ExpressionTreeNode *P_X_P1255NIL = new ExpressionTreeNode();
    P_X_P1255NIL->leftChild = nodeX;
    P_X_P1255NIL->rightChild = P_12_P55NIL;

    ExpressionTreeNode *P_PLUS_FNBODY = new ExpressionTreeNode();
    P_PLUS_FNBODY->leftChild = nodePLUS;
    P_PLUS_FNBODY->rightChild = P_X_P1255NIL;

    ExpressionTreeNode *P_FN_NIL = new ExpressionTreeNode();
    P_FN_NIL->leftChild = P_PLUS_FNBODY;
    P_FN_NIL->rightChild = nodeNIL;

    ExpressionTreeNode *P_X_NIL = new ExpressionTreeNode();
    P_X_NIL->leftChild = nodeX;
    P_X_NIL->rightChild = nodeNIL;

    ExpressionTreeNode *P_XNIL_FN = new ExpressionTreeNode();
    P_XNIL_FN->leftChild = P_X_NIL;
    P_XNIL_FN->rightChild = P_FN_NIL;

    ExpressionTreeNode *FNAME_FBODY = new ExpressionTreeNode();
    FNAME_FBODY->leftChild = nodeF23;
    FNAME_FBODY->rightChild = P_XNIL_FN;

    ExpressionTreeNode *root = new ExpressionTreeNode();
    root->leftChild = nodeDEFUN;
    root->rightChild = FNAME_FBODY;

    std::string result = printExpression(root, IS_NOT_ATOM);
    std::string expected = "(DEFUN . (F23 . ((X . NIL) . ((PLUS . (X . (12 . (55 . NIL)))) . NIL))))";

    Toolkit::debug(result);
    std::cout << std::endl;
    if (result.compare(expected) == 0) {
        Toolkit::success("SUCCESS");
    } else {
        Toolkit::error("FAILURE");
        Toolkit::errorDetail("Expected: " + expected);
    }
}
