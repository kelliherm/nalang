#ifndef LEXER_H
#define LEXER_H

#include <string>

#include "token.h"

// The lexer is the first stage of the compiler pipeline. It turns a raw source
// string into a stream of tokens that the parser can consume.
class Lexer {
public:
    Lexer();
    explicit Lexer(const std::string& source);

    // Returns the next token in the source string.
    Token* next_token();

private:
    std::string input;
    std::string::iterator pos;
    std::string::iterator readPos;
    char ch;
    unsigned int line;
    unsigned int col;

    // Advance one character and update source position tracking.
    void read_char();
    char peek_char() const;

    // Skip empty space and line terminators between tokens.
    void skip_whitespace();

    // Consume a name or keyword.
    std::string read_identifier();

    // Consume a number literal.
    std::string read_number();

    // Turn a keyword string into a token type.
    TokenType lookup_identifier(const std::string& name) const;

    // Comment handling is kept in the lexer.
    void skip_line_content();
    void skip_block_content();
};

#endif
