#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <vector>
#include <string>
#include "Token.h"
#include "TokenProfile.h"

// The LexicalAnalyzer does most of the heavy handling of the input file.
// It allows us to generate sequences of Tokens, and also (for the time being)
// has a way to generate a TokenProfile from a single pass through said input.
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