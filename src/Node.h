#ifndef NODE_H
#define NODE_H

#include <string>
using namespace std;

struct node {
    int index;
    string key;
    int back;
    int left;
    int right;
};

struct TreeNode {
    string key;
    struct TreeNode *left;
    struct TreeNode *right;
    string type;
    bool couldbeNIL;
};

#endif