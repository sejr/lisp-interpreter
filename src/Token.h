#ifndef TOKEN_H
#define TOKEN_H

#include <string>

typedef enum {
    atomLiteral,
    atomNumeric,
    parenOpen,
    parenClose,
    whitespace,
    error,
    eof
} TokenType;

class Token {
public:
    Token(char tokenChar);
    Token(int numericAtom);
    Token(std::string literalAtom);
    Token(std::string atom, bool is_error);

    TokenType getTokenType();
    std::string repr();
    
private:
    char m_char;
    int m_numericAtom;
    std::string m_literalAtom;
    TokenType m_type;
};

#endif