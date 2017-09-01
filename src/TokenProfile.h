#ifndef TOKEN_PROFILE_H
#define TOKEN_PROFILE_H

#include <vector>
#include <string>
#include <iostream>
#include "Token.h"

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