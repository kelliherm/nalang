#include <string>

#include "lexer.h"
#include "token.h"

// Default constructor is used mainly for empty or partially built lexer states.
Lexer::Lexer() : input(), pos(input.begin()), readPos(input.begin()), ch('\0'), line(1), col(0) {}

Lexer::Lexer(const std::string& source) : input(source), pos(input.begin()), readPos(input.begin()), ch('\0'), line(1), col(0) {
    read_char();
}

// Consume the next token from the source stream.
Token* Lexer::next_token() {
    skip_whitespace();

    if (ch == '=') {
        Token* token = new Token(Equal, "=", line);
        read_char();
        return token;
    }

    if (ch == ';') {
        Token* token = new Token(Semicolon, ";", line);
        read_char();
        return token;
    }

    if (ch == ',') {
        Token* token = new Token(Comma, ",", line);
        read_char();
        return token;
    }

    if (ch == '+') {
        Token* token = new Token(Plus, "+", line);
        read_char();
        return token;
    }

    if (ch == '-') {
        Token* token = new Token(Minus, "-", line);
        read_char();
        return token;
    }

    if (ch == '*') {
        Token* token = new Token(Star, "*", line);
        read_char();
        return token;
    }

    if (ch == '/') {
        if (peek_char() == '/') {
            read_char();
            read_char();
            skip_line_content();
            return next_token();
        }

        if (peek_char() == '*') {
            read_char();
            read_char();
            skip_block_content();
            return next_token();
        }

        Token* token = new Token(Slash, "/", line);
        read_char();
        return token;
    }

    if (ch == '%') {
        Token* token = new Token(Percent, "%", line);
        read_char();
        return token;
    }

    if (ch == '(') {
        Token* token = new Token(LParen, "(", line);
        read_char();
        return token;
    }

    if (ch == ')') {
        Token* token = new Token(RParen, ")", line);
        read_char();
        return token;
    }

    if (ch == '\0') {
        return new Token(Eof, "", line);
    }

    if (std::isalpha(static_cast<unsigned char>(ch)) || ch == '_') {
        std::string identifier = read_identifier();
        TokenType token_type = lookup_identifier(identifier);
        return new Token(token_type, identifier, line);
    }

    if (std::isdigit(static_cast<unsigned char>(ch))) {
        std::string number = read_number();
        return new Token(Number, number, line);
    }

    // Anything else is treated as an unrecognized symbol.
    std::string symbol(1, ch);
    read_char();
    return new Token(Identifier, symbol, line);
}

// Advance to the next character in the source string.
void Lexer::read_char() {
    if (readPos == input.end()) {
        ch = '\0';
        pos = readPos;
        return;
    }

    ch = *readPos;
    pos = readPos;
    ++readPos;

    if (ch == '\n') {
        ++line;
        col = 0;
    } else if (ch != '\0') {
        ++col;
    }
}

char Lexer::peek_char() const {
    if (readPos == input.end()) {
        return '\0';
    }
    return *readPos;
}

void Lexer::skip_whitespace() {
    while (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
        read_char();
    }
}

std::string Lexer::read_identifier() {
    std::string::iterator start = pos;
    while (std::isalnum(static_cast<unsigned char>(ch)) || ch == '_') {
        read_char();
    }
    return std::string(start, pos);
}

std::string Lexer::read_number() {
    std::string::iterator start = pos;

    while (std::isdigit(static_cast<unsigned char>(ch))) {
        read_char();
    }

    if (ch == '.') {
        read_char();
        while (std::isdigit(static_cast<unsigned char>(ch))) {
            read_char();
        }
    }

    return std::string(start, pos);
}

TokenType Lexer::lookup_identifier(const std::string& name) const {
    if (name == "let") {
        return Let;
    }
    return Identifier;
}

void Lexer::skip_line_content() {
    while (ch != '\n' && ch != '\0') {
        read_char();
    }
}

void Lexer::skip_block_content() {
    read_char();
    while (ch != '\0') {
        if (ch == '*' && peek_char() == '/') {
            read_char();
            read_char();
            break;
        }
        read_char();
    }
}
