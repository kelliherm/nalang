#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

// Base class for all expression nodes in the parse tree.
class ExprAST {
public:
    virtual ~ExprAST() = default;
};

// A numeric literal such as 42 or 3.14159.
class NumberExprAST : public ExprAST {
public:
    NumberExprAST(double value) : value(value) {}

    double get_value() const { return value; }

private:
    double value;
};

// A variable reference such as x or total.
class VariableExprAST : public ExprAST {
public:
    VariableExprAST(const std::string& name) : name(name) {}

    const std::string& get_name() const { return name; }

private:
    std::string name;
};

// A binary expression such as x + y or a * b.
class BinaryExprAST : public ExprAST {
public:
    BinaryExprAST(char op, ExprAST* left, ExprAST* right) : op(op), left(left), right(right) {}
    ~BinaryExprAST() override {
        delete left;
        delete right;
    }

    char get_op() const { return op; }
    ExprAST* get_left() const { return left; }
    ExprAST* get_right() const { return right; }

private:
    char op;
    ExprAST* left;
    ExprAST* right;
};

// A function call such as add(1, 2).
class CallExprAST : public ExprAST {
public:
    CallExprAST(const std::string& callee, const std::vector<ExprAST*>& args) : callee(callee), args(args) {}
    ~CallExprAST() override {
        for (ExprAST* arg : args) {
            delete arg;
        }
    }

    const std::string& get_callee() const { return callee; }
    const std::vector<ExprAST*>& get_args() const { return args; }

private:
    std::string callee;
    std::vector<ExprAST*> args;
};

// A function prototype captures the function name and the names of its
// parameters. This is the declaration side of a function.
class PrototypeAST {
public:
    PrototypeAST(const std::string& name, std::vector<std::string> args) : name(name), args(args) {}

    const std::string& get_name() const { return name; }
    const std::vector<std::string>& get_args() const { return args; }

private:
    std::string name;
    std::vector<std::string> args;
};

// A full function definition contains the prototype and the body expression.
class FunctionAST {
public:
    FunctionAST(PrototypeAST* proto, ExprAST* body) : proto(proto), body(body) {}

    PrototypeAST* get_proto() const { return proto; }
    ExprAST* get_body() const { return body; }

private:
    PrototypeAST* proto;
    ExprAST* body;
};

// A variable assignment such as let total = left + right or total = left + right.
class AssignmentExprAST : public ExprAST {
public:
    AssignmentExprAST(const std::string& name, ExprAST* value, bool declaration) : name(name), value(value), declaration(declaration) {}
    ~AssignmentExprAST() override { delete value; }

    const std::string& get_name() const { return name; }
    ExprAST* get_value() const { return value; }
    bool is_declaration() const { return declaration; }

private:
    std::string name;
    ExprAST* value;
    bool declaration;
};

// The root node for a source file. Statements are kept in source order.
class ProgramAST {
public:
    explicit ProgramAST(const std::vector<ExprAST*>& statements) : statements(statements) {}
    ~ProgramAST() {
        for (ExprAST* statement : statements) {
            delete statement;
        }
    }

    const std::vector<ExprAST*>& get_statements() const { return statements; }

private:
    std::vector<ExprAST*> statements;
};

#endif
