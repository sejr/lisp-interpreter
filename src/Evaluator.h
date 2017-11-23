#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <sstream>
#include <fstream>
#include <vector>

#include "Toolkit.h"
#include "Parser.h"
#include "Node.h"

struct TreeNode* eval(struct TreeNode* leaf);

int to_int(string in) {
    int out;
    istringstream(in) >> out;
    return out;
}

string to_str(int in) {
    string s;
    stringstream out;
    out << in;
    s = out.str();
    return s;
}

inline bool isNumber(const std::string & s)
{
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

    char * p ;
    strtol(s.c_str(), &p, 10) ;

    return (*p == 0) ;
}

bool isLetter(struct TreeNode **leaf) {
    if ((*leaf)->key[0] >= 65 && (*leaf)->key[0] <= 90) return true;
    return false;
}

bool isAtom(struct TreeNode *leaf) {
    if (leaf->left == NULL)
        return true;
    return false;
}

bool isList(struct TreeNode *leaf) {
    if (leaf->right == NULL) {
        if (leaf->key != "NIL")return false;
        else return true;
    } else return isList(leaf->right);
}

bool NILcheck(struct TreeNode* leaf) {
    if (leaf->key == "NIL") return false;
    if (eval(leaf->left->right->left)->type == "List" && eval(leaf->left->right->left)->key == "NIL")
        return true;
    else
        return NILcheck(leaf->right);
}

bool numberCheck(struct TreeNode* leaf) {
    if (leaf->key == "NIL")
        return true;
    if (isNumber(leaf->left->key))
        return numberCheck(leaf->right);
    else
        return false;
}

bool isListofnumbers(struct TreeNode* leaf) {
    if (isList(leaf) && numberCheck(leaf)) {
        return true;
    }
    return false;
}

struct TreeNode* cond(struct TreeNode* leaf, string type) {
    if (leaf->key == "NIL") {
        Toolkit::console(
            Toolkit::ERROR,
            "Undefined COND expression; no default case.",
            true);
        exit(0);
    }
    if (eval(leaf->left->right->left)->type != type)
    {
        Toolkit::console(
            Toolkit::ERROR,
            "Type error: COND expression results should match.",
            true);
        exit(0);
    }
    if (eval(leaf->left->left)->key == "T") return eval(leaf->left->right->left); // good
    else if (eval(leaf->left->left)->key == "F") return cond(leaf->right, type);
    Toolkit::console(
        Toolkit::ERROR,
        "Undefined COND expression.",
        true);
    exit(0);
}

struct TreeNode* NewNode(string data) {
    struct TreeNode* leaf = new(struct TreeNode);
    leaf->key = data;
    leaf->left = NULL;
    leaf->right = NULL;

    return (leaf);
}

bool si_list(struct TreeNode *leaf) {
    if (!isList(leaf->left)) return false;
    if (leaf->right->key == "NIL") return true;
    return si_list(leaf->right);
}

int treelength(struct TreeNode *leaf) {
    if (!isList(leaf)) {
        Toolkit::console(
            Toolkit::ERROR,
            "Length undefined on atoms.",
            true);
        exit(0);
    }
    if (leaf->key == "NIL") return 0;
    return 1 + treelength(leaf->right);
}

bool si_length(struct TreeNode *leaf) {
    if (treelength(leaf->left) != 2) return false;
    if (leaf->right->key == "NIL") return true;
    return si_length(leaf->right);
}

struct TreeNode* eval(struct TreeNode* leaf) {
    if (!leaf->key.empty()) {
        if ((isNumber(leaf->key))) // number
        {
            struct TreeNode *temp = NewNode(leaf->key);
            temp->type = "Nat";
            return temp;
        }
        if ((leaf->key == "T") || (leaf->key == "F")) // T, F
        {
            struct TreeNode *temp = NewNode(leaf->key);
            temp->type = "Bool";
            return temp;
        }
        if ((leaf->key == "NIL")) // NIL
        {
            struct TreeNode *temp = NewNode(leaf->key);
            temp->type = "List";
            return temp;
        }
        Toolkit::console(
            Toolkit::ERROR,
            "Undefined atom: " + leaf->key,
            true);
        exit(0);
    }


    if (leaf->left->key == "CAR") {
        if (treelength(leaf) != 2)
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Invalid length for CAR expression.",
                true);
            exit(0);
        }

        struct TreeNode *temp = 0;
        temp = eval(leaf->right->left);

        if (temp->type == "List" && temp->key == "NIL")
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Type error: input to CAR cannot be empty list.",
                true);
            exit(0);
        }

        if (temp->type != "List")
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Type error: expected list in CAR expression.",
                true);
            exit(0);
        }

        if (temp->couldbeNIL == true)
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Static type error: list may be empty.",
                true);
            exit(0);
        }

        if (isAtom(temp)) {
            Toolkit::console(
                Toolkit::ERROR,
                "Type error: expected list in CAR expression.",
                true);
            exit(0);
        }
        temp->left->type = "Nat";
        return temp->left;
    }


    if (leaf->left->key == "CDR") {
        if (treelength(leaf) != 2)
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Invalid length for CDR expression.",
                true);
            exit(0);
        }
        struct TreeNode *temp = 0;
        temp = eval(leaf->right->left);
        if (temp->type == "List" && temp->key == "NIL")
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Type error: input to CDR cannot be empty list.",
                true);
            exit(0);
        }
        if (temp->type != "List")
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Type error: expected list in CDR expression.",
                true);
            exit(0);
        }
        if (temp->couldbeNIL == true)
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Static type error: list may be empty.",
                true);
            exit(0);
        }

        if (isAtom(temp)) {

            Toolkit::console(
                Toolkit::ERROR,
                "Type error: expected list in CDR expression.",
                true);
            exit(0);
        }
        temp->left->type = "List";
        return temp->right;
    }


    if (leaf->left->key == "CONS") {
        if (treelength(leaf) != 3)
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Invalid length for CONS expression.",
                true);
            exit(0);
        }

        struct TreeNode *temp = NewNode("");
        temp->left = eval(leaf->right->left);
        temp->right = eval(leaf->right->right->left);


        if (temp->left->type != "Nat")
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Type error: expected CONS first param to be a number.",
                true);
            exit(0);
        }

        if (temp->right->type != "List")
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Type error: expected CONS second param to be a list of numbers.",
                true);
            exit(0);
        }

        temp->type = "List";
        return temp;
    }


    if (leaf->left->key == "ATOM") {
        if (treelength(leaf) != 2)
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Invalid length for ATOM expression.",
                true);
            exit(0);
        }

        if (eval(leaf->right->left)->type == "Bool" || eval(leaf->right->left)->type == "Nat") {
            struct TreeNode *temp = NewNode("T");
            temp->type = "Bool";
            return temp;
        }
        else if (eval(leaf->right->left)->type == "List") {
            struct TreeNode *temp = NewNode("F");
            temp->type = "Bool";
            return temp;
        } else {
            Toolkit::console(
                Toolkit::ERROR,
                "Type error: ATOM undefined, expected one of: <Bool>, <Nat>, <List>.",
                true);
            exit(0);
        }
    }


    if (leaf->left->key == "INT") {
        if (treelength(leaf) != 2)
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Invalid length for INT expression.",
                true);
            exit(0);
        }

        if (eval(leaf->right->left)->type == "Nat") {
            struct TreeNode *temp = NewNode("T");
            temp->type = "Bool";
            return temp;
        }
        else if (eval(leaf->right->left)->type == "Bool" || eval(leaf->right->left)->type == "List") {
            struct TreeNode *temp = NewNode("F");
            temp->type = "Bool";
            return temp;
        } else {
            Toolkit::console(
                Toolkit::ERROR,
                "Type error: INT undefined, expected one of: <Bool>, <Nat>, <List>.",
                true);
            exit(0);
        }
    }


    if (leaf->left->key == "NULL") {
        if (treelength(leaf) != 2)
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Invalid length for NULL expression.",
                true);
            exit(0);
        }

        if (eval(leaf->right->left)->type == "List") {
            if (eval(leaf->right->left)->key == "NIL")
            {
                struct TreeNode *temp = NewNode("T");
                temp->type = "Bool";
                return temp;
            } else {
                struct TreeNode *temp = NewNode("F");
                temp->type = "Bool";
                return temp;
            }
        } else {
            Toolkit::console(
                Toolkit::ERROR,
                "Type error: NULL undefined, expected (NULL <List>).",
                true);
            exit(0);
        }
    }


    if (leaf->left->key == "EQ") {
        if (treelength(leaf) != 3)
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Invalid length for EQ expression.",
                true);
            exit(0);
        } else if (!isAtom(eval(leaf->right->left))) {
            Toolkit::console(
                Toolkit::ERROR,
                "Expected atom in EQ expression (first param).",
                true);
            exit(0);
        } else if (!isAtom(eval(leaf->right->right->left))) {
            Toolkit::console(
                Toolkit::ERROR,
                "Expected atom in EQ expression (second param).",
                true);
            exit(0);
        }

        if (eval(leaf->right->left)->type == "Nat" && eval(leaf->right->right->left)->type == "Nat") {
            if ((eval(leaf->right->left))->key == (eval(leaf->right->right->left))->key)
            {
                struct TreeNode *temp = NewNode("T");
                temp->type = "Bool";
                return temp;
            } else {
                struct TreeNode *temp = NewNode("F");
                temp->type = "Bool";
                return temp;
            }
        } else {
            Toolkit::console(
                Toolkit::ERROR,
                "Type error: EQ undefined, expected (EQ <Nat> <Nat>).",
                true);
            exit(0);
        }
    }


    if (leaf->left->key == "PLUS") {
        if (treelength(leaf) != 3)
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Invalid length for PLUS expression.",
                true);
            exit(0);
        }

        if (eval(leaf->right->left)->type == "Nat" && eval(leaf->right->right->left)->type == "Nat") {
            struct TreeNode *temp = NewNode(to_str( to_int((eval(leaf->right->left))->key) + to_int((eval(leaf->right->right->left))->key) ));
            temp->type = "Nat";
            return temp;
        } else {
            Toolkit::console(
                Toolkit::ERROR,
                "Type error: PLUS undefined, expected (PLUS <Nat> <Nat>).",
                true);
            exit(0);
        }
    }


    if (leaf->left->key == "LESS") {
        if (treelength(leaf) != 3)
        {
            Toolkit::console(
                Toolkit::ERROR,
                "Invalid length for LESS expression.",
                true);
            exit(0);
        }

        if (eval(leaf->right->left)->type == "Nat" && eval(leaf->right->right->left)->type == "Nat") {
            if (to_int((eval(leaf->right->left))->key) < to_int((eval(leaf->right->right->left))->key))
            {
                struct TreeNode *temp = NewNode("T");
                temp->type = "Bool";
                return temp;
            } else {
                struct TreeNode *temp = NewNode("F");
                temp->type = "Bool";
                return temp;
            }
        } else {
            Toolkit::console(
                Toolkit::ERROR,
                "Type error: LESS undefined, expected (LESS <Nat> <Nat>).",
                true);
            exit(0);
        }
    }


    if (leaf->left->key == "COND") {
        if (treelength(leaf) == 1) {
            Toolkit::console(
                Toolkit::ERROR,
                "Invalid length for COND expression.",
                true);
            exit(0);
        } else if (!si_list(leaf->right)) {
            Toolkit::console(
                Toolkit::ERROR,
                "Inside COND expression: some si is not a list.",
                true);
            exit(0);
        } else if (!si_length(leaf->right)) {
            Toolkit::console(
                Toolkit::ERROR,
                "Inside COND expression: some si with length != 2.",
                true);
            exit(0);
        }
        string type = eval(leaf->right->left->right->left)->type;
        struct TreeNode *temp = cond(leaf->right, type);
        temp->couldbeNIL = NILcheck(leaf->right);
        return temp;
    }

    Toolkit::console(Toolkit::ERROR, "Eval is undefined: " + leaf->left->key, true);
    exit(0);
}

#endif