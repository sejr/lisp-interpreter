#include <vector>
#include <fstream>
#include <iostream>

#include "Parser.h"
#include "Evaluator.h"
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
            std::cout << "There was an issue opening the lisp file." 
                      << std::endl;
            std::cout << "Please make sure the file name was typed correctly." 
                      << std::endl;
        }
    } else {
        fileBuffer = readFileToBuffer(std::cin);
    }

    unsigned int scannerOrigin = 0;
    LexicalAnalyzer l = LexicalAnalyzer(fileBuffer, scannerOrigin);
    Evaluator e = Evaluator();
    Parser p = Parser(l, e);
    p.start();

    return 0;
}
