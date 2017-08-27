#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include "Token.h"

class LexicalAnalyzer {
public:
    Token getNextToken(); 
};

Token LexicalAnalyzer::getNextToken() {
    Token testToken;
    return testToken;
}

#endif