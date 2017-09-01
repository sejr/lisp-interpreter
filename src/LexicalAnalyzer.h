#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <vector>
#include <string>
#include "Token.h"

class LexicalAnalyzer {
private:
    bool isAlpha(char c);
    bool isNumeric(char c);
    bool isWhitespace(char c);
    void printAtoms(std::vector<std::string>& atoms);
public:
    TokenProfile tokenize(std::vector<char>& buffer);
};

TokenProfile LexicalAnalyzer::tokenize(std::vector<char>& buffer) {
    unsigned int position = 0;
    TokenProfile profile;

    while (position < buffer.size()) {
        std::string temp; // for literal / numeric atoms
        char currentChar = buffer.at(position);

        // Check if we are starting a valid literal atom (begins with A-Z)
        if (isAlpha(currentChar)) {
            temp.push_back(currentChar);

            position++;
            currentChar = buffer.at(position);

            // Fetch the rest of the literal atom
            while (!isWhitespace(currentChar)) {
                if (isAlpha(currentChar) || isNumeric(currentChar)) {
                    temp.push_back(currentChar);
                    position++;
                    currentChar = buffer.at(position);
                } else {
                    break;
                }
            }

            profile.literalAtoms.push_back(Token(temp));
            profile.orderedTokens.push_back(Token(temp));
            temp.clear();

        } else if (isNumeric(currentChar)) {
            temp.push_back(currentChar);

            position++;
            currentChar = buffer.at(position);

            while (!isWhitespace(currentChar)) {
                if (isAlpha(currentChar)) {
                    // Error
                } else if (isNumeric(currentChar)) {
                    temp.push_back(currentChar);
                    position++;
                    currentChar = buffer.at(position);
                } else {
                    break;
                }
            }

            std::string::size_type sz;
            int numeric = std::stoi (temp, &sz);
            profile.numericAtoms.push_back(Token(numeric));
            profile.orderedTokens.push_back(Token(numeric));
            temp.clear();
        }

        // Here we can fetch singleton tokens.
        switch (currentChar) {
            case '(':
                profile.orderedTokens.push_back(Token('('));
                profile.openParenCount++;
                break;
            case ')':
                profile.orderedTokens.push_back(Token('('));
                profile.closeParenCount++;
                break;
            default:
                if (!isWhitespace(currentChar)) {
                    profile.orderedTokens.push_back(Token(currentChar));
                }
                break;
        }

        position++;
    }

    return profile;
}

void LexicalAnalyzer::printAtoms(std::vector<std::string>& atoms) {
    for (unsigned int i = 0; i < atoms.size() - 1; i++) {
        std::cout << atoms.at(i) << ", ";
    }
    std::cout << atoms.at(atoms.size() - 1) << std::endl;
}

bool LexicalAnalyzer::isAlpha(char c) {
    bool lowerBound = (unsigned int) c >= 65; // A
    bool upperBound = (unsigned int) c <= 90; // Z

    return lowerBound && upperBound;
}

bool LexicalAnalyzer::isNumeric(char c) {
    bool lowerBound = (unsigned int) c >= 48; // 0
    bool upperBound = (unsigned int) c <= 57; // 9

    return lowerBound && upperBound;
}

bool LexicalAnalyzer::isWhitespace(char c) {
    bool s = (unsigned int) c == 32; // space
    bool r = (unsigned int) c == 13; // return
    bool l = (unsigned int) c == 10; // line feed

    return s || r || l;
}

#endif