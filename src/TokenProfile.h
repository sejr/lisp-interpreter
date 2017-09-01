#ifndef TOKEN_PROFILE_H
#define TOKEN_PROFILE_H

#include <vector>
#include <string>
#include <iostream>
#include "Token.h"

// The TokenProfile provides a bit of meta data regarding
// tokens generated from a particular input file. This is primarily
// used at this point to print out human-readable information for the
// sake of grading this assignment, but could be used later down the
// line to provide similar info to the user through an API.
class TokenProfile {
public:
    TokenProfile();
    TokenProfile(std::vector<Token> tokens);
    void displayTokenInfo();

    bool containsError;
    unsigned int openParenCount;
    unsigned int closeParenCount;
    std::vector<Token> literalAtoms;
    std::vector<Token> numericAtoms;
    std::vector<Token> orderedTokens;
};

#endif