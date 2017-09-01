#include "TokenProfile.h"

TokenProfile::TokenProfile() {
    openParenCount = 0;
    closeParenCount = 0;
}

TokenProfile::TokenProfile(std::vector<Token> tokens) {
    openParenCount = 0;
    closeParenCount = 0;
    containsError = false;
    for (unsigned int i = 0; i < tokens.size(); i++) {
        Token t = tokens.at(i);
        orderedTokens.push_back(t);
        if (t.getTokenType() == TokenType::atomLiteral) {
            literalAtoms.push_back(t);
        } else if (t.getTokenType() == TokenType::atomNumeric) {
            numericAtoms.push_back(t);
        } else if (t.getTokenType() == TokenType::parenOpen) {
            openParenCount++;
        } else if (t.getTokenType() == TokenType::parenClose) {
            closeParenCount++;
        } else if (t.getTokenType() == TokenType::error) {
            // std::cout << "Error on token: " << t.repr() << std::endl << std::endl;
            containsError = true;
        }
    }
}

void TokenProfile::displayTokenInfo() {
    if (containsError) {
        std::cout << "ERROR: Invalid token ";
        for (unsigned int i = 0; i < orderedTokens.size(); i++) {
            if (orderedTokens.at(i).getTokenType() == error) {
                std::cout << orderedTokens.at(i).repr() << " ";
            }
        }
    } else {
        std::cout << "LITERAL ATOMS: " << literalAtoms.size() << ", ";
        for (unsigned int i = 0; i < literalAtoms.size() - 1; i++) {
            std::cout << literalAtoms.at(i).repr() << ", ";
        }
        std::cout << literalAtoms.at(literalAtoms.size() - 1).repr();
        std::cout << std::endl;
    
        int sum = 0;
        std::cout << "NUMERIC ATOMS: " << numericAtoms.size() << ", ";
        for (unsigned int i = 0; i < numericAtoms.size(); i++) {
            sum += std::stoi(numericAtoms.at(i).repr());
        }
        std::cout << std::to_string((long long int) /* for stdlinux compatibility? */ sum) << std::endl;
        
        std::cout << "OPEN PARENTHESES: " << openParenCount << std::endl;
        std::cout << "CLOSING PARENTHESES: " << closeParenCount << std::endl;
    }
}
