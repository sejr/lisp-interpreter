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
    std::vector<Token> parseFile(std::vector<char>& buffer);
};

std::vector<Token> LexicalAnalyzer::parseFile(std::vector<char>& buffer) {
    int position = 0;

    int openParenCount = 0;
    int closeParenCount = 0;

    std::vector<std::string> literalAtoms;
    std::vector<std::string> numericAtoms;

    std::vector<Token> tokens;

    while (position < buffer.size()) {
        std::string temp; // for literal / numeric atoms
        char currentChar = buffer.at(position);

        // Here we can fetch singleton tokens.
        switch (currentChar) {
            case '(':
                tokens.push_back(Token('('));
                openParenCount++;
                break;
            case ')':
                tokens.push_back(Token('('));
                closeParenCount++;
                break;
            default:
                tokens.push_back(Token(currentChar));
                break;
        }

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

            tokens.push_back(Token(temp));
            literalAtoms.push_back(temp);
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
            tokens.push_back(Token(numeric));
            numericAtoms.push_back(temp);
            temp.clear();
        }

        position++;
    }

    std::cout << "LITERAL ATOMS: " << literalAtoms.size() << ", ";
    printAtoms(literalAtoms);

    int sum = 0;
    std::cout << "NUMERIC ATOMS: " << numericAtoms.size() << ", ";
    for (int i = 0; i < numericAtoms.size(); i++) {
        std::string::size_type sz;
        int numeric = std::stoi (numericAtoms.at(i), &sz);
        sum += numeric;
    }
    std::cout << std::to_string(sum) << std::endl;
    
    std::cout << "OPEN PARENTHESES: " << openParenCount << std::endl;
    std::cout << "CLOSING PARENTHESES: " << closeParenCount << std::endl;
}

void LexicalAnalyzer::printAtoms(std::vector<std::string>& atoms) {
    for (int i = 0; i < atoms.size() - 1; i++) {
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