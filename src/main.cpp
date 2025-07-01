#include "executor/executor.hpp"
#include "parser/parser.hpp"
#include "parser/validator.hpp"
#include "scanner/lexer.hpp"
#include "visitors/codegen.hpp"

#include <fstream>
#include <iostream>
#include <iterator>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
    return 1;
  }

  std::ifstream in(argv[1]);
  if (!in) {
    std::cerr << "Failed to open file: " << argv[1] << std::endl;
    return 1;
  }

  std::string source((std::istreambuf_iterator<char>(in)),
                     std::istreambuf_iterator<char>());

  pascal::Lexer lexer(source);
  auto tokens = lexer.scanTokens();

  pascal::Parser parser(tokens);
  pascal::AST ast = parser.parse();
  if (!ast.valid || !ast.root) {
    std::cerr << "Parsing failed" << std::endl;
    return 1;
  }

  pascal::ASTValidator validator;
  auto res = validator.validate(ast);
  if (!res.success) {
    std::cerr << res.message;
    if (res.line || res.column)
      std::cerr << " at " << res.line << ':' << res.column;
    std::cerr << std::endl;
    return 1;
  }

  pascal::CodeGenerator codegen;
  std::string asm_code = codegen.generate(ast);

  pascal::Executor exec;
  std::string output = exec.run(asm_code);

  std::cout << output;
  return 0;
}
