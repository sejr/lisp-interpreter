#include <vector>
#include <fstream>
#include <iostream>
#include "LexicalAnalyzer.h"

/*
int main(int argc, char *argv[]) {

    for (int i = 0; i < argc; i++) {
        cout << argv[i] << endl;
    }

    LexicalAnalyzer l;
    Token next = l.getNextToken();
    cout << next.getValue() << endl;
}
*/

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
    std::vector<char> fileBuffer;

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
    l.parseFile(fileBuffer);

    return 0;
}