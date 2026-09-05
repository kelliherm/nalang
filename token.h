#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

enum TokenType {
    Identifier,
    Number,
    LParen,
    RParen,
    Comma,
    Plus,
    Minus,
    Star,
    Slash,
    Percent,
    Equal,
    Semicolon,
    Let,
    Eof,
};

inline std::string get_token_name(TokenType type) {
    switch (type) {
        case Identifier: return "Identifier";
        case Number: return "Number";
        case LParen: return "LParen";
        case RParen: return "RParen";
        case Comma: return "Comma";
        case Plus: return "Plus";
        case Minus: return "Minus";
        case Star: return "Star";
        case Slash: return "Slash";
        case Percent: return "Percent";
        case Equal: return "Equal";
        case Semicolon: return "Semicolon";
        case Let: return "Let";
        case Eof: return "Eof";
        default: return "Unknown";
    }
}

// A single token produced by the lexer. It stores the token kind, the raw text
// that produced it, and the source line it came from.
class Token {
public:
    Token() : type(Eof), lexeme(""), line(0) {}
    Token(TokenType type, std::string lexeme, unsigned int line) : type(type), lexeme(lexeme), line(line) {}

    friend std::ostream& operator<<(std::ostream& os, const Token& tok) {
        os << "Token Type: " << get_token_name(tok.type) << '\n';
        os << "  Lexeme: " << tok.lexeme << '\n';
        os << "  Line: " << tok.line << '\n';
        return os;
    }

    TokenType get_type() const { return type; }
    std::string get_lexeme() const { return lexeme; }
    unsigned int get_line() const { return line; }

    std::string to_string() const {
        return get_token_name(type);
    }

private:
    TokenType type;
    std::string lexeme;
    unsigned int line;
};

#endif
