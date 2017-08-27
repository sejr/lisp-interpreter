class Token {
public:
    int getValue () { return 1; }
};

class AtomToken: public Token {
    // The atom token can be one of two different tokens; a literal atom or a numeric atom.
    // A literal atom has a sequence of capital letters followed by numbers, while the numeric
    // atom just has any sequence of numbers.
};

class OpenParenthesisToken: public Token {
    // The open parenthesis token just corresponds to the literal character '('.
};

class ClosingParenthesisToken: public Token {
    // The opening parenthesis token just corresponds to the literal character ')'.
};

class ErrorToken: public Token {
    // The error token is a special token that indicates that something has gone wrong while
    // parsing the input file.
};

class EOFToken: public Token {
    // This token indicates the file has been parsed successfully.
};