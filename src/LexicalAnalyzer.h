#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <vector>
#include <string>
#include "Token.h"
#include "TokenProfile.h"

class LexicalAnalyzer {
private:
    bool isAlpha(char c);
    bool isNumeric(char c);
    bool isWhitespace(char c);
public:
    TokenProfile tokenize(std::vector<char>& buffer);
    Token getNextToken(std::vector<char> &buffer, unsigned int &position);
};

#endif