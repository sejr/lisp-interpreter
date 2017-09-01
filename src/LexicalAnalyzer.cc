#include "LexicalAnalyzer.h"

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
                    std::string err = "ERR";
                    profile.orderedTokens.push_back(Token(err));
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

Token LexicalAnalyzer::getNextToken(std::vector<char> &buffer, unsigned int &position) {
    if (position < buffer.size()) {
        std::string temp; // for literal / numeric atoms
        char currentChar = buffer.at(position);

        if (isWhitespace(currentChar)) {
            position++;
            return Token("whitespace");
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
                    return Token(temp);
                }
            }

            return Token(temp);

        } else if (isNumeric(currentChar)) {
            bool err = false;
            temp.push_back(currentChar);

            position++;
            currentChar = buffer.at(position);

            while (!isWhitespace(currentChar)) {
                if (isAlpha(currentChar)) {
                    err = true;
                    temp.push_back(currentChar);
                    position++;
                    currentChar = buffer.at(position);
                } else if (isNumeric(currentChar)) {
                    temp.push_back(currentChar);
                    position++;
                    currentChar = buffer.at(position);
                } else {
                    break;
                }
            }

            return Token(temp, err);
        }

        // Here we can fetch singleton tokens.
        switch (currentChar) {
            case '(':
                position++;
                return Token('(');
                break;
            case ')':
                position++;
                return Token(')');
                break;
            default:
                if (!isWhitespace(currentChar)) {
                    position++;
                    return Token(currentChar);
                } else {
                    position++;
                }
                break;
        }

    } else {
        return Token("eof");
    }
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
