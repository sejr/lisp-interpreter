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
    int position;
    Token currentToken;

    static bool isAlpha(char c);
    static bool isNumeric(char c);
    static bool isWhitespace(char c);
public:
    std::vector<Token> tokens;
    LexicalAnalyzer(std::vector<char> &buffer, unsigned int position);
    Token getTokenAtPosition(std::vector<char> &buffer, unsigned int &position);
    Token getCurrentToken();
    void moveToNextToken();
};

#endif
