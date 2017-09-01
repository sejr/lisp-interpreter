#include "Token.h"

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

Token::Token(int numericAtom) {
    m_numericAtom = numericAtom;
    m_type = atomNumeric;
}

Token::Token(std::string literalAtom) {
    m_literalAtom = literalAtom;
    if (literalAtom == "err") {
        m_type = error;
    } else if (literalAtom == "eof") {
        m_type = eof;
    } else if (literalAtom == "whitespace") {
        m_type = whitespace;
    } else {
        m_type = atomLiteral;
    }
}

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

TokenType Token::getTokenType() {
    return m_type;
}

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
    } else {
        s = "???";
    }

    return s;
}