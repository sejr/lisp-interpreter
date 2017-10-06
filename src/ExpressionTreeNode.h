#ifndef EXPRESSION_TREE_NODE_H
#define EXPRESSION_TREE_NODE_H

struct ExpressionTreeNode {
    Token atom;
    ExpressionTreeNode *leftChild;
    ExpressionTreeNode *rightChild;

    operator bool() const {
        return (bool) atom;
    }
};

#endif