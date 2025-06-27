#include "lexer.hpp"

namespace pascal {

Lexer::Lexer(std::string_view source) : m_source(source) {}

std::vector<Token> Lexer::scanTokens() {
  return {};
}

} // namespace pascal
