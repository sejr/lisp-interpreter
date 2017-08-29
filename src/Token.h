#include <string>

typedef enum {
    atom,
    openParen,
    closeParen,
    error,
    eof
} TokenType;

class Token {
public:
    Token(char tokenChar) : m_char(tokenChar) {
        std::cout << "Initialized character token." << std::endl;
        switch (tokenChar) {
            case '(':
                m_type = TokenType::openParen;
                break;
            case ')':
                m_type = TokenType::closeParen;
                break;
            default:
                m_type = TokenType::error;
                break;
        }
    }

    Token(std::string literalAtom) : m_literalAtom(literalAtom) {
        std::cout << "Initialized literal token." << std::endl;
        m_type = TokenType::atom;
    }

    Token(int numericAtom) : m_numericAtom(numericAtom) {
        std::cout << "Initialized numeric token." << std::endl;
        m_type = TokenType::atom;
    }
    
private:
    char m_char;
    int m_numericAtom;
    std::string m_literalAtom;
    TokenType m_type;
}