#include <sstream>
#include <fstream>
#include <vector>

#include "Evaluator.h"
#include "Toolkit.h"
#include "Parser.h"
#include "Node.h"

using namespace std;

Parser p;
stringstream ss;
vector<node> tree;
struct TreeNode *root = 0;
int focus, treeindex;
ofstream myfile;

void handleExpression();
void handleList();

void fillNode(string key) { tree[focus].key = key; }
void focusRight() { focus = tree[tree[focus].back].right; }
void focusBack() { focus = tree[focus].back; }

node anode(
    int index, string key, int back, int left, int right) {
    node temp;
    temp.back = back;
    temp.index = index;
    temp.key = key;
    temp.left = left;
    temp.right = right;
    return temp;
}

// Initializes an empty parse tree.
void initializeTree() {
    tree.clear();
    focus = 0;
    treeindex = 0;
    tree.push_back(anode(0, "", -1, -1, -1));
}

// Creates a new node.
void initializeNode() {
    tree[focus].left = ++treeindex;
    tree.push_back(anode(treeindex, "", focus, -1, -1));
    tree[focus].right = ++treeindex;
    tree.push_back(anode(treeindex, "", focus, -1, -1));
    focus = tree[focus].left;
}

// Utility for checking for errors; if we have an error present in one of our
// binary parse tree nodes, we print it and exit.
void Check() {
    if (p.Current.Type == "ERROR" || !p.error.empty()) {
        Toolkit::console(Toolkit::ERROR, p.error, true);
        exit(0);
    } else if (p.Current.Type == "EOF") {
        Toolkit::console(Toolkit::ERROR, "Reached EOF.", true);
        exit(0);
    }
}

struct TreeNode* insert(struct TreeNode* leaf, string data) {
    if (leaf == NULL) {
        return (NewNode(data));
    }
    else {
        if (data <= leaf->key) leaf->left = insert(leaf->left, data);
        else leaf->right = insert(leaf->right, data);
        return (leaf);
    }
}

void treeToTree(struct TreeNode* leaf, int index) {
    if (tree[index].left != -1) {
        leaf->left = NewNode(tree[tree[index].left].key);
        treeToTree(leaf->left, tree[tree[index].left].index);
    }
    if (tree[index].right != -1) {
        leaf->right = NewNode(tree[tree[index].right].key);
        treeToTree(leaf->right, tree[tree[index].right].index);
    }
}

// Pretty prints a Lisp parse tree, as specified in class.
void printList(struct TreeNode* leaf) {
    cout << "(";
    struct TreeNode* temp = leaf;
    while (temp->left != NULL) {
        if (temp->left->left != NULL) printList(temp->left);
        else cout << temp->left->key;
        if (temp->right->key != "NIL") cout << " ";
        temp = temp->right;
    }
    if (temp->key != "NIL") {
        cout << ". " << temp->key << ")";
    } else {
        cout << ")";
    }
}

// Lists out the keys in a particular binary parse tree.
void treeToList(struct TreeNode* leaf) {
    if (leaf->right == NULL) {
        cout << leaf->key;
    } else {
        printList(leaf);
    }
    cout << endl;
}

// Utilizes the Parsing utility to fetch tokens from stdin and evaluate the
// type checking mechanism.
void Start() {
    Check();
    while (p.Current.Type != "EOF") {
        initializeTree();
        handleExpression();
        root = NewNode(tree[0].key);
        treeToTree(root, 0);
        struct TreeNode *dummy = eval(root);
        free(dummy);
        treeToList(root);
    }
}

// Helper function for evaluating an expression, passes off to relevant functions
// to process lists and atomp. Otherwise we have a syntax error that has to
// be presented to the user.
void handleExpression() {
    Check();
    if (p.Current.Type == "Atom") {
        fillNode(p.Current.Content);
        p.MoveToNext();
    }
    else if (p.Current.Type == "OpenParenthesis") {
        p.MoveToNext();
        if (p.Current.Type == "ClosingParenthesis") {
            fillNode("NIL");
        }
        while (p.Current.Type != "ClosingParenthesis") {
            handleList();
        }
        p.MoveToNext();
    }
    else
        p.error = "Syntax error: unexpected \"" + p.Current.Content + "\"";
}

// Helper function for evaluating a list.
void handleList() {
    Check();
    int x = 0;
    while (p.Current.Type == "Atom" || p.Current.Type == "OpenParenthesis") {
        x++;
        initializeNode();
        handleExpression();
        focusRight();
    }
    fillNode("NIL");
    while (x != 0) {
        x--;
        focusBack();
    }
}

int main() {
    p.Init();
    Start();
    return 0;
}