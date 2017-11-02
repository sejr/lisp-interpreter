#ifndef EVALUATOR_H
#define EVALUATOR_H

// New eval(), as defined in project specification.
struct TreeNode* eval(
    struct TreeNode* leaf, 
    struct TreeNode* alist, 
    struct TreeNode* dlist);

// Converts string containing integer to actual integer.
int to_int(string in){
    int out;
    istringstream(in) >> out;
    return out;
} 

// Converts integer to string representation.
string to_str(int in){
    string s;
    stringstream out;
    out << in;
    s = out.str();
    return s;
}

// Verifies that input string contains a valid number.
inline bool isNumber(const std::string & s) {
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) {
        return false;
    }

    char * p;
    strtol(s.c_str(), &p, 10);
    return (*p == 0);
}

Node anode(int index, string key, int back, int left, int right) {
    Node temp;
    temp.back = back;
    temp.index = index;
    temp.key = key;
    temp.left = left;
    temp.right = right;
    return temp;
}

struct TreeNode* NewNode(string data) { 
  struct TreeNode* leaf = new(struct TreeNode);
  leaf->key = data; 
  leaf->left = NULL; 
  leaf->right = NULL;

  return(leaf); 
} 

struct TreeNode *dlist = NewNode("NIL");
struct TreeNode *alist = NewNode("NIL");

struct TreeNode* insert(struct TreeNode* leaf, string data) { 
    if (leaf == NULL) { 
        return(NewNode(data)); 
    } 
    else { 
        if (data <= leaf->key) leaf->left = insert(leaf->left, data); 
        else leaf->right = insert(leaf->right, data);
        return(leaf);
    } 
} 

bool isLetter(struct TreeNode *leaf) {
    if (leaf->key[0] >= 65 && leaf->key[0] <= 90) return true;
    return false;
}

bool isAtom(struct TreeNode *leaf) {
    if (leaf->left == NULL) return true;
    return false;
}

bool isList(struct TreeNode *leaf) {
    if (leaf->right==NULL) {
        if (leaf->key!="NIL") return false;
        else return true;
    } else return isList(leaf->right);
}

bool siList(struct TreeNode *leaf) {
    if (!isList(leaf->left)) return false;
    if (leaf->right->key=="NIL") return true;
    return siList(leaf->right);
}

// Returns the length of a list.
int len(struct TreeNode *leaf){
    if (!isList(leaf)) {
        cout << "ERROR: Length is undefined if the input is not a list." << endl;
        exit(0);
    }
    if (leaf->key == "NIL") return 0;
    return 1 + len(leaf->right);
}

bool siLength(struct TreeNode *leaf){
    if (len(leaf->left)!=2) return false;
    if (leaf->right->key=="NIL") return true;
    return siLength(leaf->right);
}

// Evaluates a condition; implementation as described in class.
struct TreeNode* evcon(struct TreeNode* leaf, struct TreeNode* alist, struct TreeNode* dlist){
    if(eval(leaf->left->left, alist, dlist)->key!="NIL") return eval(leaf->left->right->left, alist, dlist);
    if(leaf->right->key=="NIL"){
            cout << "ERROR: Undefined COND statement." << endl;
            exit(0);
    }
    return evcon(leaf->right, alist, dlist);
}

// Implementation as described in class.
bool bound(string key, struct TreeNode* alist){
    if (alist->key == "NIL") return false;
    else if (alist->left->left->key == key) return true;
    return bound(key, alist->right);
    cout << "ERROR: Function \"bound\" error." << endl;
    exit(0);
}

// Implementation as described in class.
struct TreeNode* getval(string key, struct TreeNode* list){
    if(list->key == "NIL"){
        cout << "ERROR: Function \"getval\" can't find " << key << endl;
        exit(0);
    }
    if (list->left->left->key == key) return list->left->right;
    return getval(key, list->right);
    cout << "ERROR: Function \"getval\" error while getting " << key << endl;
    exit(0);
}

// Implementation as described in class.
struct TreeNode* evlist(struct TreeNode* leaf, struct TreeNode* alist, struct TreeNode* dlist){
    if (leaf->key == "NIL") return NewNode("NIL");
    else {
        struct TreeNode *temp = NewNode("");
        temp->left = eval(leaf->left, alist, dlist);
        temp->right = evlist(leaf->right, alist, dlist);
        return temp;
    }
}

// Implementation as described in class.
struct TreeNode* addpairs(struct TreeNode* xlist, struct TreeNode* ylist, struct TreeNode* z) {
    if(xlist->key == "NIL") return z;
    else{
        struct TreeNode *temp = NewNode("");
        struct TreeNode *temp1 = NewNode("");
        temp->left = xlist->left;
        temp->right = ylist->left;
        temp1->left = temp;
        temp1->right = addpairs(xlist->right, ylist->right, z);
        return temp1;
    }
    cout << "ERROR: Function \"addpairs\" error." << endl;
    exit(0);
}

// Checks for our reserved words to prevent naming conflicts.
bool isReservedName(string name) {
    if( name == "T"     || name == "NIL"     || name == "CAR"   ||
        name == "CDR"   || name == "CONS"    || name == "ATOM"  ||
        name == "EQ"    || name == "NULL"    || name == "INT"   ||
        name == "PLUS"  || name == "MINUS"   || name == "TIMES" ||
        name == "LESS"  || name == "GREATER" || name == "COND"  ||
        name == "QUOTE" || name == "DEFUN" ) return true;

    else return false;
}

// Helper for verifying the parameter list of a user-defined function.
bool listIsLiteralAtoms(struct TreeNode *leaf){
    if(leaf->key=="NIL") return true;
    if(isLetter(leaf->left)) return listIsLiteralAtoms(leaf->right);
    else return false;
}

// Helper for verifying that list doesn't contain reserved names.
bool checkListForReservedNames(struct TreeNode *leaf){
    if(leaf->key=="NIL") return false;
    if(!isReservedName(leaf->left->key)) return checkListForReservedNames(leaf->right);
    else return true;
}

// Adds a new function definition to the dlist.
void dlistAdd(struct TreeNode *dlist, struct TreeNode *leaf){
    if(dlist->right!=NULL) {
        dlistAdd(dlist->right, leaf);
        return;
    }

    struct TreeNode *temp = NewNode("");
    struct TreeNode *temp1 = NewNode("");
    temp->left = leaf->right->left;
    temp->right = temp1;
    temp1->left = leaf->right->right->left;
    temp1->right = leaf->right->right->right->left;
    dlist->key.clear();
    dlist->left = temp;
    dlist->right = NewNode("NIL");
}

bool existInDlist(string name, struct TreeNode *dlist){
    if(dlist->key == "NIL") return false;
    if(name == dlist->left->left->key) return true;
    else return existInDlist(name, dlist->right);
}

int dlistLength(string name, struct TreeNode *dlist){
    if(name == dlist->left->left->key) return len(dlist->left->right->left);
    else return dlistLength(name, dlist->right);
}

struct TreeNode* apply(
    string f, 
    struct TreeNode* x, 
    struct TreeNode* alist, 
    struct TreeNode* dlist) {

    return eval(
        getval(f,dlist)->right, 
        addpairs(getval(f,dlist)->left,x,alist), 
        dlist);
}

struct TreeNode* eval(struct TreeNode* leaf, struct TreeNode* alist, struct TreeNode* dlist) {

    // If we are dealing with an atom, verify it is valid then print its value.
    if(!leaf->key.empty()){
        if((leaf->key=="T") || (leaf->key=="NIL") || (isNumber(leaf->key))) return leaf;
        else if (bound(leaf->key,alist)) return getval(leaf->key,alist);
        else{
            cout << "ERROR: Undefined atom: " << leaf->key <<endl;
            exit(0);
        }
    }

    if (leaf->left->key=="CAR"){
        if (len(leaf)!=2)
        {
            cout << "ERROR: CAR undefined, expected len = 2" << endl;
            exit(0);
        }
        struct TreeNode *temp = 0;
        temp = eval(leaf->right->left, alist, dlist);
        if(isAtom(temp)){
            cout << "ERROR: CAR undefined (eval(s1) is an atom)" << endl;
            exit(0);
        }
        return temp->left;
    }

    // Validate and execute the Lisp CDR function; return tail of list.
    if (leaf->left->key=="CDR"){
        if (len(leaf)!=2)
        {
            cout << "ERROR: CDR undefined, expected len = 2" << endl;
            exit(0);
        }
        struct TreeNode *temp = 0;
        temp = eval(leaf->right->left, alist, dlist);
        if(isAtom(temp)){
            cout << "ERROR: CDR undefined (eval(s1) is an atom)" << endl;
            exit(0);
        }
        return temp->right;
    }

    // Validate and execute the Lisp CONS function; return new list.
    if (leaf->left->key=="CONS"){
        if (len(leaf)!=3)
        {
            cout << "ERROR: CONS undefined (len(leaf)!=3)" << endl;
            exit(0);
        }
        struct TreeNode *temp = NewNode("");
        temp->left = eval(leaf->right->left, alist, dlist);
        temp->right = eval(leaf->right->right->left, alist, dlist);
        return temp;
    }

    // Validate and execute the Lisp ATOM function; T if atom, NIL otherwise.
    if (leaf->left->key=="ATOM"){
        if (len(leaf)!=2)
        {
            cout << "ERROR: ATOM undefined (len(leaf)!=2)" << endl;
            exit(0);
        }
        if(isAtom(eval(leaf->right->left, alist, dlist)))
            return NewNode("T");
        else
            return NewNode("NIL");
    }

    // Validate and execute the Lisp INT function; T if integer; NIL otherwise.
    if (leaf->left->key=="INT"){
        if (len(leaf)!=2)
        {
            cout << "ERROR: INT undefined (len(leaf)!=2)" << endl;
            exit(0);
        }
        if(isNumber((eval(leaf->right->left, alist, dlist)->key)))
            return NewNode("T");
        else
            return NewNode("NIL");
    }

    // Validate and execute the Lisp NULL function; T if NIL, NIL otherwise.
    if (leaf->left->key=="NULL"){
        if (len(leaf)!=2)
        {
            cout << "ERROR: NULL undefined (len(leaf)!=2)" << endl;
            exit(0);
        }
        if ((eval(leaf->right->left, alist, dlist))->key == "NIL")
            return NewNode("T");
        else
            return NewNode("NIL");
    }

    // Validate and execute the Lisp EQ function; T if equal, NIL otherwise.
    if (leaf->left->key=="EQ"){
        if (len(leaf)!=3)
        {
            cout << "ERROR: EQ undefined (len(leaf)!=3)" << endl;
            exit(0);
        }else if(!isAtom(eval(leaf->right->left, alist, dlist))){
            cout << "ERROR: eval(s1) is something other than an atom" << endl;
            exit(0);
        }else if(!isAtom(eval(leaf->right->right->left, alist, dlist))){
            cout << "ERROR: eval(s2) is something other than an atom" << endl;
            exit(0);
        }
        if((eval(leaf->right->left, alist, dlist))->key == (eval(leaf->right->right->left, alist, dlist))->key)
            return NewNode("T");
        else
            return NewNode("NIL");
    }

    // Validate and execute the Lisp PLUS function.
    // Returns sum as numeric atom.
    if (leaf->left->key=="PLUS"){
        if (len(leaf)!=3)
        {
            cout << "ERROR: PLUS undefined (len(leaf)!=3)" << endl;
            exit(0);
        }else if(!isNumber((eval(leaf->right->left, alist, dlist))->key)){
            cout << "ERROR: PLUS - eval(s1) is something other than an numeric atom " << (eval(leaf->right->left, alist, dlist))->key  << endl;
            exit(0);
        }else if(!isNumber( (eval(leaf->right->right->left, alist, dlist))->key )){
            cout << "ERROR: PLUS - eval(s2) is something other than an numeric atom " << (eval(leaf->right->right->left, alist, dlist))->key << endl;
            exit(0);
        }
        return NewNode(to_str( to_int((eval(leaf->right->left, alist, dlist))->key)+to_int((eval(leaf->right->right->left, alist, dlist))->key) ));
    }

    // Validate and execute the Lisp MINUS function.
    // Returns difference as numeric atom.
    if (leaf->left->key=="MINUS"){
        if (len(leaf)!=3)
        {
            cout << "ERROR: MINUS undefined (len(leaf)!=3)" << endl;
            exit(0);
        }else if(!isNumber((eval(leaf->right->left, alist, dlist))->key)){
            cout << "ERROR: MINUS - eval(s1) is something other than an numeric atom " << (eval(leaf->right->left, alist, dlist))->key << endl;
            exit(0);
        }else if(!isNumber((eval(leaf->right->right->left, alist, dlist))->key)){
            cout << "ERROR: MINUS - eval(s2) is something other than an numeric atom " << (eval(leaf->right->right->left, alist, dlist))->key  << endl;
            exit(0);
        }
        return NewNode(to_str( to_int((eval(leaf->right->left, alist, dlist))->key)-to_int((eval(leaf->right->right->left, alist, dlist))->key) ));
    }

    // Validate and execute the Lisp TIMES function.
    // Returns product as numeric atom.
    if (leaf->left->key=="TIMES"){
        if (len(leaf)!=3)
        {
            cout << "ERROR: TIMES undefined (len(leaf)!=3)" << endl;
            exit(0);
        }else if(!isNumber((eval(leaf->right->left, alist, dlist))->key)){
            cout << "ERROR: TIMES - eval(s1) is something other than an numeric atom " << (eval(leaf->right->left, alist, dlist))->key << endl;
            exit(0);
        }else if(!isNumber((eval(leaf->right->right->left, alist, dlist))->key)){
            cout << "ERROR: TIMES - eval(s2) is something other than an numeric atom " << (eval(leaf->right->right->left, alist, dlist))->key  << endl;
            exit(0);
        }
        return NewNode(to_str( to_int((eval(leaf->right->left, alist, dlist))->key)*to_int((eval(leaf->right->right->left, alist, dlist))->key) ));
    }

    // Validate and execute the Lisp LESS function.
    // T if a < b, NIL otherwise.
    if (leaf->left->key=="LESS"){
        if (len(leaf)!=3)
        {
            cout << "ERROR: LESS undefined (len(leaf)!=3)" << endl;
            exit(0);
        }else if(!isNumber((eval(leaf->right->left, alist, dlist))->key)){
            cout << "ERROR: LESS - eval(s1) is something other than an numeric atom " << (eval(leaf->right->left, alist, dlist))->key << endl;
            exit(0);
        }else if(!isNumber((eval(leaf->right->right->left, alist, dlist))->key)){
            cout << "ERROR: LESS - eval(s2) is something other than an numeric atom " << (eval(leaf->right->right->left, alist, dlist))->key  << endl;
            exit(0);
        }
        if(to_int((eval(leaf->right->left, alist, dlist))->key)<to_int((eval(leaf->right->right->left, alist, dlist))->key))
            return NewNode("T");
        else
            return NewNode("NIL");
    }

    // Validate and execute the Lisp GREATER function.
    // T if a > b, NIL otherwise.
    if (leaf->left->key=="GREATER"){
        if (len(leaf)!=3)
        {
            cout << "ERROR: GREATER undefined (len(leaf)!=3)" << endl;
            exit(0);
        }else if(!isNumber((eval(leaf->right->left, alist, dlist))->key)){
            cout << "ERROR: GREATER - eval(s1) is something other than an numeric atom " << (eval(leaf->right->left, alist, dlist))->key << endl;
            exit(0);
        }else if(!isNumber((eval(leaf->right->right->left, alist, dlist))->key)){
            cout << "ERROR: GREATER - eval(s2) is something other than an numeric atom " << (eval(leaf->right->right->left, alist, dlist))->key  << endl;
            exit(0);
        }
        if(to_int((eval(leaf->right->left, alist, dlist))->key)>to_int((eval(leaf->right->right->left, alist, dlist))->key))
            return NewNode("T");
        else
            return NewNode("NIL");
    }

    // Validate and execute the Lisp QUOTE function.
    // Returns unevaluated list tail.
    if (leaf->left->key=="QUOTE"){
        if (len(leaf)!=2)
        {
            cout << "ERROR: QUOTE length != 2" << endl;
            exit(0);
        }
        return leaf->right->left;
    }

    // Validate and execute the Lisp COND function.
    // More information about COND is given in the evcon() definition.
    if (leaf->left->key=="COND"){
        if (len(leaf)==1){
            cout << "ERROR: COND length can't be 1" << endl;
            exit(0);
        }else if(!siList(leaf->right)){
            cout << "ERROR: COND - some si is not a list" << endl;
            exit(0);
        }else if(!siLength(leaf->right)){
            cout << "ERROR: COND - some si is a list such that length(si) != 2" << endl;
            exit(0);
        }
        return evcon(leaf->right, alist, dlist);
    }

    // Validate and execute the Lisp DEFUN function.
    // Adds the validated user-defined function to the dlist, returns the function key.
    if (leaf->left->key == "DEFUN"){
        if (len(leaf) != 4){
            cout << "ERROR: DEFUN length must be 4, example (DEFUN F (X Y) BODY)" << endl;
            exit(0);
        } else if(!isLetter(leaf->right->left)) {
            cout << "ERROR: function name inside DEFUN is not literal atom." << endl;
            exit(0);
        } else if(isReservedName(leaf->right->left->key)) {
            cout << "ERROR: \"" << leaf->right->left->key << "\" inside DEFUN is a reserved name." << endl;
            exit(0);
        } else if(!isList(leaf->right->right->left)) {
            cout << "ERROR: S1 for DEFUN is not a list." << endl;
            exit(0);
        } else if(!listIsLiteralAtoms(leaf->right->right->left)) {
            cout << "ERROR: S1 for DEFUN can only contain literal atomp." << endl;
            exit(0);
        } else if (checkListForReservedNames(leaf->right->right->left)) {
            cout << "ERROR: S1 for DEFUN cannot have reserved namep." << endl;
            exit(0);
        }

        dlistAdd(dlist, leaf);
        return NewNode(leaf->right->left->key);
    }

    // Check the keyword amongst the existing user-defined functionp.
    // If the function exists, evaluate that function with the given argumentp.
    // If not, return an error.
    if (existInDlist(leaf->left->key, dlist)){
        if(dlistLength(leaf->left->key, dlist)!=len(leaf->right)){
            cout << "ERROR: Defined function \"" << leaf->left->key << "\" should have " << dlistLength(leaf->left->key, dlist) << " inputp." << endl;
            exit(0);
        }
        return apply(leaf->left->key, evlist(leaf->right,alist,dlist), alist, dlist);
    }

    cout << "ERROR: Undefined function in eval: " << leaf->left->key << endl;
    exit(0);
}

#endif