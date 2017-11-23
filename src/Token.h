#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

struct Token {

    string Type, Content;

    Token(string a, string b) {
        Type = a;
        Content = b;
    }

    Token(string a) {
        Type = Content = a;
    }

    Token() {}
};

#endif