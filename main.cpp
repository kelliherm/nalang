#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "parser.h"
#include "evaluator.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Welcome to Sodium!" << std::endl;
        return 0;
    }

    const std::string filename = argv[1];
    std::ifstream source_file(filename);
    if (!source_file) {
        std::cerr << "Unable to open source file: " << filename << '\n';
        return 1;
    }

    std::stringstream source;
    source << source_file.rdbuf();

    Parser parser(source.str());
    ProgramAST* program = parser.parse();
    if (program == nullptr) {
        std::cerr << "Parse error: " << parser.get_error() << '\n';
        return 1;
    }

    Evaluator evaluator;
    if (!evaluator.execute(program)) {
        std::cerr << "Runtime error: " << evaluator.get_error() << '\n';
        delete program;
        return 1;
    }

    std::cout << "Executed " << program->get_statements().size()
              << " statement(s) from " << filename << '\n';
    delete program;
    return 0;
}
