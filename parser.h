#ifndef PARSER_H
#define PARSER_H

#include <string>

#include "ast.h"
#include "lexer.h"

// The parser turns the lexer's token stream into an abstract syntax tree.
// It uses recursive descent so operator precedence is explicit and easy to
// extend as the language grows.
class Parser {
public:
    explicit Parser(Lexer& lexer);
    explicit Parser(const std::string& source);
    ~Parser();

    // Parse a complete source file and return its program root, or nullptr on
    // the first syntax error.
    ProgramAST* parse();

    // Parse one expression from the current token stream.
    ExprAST* parse_expression();

    // Describe the most recent parse error. An empty string means no error.
    const std::string& get_error() const { return error; }

private:
    Lexer* lexer;
    Lexer owned_lexer;
    Token* current;
    Token* peek;
    std::string error;

    void advance();
    bool expect(TokenType type, const std::string& description);
    void set_error(const std::string& message);

    // Parse either a declaration (`let name = value`) or an update
    // assignment (`name = value`).
    ExprAST* parse_statement();
    ExprAST* parse_additive();
    ExprAST* parse_multiplicative();
    ExprAST* parse_unary();
    ExprAST* parse_primary();
    ExprAST* parse_identifier_expression();
};

#endif
