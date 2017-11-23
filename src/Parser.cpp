#include "Parser.h"
#include <sstream>

#define OpenParenthesis "OpenParenthesis"
#define ClosingParenthesis "ClosingParenthesis"
#define ERROR "ERROR"
#define Atom "Atom"
#define TEOF "EOF"

using namespace std;

void Parser::Init() {
    string buff;
    while (getline(cin, buff)) {
        InputLine += buff + "\n";
    }
    Current = getNextToken();
}

void Parser::MoveToNext() {
    Current = getNextToken();
}

Token Parser::getNextToken() {
    while (
        InputLine[Index] == 32 || 
        InputLine[Index] == 9  || 
        InputLine[Index] == 13 || 
        InputLine[Index] == 10) {

        Index++;
    }
    if ((InputLine.length()) == (unsigned)Index) {
        return Token(TEOF);
    }
    else if (InputLine[Index] == 40) {
        Index++;
        return Token(OpenParenthesis, "(");
    }
    else if (InputLine[Index] == 41) {
        Index++;
        return Token(ClosingParenthesis, ")");
    }
    else if ((InputLine[Index] >= 65) && (InputLine[Index] <= 90)) {
        string word = string(1, InputLine[Index]);
        int temp = Index;
        while ( 
            ((InputLine[++temp]) >= 65 && (InputLine[temp]) <= 90) || 
            ((InputLine[temp]) >= 48 && (InputLine[temp]) <= 57))
            word += InputLine[temp];
        Index = temp;
        return Token(Atom, word);
    }
    else if ((InputLine[Index] >= 48) && (InputLine[Index] <= 57)) {
        string number = string(1, InputLine[Index]);
        int temp = Index;
        while (
            ((InputLine[++temp]) >= 65 && (InputLine[temp]) <= 90) || 
            ((InputLine[temp]) >= 48 && (InputLine[temp]) <= 57) )
            number += InputLine[temp];
        Index = temp;
        for (unsigned int i = 0; i < number.length(); i++) {
            if (number[i] >= 65 && number[i] <= 90) {
                error = "Invalid token " + number;
                return Token(ERROR);
            }
        }
        return Token(Atom, number);
    }
    else {
        stringstream convert;
        convert << Index + 1;
        error = 
            "Unrecognized character \"" + 
            string(1, InputLine[Index]) + 
            "\" at " + convert.str();
        return Token(ERROR);
    }
}

void Parser::print(stringstream& ss) {
    ss << Current.Content;
}