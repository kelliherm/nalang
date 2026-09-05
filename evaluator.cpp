#include "evaluator.h"

#include <cmath>

bool Evaluator::execute(const ProgramAST* program) {
    error.clear();
    for (ExprAST* statement : program->get_statements()) {
        AssignmentExprAST* assignment = dynamic_cast<AssignmentExprAST*>(statement);
        if (assignment == nullptr || !assign(assignment)) {
            return false;
        }
    }
    return true;
}

bool Evaluator::get_variable(const std::string& name, double& value) const {
    std::map<std::string, double>::const_iterator found = variables.find(name);
    if (found == variables.end()) {
        return false;
    }
    value = found->second;
    return true;
}

bool Evaluator::assign(const AssignmentExprAST* assignment) {
    if (assignment->is_declaration() &&
        variables.find(assignment->get_name()) != variables.end()) {
        set_error("Variable '" + assignment->get_name() + "' is already declared");
        return false;
    }

    if (!assignment->is_declaration() &&
        variables.find(assignment->get_name()) == variables.end()) {
        set_error("Cannot assign to undeclared variable '" + assignment->get_name() + "'");
        return false;
    }

    double value = 0;
    if (!evaluate(assignment->get_value(), value)) {
        return false;
    }
    variables[assignment->get_name()] = value;
    return true;
}

bool Evaluator::evaluate(const ExprAST* expression, double& value) {
    if (const NumberExprAST* number = dynamic_cast<const NumberExprAST*>(expression)) {
        value = number->get_value();
        return true;
    }

    if (const VariableExprAST* variable =
            dynamic_cast<const VariableExprAST*>(expression)) {
        if (!get_variable(variable->get_name(), value)) {
            set_error("Cannot read undeclared variable '" + variable->get_name() + "'");
            return false;
        }
        return true;
    }

    if (const BinaryExprAST* binary =
            dynamic_cast<const BinaryExprAST*>(expression)) {
        double left = 0;
        double right = 0;
        if (!evaluate(binary->get_left(), left) ||
            !evaluate(binary->get_right(), right)) {
            return false;
        }

        switch (binary->get_op()) {
            case '+': value = left + right; return true;
            case '-': value = left - right; return true;
            case '*': value = left * right; return true;
            case '/':
                if (right == 0) {
                    set_error("Cannot divide by zero");
                    return false;
                }
                value = left / right;
                return true;
            case '%':
                if (right == 0) {
                    set_error("Cannot take a remainder with zero");
                    return false;
                }
                value = std::fmod(left, right);
                return true;
            default:
                set_error("Unsupported binary operator");
                return false;
        }
    }

    set_error("Unsupported expression");
    return false;
}

void Evaluator::set_error(const std::string& message) {
    if (error.empty()) {
        error = message;
    }
}
