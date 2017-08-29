#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <vector>
#include "Token.h"

class LexicalAnalyzer {
public:
    void parseFile(std::vector<char>& buffer);
    Token getNextToken();
};

void LexicalAnalyzer::parseFile(std::vector<char>& buffer) {
    int position = 0;

    int openParenCount = 0;
    int closeParenCount = 0;

    while (position < buffer.size()) {
        char currentChar = buffer.at(position);

        switch (currentChar) {
            case '(':
                openParenCount++;
                position++;
                break;
            case ')':
                closeParenCount++;
                position++;
                break;
            default:
                position++;
                break;
        }
    }

    std::cout << "LITERAL ATOMS: " << std::endl;
    std::cout << "NUMERIC ATOMS: " << std::endl;
    std::cout << "OPEN PARENTHESES: " << openParenCount << std::endl;
    std::cout << "CLOSING PARENTHESES: " << closeParenCount << std::endl;
}

Token LexicalAnalyzer::getNextToken() {
    Token testToken = closingParenthesis;
    return testToken;
}

#endif