#ifndef NODE_H
#define NODE_H

using namespace std;

struct Node {
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
};

#endif