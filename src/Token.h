#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

struct Token {
    string type, content;
    Token(string a, string b) {
        type = a;
        content = b;
    }
    Token(string a){
        type = content = a;
    }
    Token(){}
};

#endif