#include <vector>
#include <sstream>
#include <fstream>

#include "Node.h"
#include "Parser.h"
#include "Printers.h"
#include "Evaluator.h"

using namespace std;

Parser p;
stringstream ss;
vector<Node> tree;
ofstream outputFile;
int focus, treeindex;
struct TreeNode *root = 0;

void handleExpression();
void handleList();
void fillNode(string key) { tree[focus].key = key; }

void focusBack() { focus = tree[focus].back; }
void focusRight() { focus = tree[tree[focus].back].right; }

void initializeTree(){
	tree.clear();
	focus = 0;
	treeindex = 0;
	tree.push_back(anode(0,"",-1,-1,-1));
}

void newnode() {
	tree[focus].left = ++treeindex;
	tree.push_back(anode(treeindex,"",focus,-1,-1));
	tree[focus].right = ++treeindex;
	tree.push_back(anode(treeindex,"",focus,-1,-1));
	focus = tree[focus].left;
}

void treeToTree(struct TreeNode* leaf, int index){
    if(tree[index].left!=-1){
        leaf->left = NewNode(tree[tree[index].left].key);
        treeToTree(leaf->left, tree[tree[index].left].index);
    }
    if(tree[index].right!=-1){
        leaf->right = NewNode(tree[tree[index].right].key);
        treeToTree(leaf->right, tree[tree[index].right].index);
    }
}

// If we come across an error token, we need to exit out of the program's
// execution and provide some kind of error.
void catchError(){
	if(p.currentToken.type=="ERROR" || !p.error.empty()) {
		// returning token is ERROR or the string named error is written by something.
		cout << "ERROR: " << p.error << endl;
		exit(0);
	} else if (p.currentToken.type=="EOF"){
		// this happens when file ended but the grammar is not finished.
		cout << "ERROR: Unexpected end of file." << endl;
		exit(0);
	}
}

// Handle an expression.
void handleExpression() {
	catchError();
	if (p.currentToken.type == "Atom") {
		fillNode(p.currentToken.content);
		p.moveToNext();
	} else if(p.currentToken.type == "OpenParenthesis"){
		p.moveToNext();
		if(p.currentToken.type == "ClosingParenthesis"){
			fillNode("NIL");
		}
		while(p.currentToken.type != "ClosingParenthesis"){
			handleList();
		}
		p.moveToNext();
	} else {
		p.error = "Unexpected token: \"" + p.currentToken.content + "\"";
	}
}

// Handle list.
void handleList() {
	catchError();
	int x = 0;
	while(p.currentToken.type == "Atom" || p.currentToken.type == "OpenParenthesis"){
		x++;
		newnode();
		handleExpression();
		focusRight();
	}
	fillNode("NIL");
	while(x != 0){
		x--;
		focusBack();
	}
}

// Begin parsing and evaluation of input file.
int main(){
	p.initialize();
	catchError();
    while(p.currentToken.type != "EOF") {
        initializeTree();
        handleExpression();
        root = NewNode(tree[0].key);
        treeToTree(root, 0);
        root = eval(root, alist, dlist);
        treetolist(root);
    }
	return 0;
}