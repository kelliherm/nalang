#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <map>
#include <string>

#include "ast.h"

// Evaluates the AST and stores declared variables in memory for later
// assignments and variable references.
class Evaluator {
public:
    // Execute every statement in source order. Returns false if execution
    // fails, such as assigning to a variable that has not been declared.
    bool execute(const ProgramAST* program);

    // Describe the most recent execution error.
    const std::string& get_error() const { return error; }

    // Read a variable after successful execution.
    bool get_variable(const std::string& name, double& value) const;

private:
    std::map<std::string, double> variables;
    std::string error;

    bool evaluate(const ExprAST* expression, double& value);
    bool assign(const AssignmentExprAST* assignment);
    void set_error(const std::string& message);
};

#endif
