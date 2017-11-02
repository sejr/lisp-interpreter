#ifndef PRINTERS_H
#define PRINTERS_H

void printlist(struct TreeNode* leaf) {
    cout<<"(";

    struct TreeNode* temp = leaf;

    while (temp->left != NULL) {
        if (temp->left->left != NULL) printlist(temp->left);
        else cout << temp->left->key;

        if(temp->right->key != "NIL") cout << " ";
        temp = temp->right;
    }
    
    if (temp->key != "NIL"){
        cout << ". " << temp->key << ")";
    } else {
        cout << ")";
    }
}

void treetolist(struct TreeNode* leaf) {
    if (leaf->right == NULL){
        cout << leaf->key;
    } else {
        printlist(leaf);
    }

    cout << endl;
}

#endif