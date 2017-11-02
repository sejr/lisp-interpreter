#include "Parser.h"
#include <sstream>

#define TEOF "EOF"
#define OpenParenthesis "OpenParenthesis"
#define ClosingParenthesis "ClosingParenthesis"
#define Atom "Atom"
#define ERROR "ERROR"

using namespace std;

void Parser::initialize(){
	string buff;
	while (getline(cin, buff)){
		inputLine += buff + "\n";
	}
	currentToken = getNextToken();
}

void Parser::moveToNext(){
	currentToken = getNextToken(); 
}

// "index" is the starting position to get token
// "inputLines" is the string this function is reading
// "error" is for error message. Stays empty unless error occurs.
//  "atoms" is a string vector storing all literal atoms.
//  "sum" is for summing up all numeric atoms.
Token Parser::getNextToken(){
	while (inputLine[index]==32 || inputLine[index]==9 || inputLine[index]==13 || inputLine[index]==10){
		index++;
	}
	if((inputLine.length())==(unsigned)index){
		return Token(TEOF);
	}
	else if(inputLine[index]==40){
		index++;
		return Token(OpenParenthesis,"(");
	}
	else if(inputLine[index]==41){
		index++;
		return Token(ClosingParenthesis,")");
	}
	else if((inputLine[index]>=65) && (inputLine[index]<=90)){
		string word = string(1,inputLine[index]);
		int temp = index;
		while( ((inputLine[++temp])>=65 && (inputLine[temp])<=90) || ((inputLine[temp])>=48 && (inputLine[temp])<=57) )
			word+=inputLine[temp];
		index=temp;
		return Token(Atom,word);
	}
	else if((inputLine[index]>=48) && (inputLine[index]<=57)){
		string number = string(1,inputLine[index]);
		int temp = index;
		while(((inputLine[++temp])>=65 && (inputLine[temp])<=90) || ((inputLine[temp])>=48 && (inputLine[temp])<=57) )
			number+=inputLine[temp];
		index = temp;
		for(unsigned int i =0;i<number.length();i++){
			if (number[i] >=65 && number[i] <=90){
				error = "Invalid token " + number;
				return Token(ERROR);
			}
		}
		return Token(Atom,number);
	}
	else{
		stringstream convert;
		convert << index+1;
		error = "Unrecognized character \"" + string(1,inputLine[index]) + "\" at " + convert.str();
		return Token(ERROR);
	}
}

void Parser::print(stringstream& ss){
	ss << currentToken.content;
}