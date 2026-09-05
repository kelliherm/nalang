#include "parser.h"

#include <cstdlib>

Parser::Parser(Lexer& lexer) : lexer(&lexer), owned_lexer(), current(nullptr), peek(nullptr), error() {
    advance();
    advance();
}

Parser::Parser(const std::string& source) : lexer(&owned_lexer), owned_lexer(source), current(nullptr), peek(nullptr), error() {
    advance();
    advance();
}

Parser::~Parser() {
    delete current;
    delete peek;
}

void Parser::advance() {
    delete current;
    current = peek;
    peek = lexer->next_token();
}

bool Parser::expect(TokenType type, const std::string& description) {
    if (current->get_type() == type) {
        advance();
        return true;
    }

    set_error(
        "Expected " +description + " on line " +
        std::to_string(current->get_line()) + ", got " + current->get_lexeme()
    );
    return false;
}

void Parser::set_error(const std::string& message) {
    if (error.empty()) {
        error = message;
    }
}

ProgramAST* Parser::parse() {
    std::vector<ExprAST*> statements;

    while (current->get_type() != Eof) {
        ExprAST* statement = parse_statement();
        if (statement == nullptr) {
            for (ExprAST* parsed : statements) {
                delete parsed;
            }
            return nullptr;
        }

        statements.push_back(statement);
        if (!expect(Semicolon, "';'")) {
            for (ExprAST* parsed : statements) {
                delete parsed;
            }
            return nullptr;
        }
    }

    return new ProgramAST(statements);
}

ExprAST* Parser::parse_expression() {
    return parse_additive();
}

ExprAST* Parser::parse_statement() {
    bool declaration = current->get_type() == Let;
    if (declaration) {
        advance();
    } else if (current->get_type() != Identifier) {
        set_error("Expected 'let' or a variable name on line " + std::to_string(current->get_line()));
        return nullptr;
    }

    if (current->get_type() != Identifier) {
        set_error("Expected a variable name on line " + std::to_string(current->get_line()));
        return nullptr;
    }
    std::string name = current->get_lexeme();
    advance();

    if (!expect(Equal, "'='")) {
        return nullptr;
    }

    ExprAST* value = parse_expression();
    if (value == nullptr) {
        return nullptr;
    }
    return new AssignmentExprAST(name, value, declaration);
}

ExprAST* Parser::parse_additive() {
    ExprAST* expression = parse_multiplicative();
    while (expression != nullptr && (current->get_type() == Plus || current->get_type() == Minus)) {
        char op = current->get_lexeme()[0];
        advance();
        ExprAST* right = parse_multiplicative();
        if (right == nullptr) {
            delete expression;
            return nullptr;
        }
        expression = new BinaryExprAST(op, expression, right);
    }
    return expression;
}

ExprAST* Parser::parse_multiplicative() {
    ExprAST* expression = parse_unary();
    while (expression != nullptr &&
           (current->get_type() == Star || current->get_type() == Slash || current->get_type() == Percent)) {
        char op = current->get_lexeme()[0];
        advance();
        ExprAST* right = parse_unary();
        if (right == nullptr) {
            delete expression;
            return nullptr;
        }
        expression = new BinaryExprAST(op, expression, right);
    }
    return expression;
}

ExprAST* Parser::parse_unary() {
    if (current->get_type() == Minus) {
        advance();
        ExprAST* operand = parse_unary();
        if (operand == nullptr) {
            return nullptr;
        }
        return new BinaryExprAST('-', new NumberExprAST(0), operand);
    }
    return parse_primary();
}

ExprAST* Parser::parse_primary() {
    if (current->get_type() == Number) {
        double value = std::strtod(current->get_lexeme().c_str(), nullptr);
        advance();
        return new NumberExprAST(value);
    }

    if (current->get_type() == Identifier) {
        return parse_identifier_expression();
    }

    if (current->get_type() == LParen) {
        advance();
        ExprAST* expression = parse_expression();
        if (expression == nullptr || !expect(RParen, "')'")) {
            delete expression;
            return nullptr;
        }
        return expression;
    }

    set_error(
        "Expected an expression on line " +
        std::to_string(current->get_line()) + ", got " + current->get_lexeme()
    );
    return nullptr;
}

ExprAST* Parser::parse_identifier_expression() {
    std::string name = current->get_lexeme();
    advance();

    if (current->get_type() != LParen) {
        return new VariableExprAST(name);
    }
    advance();

    std::vector<ExprAST*> args;
    if (current->get_type() != RParen) {
        while (true) {
            ExprAST* argument = parse_expression();
            if (argument == nullptr) {
                for (ExprAST* parsed : args) {
                    delete parsed;
                }
                return nullptr;
            }
            args.push_back(argument);
            if (current->get_type() != Comma) {
                break;
            }
            advance();
        }
    }

    if (!expect(RParen, "')'")) {
        for (ExprAST* argument : args) {
            delete argument;
        }
        return nullptr;
    }
    return new CallExprAST(name, args);
}
