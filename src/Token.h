#ifndef TOKEN_H
#define TOKEN_H

#include <string>

// The TokenType is a short-hand way to uniquely identify
// different types of tokens. This list will likely be
// extended in the future.
typedef enum {
    atomLiteral,
    atomNumeric,
    parenOpen,
    parenClose,
    whitespace,
    error,
    eof,
    nil
} TokenType;

// The token is an object which provides information about
// a single piece of code in our lisp interpreter. Tokens are
// generated through the LexicalAnalyzer, then parsed using
// the parsing module that hasn't been built yet.
class Token {
public:
    Token();
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
