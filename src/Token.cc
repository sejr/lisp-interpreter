#include "Token.h"

Token::Token() {
    m_type = nil;
}

// This generates a token instance with an associated character,
// which will be used primarily for opening and closing braces
// of various kinds.
Token::Token(char tokenChar) {
    m_char = tokenChar;
    switch (tokenChar) {
        case '(':
            m_type = parenOpen;
            break;
        case ')':
            m_type = parenClose;
            break;
        default:
            m_type = error;
            break;
    }
}

// This generates a token instance with an associated integer value,
// which is used (obviously) for storing numerical information.
Token::Token(int numericAtom) {
    m_numericAtom = numericAtom;
    m_type = atomNumeric;
}

// This generates a token instance with an associated integer value.
// We also incorporate meta tokens through this interface, including
// those for error, EOF, and whitespace.
Token::Token(std::string literalAtom) {
    m_literalAtom = literalAtom;
    if (literalAtom == "err") {
        m_type = error;
    } else if (literalAtom == "eof") {
        m_type = eof;
    } else if (literalAtom == "whitespace") {
        m_type = whitespace;
    } else if (literalAtom == "nil"){
        m_type = nil;
    } else {
        m_type = atomLiteral;
    }
}

// This generates a token instance that is in an error state. We do
// this so we can store the token literal that caused the error; this
// allows us to tell the user which token caused the problem.
Token::Token(std::string atom, bool is_error) {
    if (is_error) {
        m_type = error;
        m_literalAtom = atom;
    } else {
        // Right now we can only have errors in numeric atoms.
        m_type = atomNumeric;
        std::string::size_type sz;
        int numeric = std::stoi (atom, &sz);
        m_numericAtom = numeric;
    }
}

// Returns the enum value corresponding to the token type.
TokenType Token::getTokenType() {
    return m_type;
}

// Returns a print-friendly representation of the token, mainly used for
// debugging at this point in time. Inspired by Python's __repr__() method.
std::string Token::repr() {
    std::string s;
    if (m_type == TokenType::atomLiteral) {
        s = m_literalAtom;
    } else if (m_type == TokenType::atomNumeric) {
        s = std::to_string((long long int) m_numericAtom);
    } else if (
        m_type == TokenType::parenOpen  ||
        m_type == TokenType::parenClose
    ) {
        std::string temp;
        temp.push_back(m_char);
        s = temp;
    } else if (m_type == whitespace) {
        s = "";
    } else if (m_type == error) {
        s = m_literalAtom;
    } else if (m_type == eof) {
        s = "eof";
    } else if (m_type == nil) {
        s = "NIL";
    } else {
        s = "???";
    }

    return s;
}
