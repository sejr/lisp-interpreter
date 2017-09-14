#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer(
    std::vector<char> &buffer,
    unsigned int origin
) : position(origin), currentToken(getTokenAtPosition(buffer, origin)) {

    unsigned int p = 0;
    Token t = getTokenAtPosition(buffer, p);
    tokens.push_back(t);

    currentToken = getCurrentToken();

    while (
        t.getTokenType() != eof &&
        t.getTokenType() != error
    ) {
        t = getTokenAtPosition(buffer, p);
        if (t.getTokenType() != whitespace) {
            tokens.push_back(t);
        }
    }
}

void LexicalAnalyzer::moveToNextToken() {
  position += 1;
}

Token LexicalAnalyzer::getCurrentToken() {
  return tokens.at(position);
}

// This is the preferred way of generating token vectors, which uses a tracked position
// value to continue through a buffer, returning individual tokens one at a time.
Token LexicalAnalyzer::getTokenAtPosition(std::vector<char> &buffer, unsigned int &position) {
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

        // Check if we are starting a valid numeric atom (begins with 0-9)
        } else if (isNumeric(currentChar)) {
            bool err = false;
            temp.push_back(currentChar);

            position++;
            currentChar = buffer.at(position);

            while (!isWhitespace(currentChar)) {
                // Once we begin a numeric atom, we can't go back to alpha
                // characters; thus, we are in an error state and need to
                // abort. We still generate error tokens, however, to inform
                // the user of which token caused the error.
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
            case '(': {
                position++;
                char nextChar = buffer.at(position);
                while (isWhitespace(nextChar)) {
                    position++;
                    nextChar = buffer.at(position);
                }
                if (nextChar == ')') {
                    position++;
                    return Token("nil");
                } else {
                    return Token('(');
                }
                break;
            }
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
        // Forcing the EOF token to prevent an error;
        // the LexicalAnalyzer should never reach this point.
        return Token("eof");
    }

    exit(EXIT_FAILURE);
}

// Quick helper function to determine if a particular
// character is a valid alpha character (A-Z).
bool LexicalAnalyzer::isAlpha(char c) {
    bool lowerBound = (unsigned int) c >= 65; // A
    bool upperBound = (unsigned int) c <= 90; // Z

    return lowerBound && upperBound;
}

// Quick helper function to determine if a particular
// character is a valid numeric character (0-9).
bool LexicalAnalyzer::isNumeric(char c) {
    bool lowerBound = (unsigned int) c >= 48; // 0
    bool upperBound = (unsigned int) c <= 57; // 9

    return lowerBound && upperBound;
}

// Quick helper function to determine if a particular
// character is whitespace.
bool LexicalAnalyzer::isWhitespace(char c) {
    bool s = (unsigned int) c == 32; // space
    bool r = (unsigned int) c == 13; // return
    bool l = (unsigned int) c == 10; // line feed

    return s || r || l;
}
