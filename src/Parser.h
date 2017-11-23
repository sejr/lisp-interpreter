#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <sstream>
#include <string>

#include "Token.h"

using namespace std;

struct Parser {

    int Index;
    Token Current;
    string InputLine, error;

    void Init();
    void MoveToNext();
    Token getNextToken();
    void print(stringstream & ss);
};

#endif