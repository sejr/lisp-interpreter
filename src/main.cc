#include <vector>
#include <fstream>
#include <iostream>

#include "LexicalAnalyzer.h"
#include "TokenProfile.h"
#include "Token.h"

// Helper function that just takes the input file and returns a vector
// of characters. This makes the LexicalAnalyzer's job a lot easier.
std::vector<char> readFileToBuffer(std::istream& inputFile) {
    std::vector<char> buffer;
    char c = inputFile.get();
    while (inputFile.good()) {
        buffer.push_back(c);
        c = inputFile.get();
    }

    return buffer;
}

int main(int argc, char** argv) {
    std::vector<char> fileBuffer; // For storing the file contents.

    if ( argc > 1 ) {
        std::ifstream inputFile(argv[1]);

        if (inputFile) {
            fileBuffer = readFileToBuffer(inputFile);
        } else {
            std::cout << "There was an issue opening the lisp file." << std::endl;
            std::cout << "Please make sure the file name was typed correctly." << std::endl;
        }
    } else {
        fileBuffer = readFileToBuffer(std::cin);
    }

    LexicalAnalyzer l;

    unsigned int position = 0;
    std::vector<Token> tokens;
    Token t = l.getNextToken(fileBuffer, position);
    tokens.push_back(t);

    while (
        t.getTokenType() != eof &&
        t.getTokenType() != error
    ) {
        // std::cout << t.repr() << std::endl;
        t = l.getNextToken(fileBuffer, position);
        if (t.getTokenType() != whitespace) {
            tokens.push_back(t);
        }
    }

    TokenProfile tp = TokenProfile(tokens);
    tp.displayTokenInfo();

    return 0;
}
