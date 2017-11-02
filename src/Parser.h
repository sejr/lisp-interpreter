#include <string>
#include <iostream>
#include <sstream>
#include "Token.h"

#ifndef SCANNER_H
#define SCANNER_H

using namespace std;

struct Parser {
	int index;
	string inputLine, error;
	Token currentToken;
	void initialize();// read file
	void moveToNext();
	Token getNextToken();
	void print(stringstream& ss);
};

#endif