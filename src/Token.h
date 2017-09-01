#include <string>
#include <map>

typedef enum {
    atom,
    openParen,
    closeParen,
    error,
    eof
} TokenType;

/* BEGIN TOKEN ***************************************************************/

class Token {
public:
    std::string getLiteralAtom();
    int getNumericAtom();
    char getCharAtom();

    Token(char tokenChar) : m_char(tokenChar) {
        // std::cout << "Initialized character token." << std::endl;
        switch (tokenChar) {
            case '(':
                m_type = openParen;
                break;
            case ')':
                m_type = closeParen;
                break;
            default:
                m_type = error;
                break;
        }
    }

    Token(std::string literalAtom) : m_literalAtom(literalAtom) {
        // std::cout << "Initialized literal token." << std::endl;
        m_type = atom;
    }

    Token(int numericAtom) : m_numericAtom(numericAtom) {
        // std::cout << "Initialized numeric token." << std::endl;
        m_type = atom;
    }
    
private:
    char m_char;
    int m_numericAtom;
    std::string m_literalAtom;
    TokenType m_type;
};

char Token::getCharAtom() {
    return m_char;
}

int Token::getNumericAtom() {
    return m_numericAtom;
}

std::string Token::getLiteralAtom() {
    return m_literalAtom;
}

/* BEGIN TOKENPROFILE ********************************************************/

class TokenProfile {
public:
    int openParenCount;
    int closeParenCount;
    std::vector<Token> literalAtoms;
    std::vector<Token> numericAtoms;
    std::vector<Token> orderedTokens;
    void displayTokenInfo();

    TokenProfile() {
        openParenCount = 0;
        closeParenCount = 0;
    }
};

void TokenProfile::displayTokenInfo() {
    std::cout << "LITERAL ATOMS: " << literalAtoms.size() << ", ";
    for (int i = 0; i < literalAtoms.size() - 1; i++) {
        std::cout << literalAtoms.at(i).getLiteralAtom() << ", ";
    }
    std::cout << literalAtoms.at(literalAtoms.size() - 1).getLiteralAtom();
    std::cout << std::endl;

    int sum = 0;
    std::cout << "NUMERIC ATOMS: " << numericAtoms.size() << ", ";
    for (int i = 0; i < numericAtoms.size(); i++) {
        sum += numericAtoms.at(i).getNumericAtom();
    }
    std::cout << std::to_string(sum) << std::endl;
    
    std::cout << "OPEN PARENTHESES: " << openParenCount << std::endl;
    std::cout << "CLOSING PARENTHESES: " << closeParenCount << std::endl;
}