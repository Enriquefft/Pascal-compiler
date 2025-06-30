#include "parser/parser.hpp"

namespace pascal {

Parser::Parser(const std::vector<Token> &tokens) : m_tokens(tokens) {}

AST Parser::parse() {
  AST ast;
  ast.valid = true;
  return ast;
}

} // namespace pascal
